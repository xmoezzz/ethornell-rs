//! JSON form of the UserB0 input dialogs for embedding hosts (mobile, web,
//! consoles). The host shows the request with its own UI and answers with a
//! response object; this module turns that answer back into the VM's typed
//! response, applying the same filters and byte limits as the desktop dialogs.
//!
//! Request (`type` selects the shape):
//! - `{"type":"input","title":..,"fields":[{"label","initial","max_bytes","numeric"}]}`
//!   with one or two fields;
//! - `{"type":"segmented","title","prompt","segments","max_bytes","numeric"}`;
//! - `{"type":"select","title","prompt","options":[..]}`;
//! - `{"type":"date","fields":[4 strings],"month","day"}` (zero-based indices).
//!
//! Response: `{"accepted":bool,"texts":[..],"index":n,"month":n,"day":n}`.
//! `texts` holds one entry per field or segment, `index` the chosen option.
//! `"accepted":false` (or a missing value) cancels.
use crate::host_dialog::{TextInputConstraint, constrain_text};
use ethornell_vm::{UserDialogRequest, UserDialogResponse};
use serde_json::{Value, json};

/// The short name a host event carries before the payload is fetched.
pub(crate) fn dialog_kind(request: &UserDialogRequest) -> &'static str {
    match request {
        UserDialogRequest::Input { .. } | UserDialogRequest::TwoField { .. } => "input",
        UserDialogRequest::Segmented { .. } => "segmented",
        UserDialogRequest::Selection { .. } => "select",
        UserDialogRequest::DateFields { .. } => "date",
    }
}

pub(crate) fn request_json(request: &UserDialogRequest) -> String {
    let field = |label: &str, initial: &str, max_bytes: usize, numeric: bool| {
        json!({"label": label, "initial": initial, "max_bytes": max_bytes, "numeric": numeric})
    };
    let value = match request {
        UserDialogRequest::Input { title, initial, max_bytes, numeric } => json!({
            "type": "input",
            "title": title,
            "fields": [field(title, initial, *max_bytes, *numeric)],
        }),
        UserDialogRequest::TwoField {
            title,
            first_label,
            first_initial,
            first_max_bytes,
            first_numeric,
            second_label,
            second_initial,
            second_max_bytes,
            second_numeric,
        } => json!({
            "type": "input",
            "title": title,
            "fields": [
                field(first_label, first_initial, *first_max_bytes, *first_numeric),
                field(second_label, second_initial, *second_max_bytes, *second_numeric),
            ],
        }),
        UserDialogRequest::Segmented {
            title,
            prompt,
            segment_count,
            max_bytes_per_segment,
            numeric,
        } => json!({
            "type": "segmented",
            "title": title,
            "prompt": prompt,
            "segments": segment_count,
            "max_bytes": max_bytes_per_segment,
            "numeric": numeric,
        }),
        UserDialogRequest::Selection { title, prompt, options } => json!({
            "type": "select",
            "title": title,
            "prompt": prompt,
            "options": options,
        }),
        UserDialogRequest::DateFields { fields, month_index, day_index } => json!({
            "type": "date",
            "fields": fields,
            "month": month_index,
            "day": day_index,
        }),
    };
    value.to_string()
}

fn constraint(numeric: bool) -> TextInputConstraint {
    if numeric { TextInputConstraint::SignedDecimal } else { TextInputConstraint::Any }
}

/// Decode a host answer for `request`. Ok(None) is a cancel; Err is a
/// malformed answer, which the caller reports instead of guessing.
pub(crate) fn response_from_json(
    request: &UserDialogRequest,
    text: &str,
) -> Result<Option<UserDialogResponse>, String> {
    let value: Value =
        serde_json::from_str(text).map_err(|error| format!("dialog response is not JSON: {error}"))?;
    if !value.get("accepted").and_then(Value::as_bool).unwrap_or(false) {
        return Ok(None);
    }
    let texts = value
        .get("texts")
        .and_then(Value::as_array)
        .map(|texts| texts.iter().map(|text| text.as_str().unwrap_or_default().to_string()).collect())
        .unwrap_or_else(Vec::<String>::new);
    let text_at = |index: usize| texts.get(index).cloned().unwrap_or_default();
    let integer = |name: &str| {
        value
            .get(name)
            .and_then(Value::as_i64)
            .and_then(|number| i32::try_from(number).ok())
            .ok_or_else(|| format!("dialog response lacks integer `{name}`"))
    };
    Ok(Some(match request {
        UserDialogRequest::Input { max_bytes, numeric, .. } => {
            UserDialogResponse::Input(constrain_text(&text_at(0), *max_bytes, constraint(*numeric)))
        }
        UserDialogRequest::TwoField {
            first_max_bytes,
            first_numeric,
            second_max_bytes,
            second_numeric,
            ..
        } => UserDialogResponse::TwoField([
            constrain_text(&text_at(0), *first_max_bytes, constraint(*first_numeric)),
            constrain_text(&text_at(1), *second_max_bytes, constraint(*second_numeric)),
        ]),
        UserDialogRequest::Segmented { segment_count, max_bytes_per_segment, numeric, .. } => {
            // The desktop dialog filters each segment (decimal or ASCII
            // alphanumeric) and joins them with '-'.
            let filter = if *numeric {
                TextInputConstraint::SignedDecimal
            } else {
                TextInputConstraint::Alphanumeric
            };
            UserDialogResponse::Segmented(
                (0..*segment_count)
                    .map(|index| constrain_text(&text_at(index), *max_bytes_per_segment, filter))
                    .collect::<Vec<_>>()
                    .join("-"),
            )
        }
        UserDialogRequest::Selection { options, .. } => {
            let index = integer("index")?;
            let option = usize::try_from(index)
                .ok()
                .and_then(|index| options.get(index))
                .ok_or_else(|| format!("dialog response index {index} is not an option"))?;
            UserDialogResponse::Selection(option.clone())
        }
        UserDialogRequest::DateFields { .. } => {
            // As the desktop dialog: 10-byte fields, month 0..=11 and a day
            // within that month (February has 29).
            const DAYS: [i32; 12] = [31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];
            let month_index = integer("month")?.clamp(0, 11);
            UserDialogResponse::DateFields {
                fields: std::array::from_fn(|index| {
                    constrain_text(&text_at(index), 10, TextInputConstraint::Any)
                }),
                month_index,
                day_index: integer("day")?.clamp(0, DAYS[month_index as usize] - 1),
            }
        }
    }))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn two_field_requests_round_trip_with_desktop_filters() {
        let request = UserDialogRequest::TwoField {
            title: "Name".into(),
            first_label: "Family".into(),
            first_initial: "山田".into(),
            first_max_bytes: 4,
            first_numeric: false,
            second_label: "Age".into(),
            second_initial: String::new(),
            second_max_bytes: 3,
            second_numeric: true,
        };
        let json: Value = serde_json::from_str(&request_json(&request)).unwrap();
        assert_eq!(json["type"], "input");
        assert_eq!(json["fields"][1]["numeric"], true);
        assert_eq!(dialog_kind(&request), "input");
        assert_eq!(
            response_from_json(&request, r#"{"accepted":true,"texts":["山田太郎","-1a2b3"]}"#),
            Ok(Some(UserDialogResponse::TwoField(["山田".into(), "-12".into()])))
        );
        assert_eq!(response_from_json(&request, r#"{"accepted":false}"#), Ok(None));
        assert!(response_from_json(&request, "nope").is_err());
    }

    #[test]
    fn selection_and_segments_follow_the_desktop_answers() {
        let select = UserDialogRequest::Selection {
            title: "t".into(),
            prompt: "p".into(),
            options: vec!["A".into(), "B".into()],
        };
        assert_eq!(
            response_from_json(&select, r#"{"accepted":true,"index":1}"#),
            Ok(Some(UserDialogResponse::Selection("B".into())))
        );
        assert!(response_from_json(&select, r#"{"accepted":true,"index":2}"#).is_err());
        let segments = UserDialogRequest::Segmented {
            title: "t".into(),
            prompt: "p".into(),
            segment_count: 4,
            max_bytes_per_segment: 2,
            numeric: false,
        };
        assert_eq!(
            response_from_json(&segments, r#"{"accepted":true,"texts":["ab-c","de","f","ghi"]}"#),
            Ok(Some(UserDialogResponse::Segmented("ab-de-f-gh".into())))
        );
        let date = UserDialogRequest::DateFields {
            fields: Default::default(),
            month_index: 0,
            day_index: 0,
        };
        assert_eq!(
            response_from_json(
                &date,
                r#"{"accepted":true,"texts":["a","b","c","0123456789AB"],"month":1,"day":30}"#
            ),
            Ok(Some(UserDialogResponse::DateFields {
                fields: ["a".into(), "b".into(), "c".into(), "0123456789".into()],
                month_index: 1,
                day_index: 28,
            }))
        );
    }
}
