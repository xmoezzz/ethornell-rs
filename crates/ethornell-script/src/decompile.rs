use crate::bcs::{BcsProgram, BcsValue};
use crate::calls::instruction_call_key;
use crate::{BpInstruction, BpOperand, BpProgram};
use std::collections::BTreeMap;

#[derive(Debug, Clone, Default)]
pub struct DecompileOptions {
    pub limit: Option<usize>,
    pub show_stack: bool,
}

pub fn decompile_bp(program: &BpProgram, options: &DecompileOptions) -> String {
    let mut state = DecompileState {
        stack: Vec::new(),
        lines: Vec::new(),
        show_stack: options.show_stack,
        temp_counter: 0,
    };

    if let Some(name) = &program.script_name {
        state.lines.push(format!("// script {name}"));
    }
    for function in &program.functions {
        state
            .lines
            .push(format!("// function sub_{:08X}", function.offset));
    }

    for instruction in program
        .instructions
        .iter()
        .take(options.limit.unwrap_or(usize::MAX))
    {
        state.visit(instruction);
    }

    state.lines.join("\n")
}

/// Render BCS as stack-oriented pseudocode. This deliberately preserves opcode
/// IDs and branch targets so reverse-engineering evidence is not hidden behind
/// guessed high-level syntax.
pub fn decompile_bcs(program: &BcsProgram, options: &DecompileOptions) -> String {
    let mut state = BcsDecompileState {
        stack: Vec::new(),
        lines: vec![format!(
            "// BURIKO scenario code 0x{:X}..0x{:X}",
            program.code_start, program.code_end
        )],
        show_stack: options.show_stack,
        pending_arg_count: None,
        sub_names: program
            .subs
            .iter()
            .map(|sub| (sub.addr, sub.name.as_str()))
            .chain(
                program
                    .symbols
                    .iter()
                    .map(|symbol| (symbol.addr, symbol.name.as_str())),
            )
            .collect(),
    };

    for command in program
        .commands
        .iter()
        .take(options.limit.unwrap_or(usize::MAX))
    {
        state.visit(command);
    }
    state.lines.join("\n")
}

struct BcsDecompileState<'a> {
    stack: Vec<String>,
    lines: Vec<String>,
    show_stack: bool,
    pending_arg_count: Option<usize>,
    sub_names: BTreeMap<u32, &'a str>,
}

impl BcsDecompileState<'_> {
    fn visit(&mut self, command: &crate::bcs::BcsCommand) {
        if let Some(name) = self.sub_names.get(&command.addr) {
            self.lines
                .push(format!("\nsub_{:08X}_{name}:", command.addr));
        }
        let op = command.name.unwrap_or("unknown");
        match op {
            "push_dword" | "push_offset" | "push_base_offset" | "push_string" => {
                for value in &command.args {
                    self.stack.push(format_bcs_value(value));
                }
                self.emit(command, format!("push {}", command_args(&command.args)));
            }
            "nargs" => {
                self.pending_arg_count = command
                    .args
                    .iter()
                    .rev()
                    .find_map(bcs_value_i32)
                    .and_then(|value| usize::try_from(value).ok());
                self.emit(
                    command,
                    format!("argc = {};", self.pending_arg_count.unwrap_or_default()),
                );
            }
            "add" | "sub" | "mul" | "div" | "mod" | "and" | "or" | "xor" | "shl" | "shr"
            | "sar" | "eq" | "neq" | "leq" | "geq" | "lt" | "gt" => {
                let right = self.pop();
                let left = self.pop();
                self.stack
                    .push(format!("({left} {} {right})", bcs_binary_symbol(op)));
                self.emit(command, format!("stack.push({});", self.stack_tail(1)));
            }
            "not" | "bool_zero" => {
                let value = self.pop();
                let expression = if op == "not" {
                    format!("~({value})")
                } else {
                    format!("({value} == 0)")
                };
                self.stack.push(expression);
                self.emit(command, format!("stack.push({});", self.stack_tail(1)));
            }
            "jmp" => {
                let target = self.pop();
                self.emit(command, format!("goto {target};"));
            }
            "jc" => {
                let target = self.pop();
                let condition = self.pop();
                self.emit(command, format!("if ({condition}) goto {target};"));
            }
            "call" => {
                let target = self.pop();
                self.emit(command, format!("call {target};"));
            }
            "resolve_symbol" => {
                let symbol = self.pop();
                let resolved = if self
                    .stack
                    .last()
                    .is_some_and(|value| value.starts_with('"'))
                {
                    let script = self.pop();
                    format!("resolve({script}, {symbol})")
                } else {
                    format!("resolve({symbol})")
                };
                self.stack.push(resolved.clone());
                self.emit(command, format!("stack.push({resolved});"));
            }
            "global_set" => {
                let count = self.pending_arg_count.take().unwrap_or(2);
                let args = self.pop_args(count);
                let index = args.first().cloned().unwrap_or_else(|| "?".into());
                let value = args.get(1).cloned().unwrap_or_else(|| "?".into());
                self.emit(command, format!("global[{index}] = {value};"));
            }
            "global_get" => {
                let count = self.pending_arg_count.take().unwrap_or(1);
                let args = self.pop_args(count);
                let index = args.last().cloned().unwrap_or_else(|| "?".into());
                self.stack.push(format!("global[{index}]"));
                self.emit(command, format!("stack.push(global[{index}]);"));
            }
            "line" => self.emit(
                command,
                format!("debug_line({});", command_args(&command.args)),
            ),
            "source_line" => {
                let count = self.pending_arg_count.take().unwrap_or(2);
                let args = self.pop_args(count);
                self.emit(command, format!("debug_line({});", args.join(", ")));
            }
            "script_call" => {
                let Some(function_index) = self.stack.iter().rposition(|value| {
                    value.starts_with('"') && value.trim_matches('"').starts_with('_')
                }) else {
                    self.emit(command, "script_call(<missing-function>);".to_string());
                    return;
                };
                let function = self.stack[function_index].trim_matches('"').to_string();
                let args = self.stack.drain(..function_index).collect::<Vec<_>>();
                self.stack.clear();
                self.emit(command, format!("{function}({});", args.join(", ")));
            }
            "ret" => self.emit(command, "return;".to_string()),
            "check_translator_note" => {
                let count = self.pending_arg_count.take().unwrap_or(1);
                let value = self.pop_args(count).pop().unwrap_or_else(|| "?".into());
                self.stack.push(format!("note({value})"));
                self.emit(command, format!("stack.push({});", self.stack_tail(1)));
            }
            _ => {
                let arg_count = self.pending_arg_count.take().unwrap_or(0);
                let args = self.pop_args(arg_count);
                let name = format!("{op}_0x{:03X}", command.opcode);
                self.emit(command, format!("{name}({});", args.join(", ")));
            }
        }
    }

    fn pop(&mut self) -> String {
        self.stack
            .pop()
            .unwrap_or_else(|| "<stack-underflow>".to_string())
    }

    fn pop_args(&mut self, count: usize) -> Vec<String> {
        let mut args = Vec::with_capacity(count);
        for _ in 0..count {
            args.push(self.pop());
        }
        args.reverse();
        args
    }

    fn emit(&mut self, command: &crate::bcs::BcsCommand, body: String) {
        if self.show_stack {
            self.lines.push(format!(
                "{:08X}: {body:<72} // stack=[{}]",
                command.addr,
                self.stack_tail(12)
            ));
        } else {
            self.lines.push(format!("{:08X}: {body}", command.addr));
        }
    }

    fn stack_tail(&self, limit: usize) -> String {
        let start = self.stack.len().saturating_sub(limit);
        let mut values = self.stack[start..].to_vec();
        if start > 0 {
            values.insert(0, format!("...{start} more"));
        }
        values.join(", ")
    }
}

fn command_args(args: &[BcsValue]) -> String {
    args.iter()
        .map(format_bcs_value)
        .collect::<Vec<_>>()
        .join(", ")
}

fn format_bcs_value(value: &BcsValue) -> String {
    match value {
        BcsValue::Int(value) => value.to_string(),
        BcsValue::Addr(value) => format!("sub_0x{value:08X}"),
        BcsValue::BaseOffset(value) => format!("base[{value}]"),
        BcsValue::MemoryAddr(value) => format!("mem[0x{value:08X}]"),
        BcsValue::Line { file, line } => format!("line({file:?}, {line})"),
        BcsValue::Arg2 => "arg2".to_string(),
        BcsValue::Str(value) => format!("{value:?}"),
        BcsValue::Mul(left, right) => {
            format!("({} * {})", format_bcs_value(left), format_bcs_value(right))
        }
        BcsValue::CheckNote(value) => format!("note({})", format_bcs_value(value)),
    }
}

fn bcs_value_i32(value: &BcsValue) -> Option<i32> {
    match value {
        BcsValue::Int(value)
        | BcsValue::Addr(value)
        | BcsValue::BaseOffset(value)
        | BcsValue::MemoryAddr(value) => Some(*value),
        BcsValue::Mul(left, right) => Some(bcs_value_i32(left)? * bcs_value_i32(right)?),
        BcsValue::CheckNote(value) => bcs_value_i32(value),
        BcsValue::Line { .. } | BcsValue::Arg2 | BcsValue::Str(_) => None,
    }
}

fn bcs_binary_symbol(name: &str) -> &'static str {
    match name {
        "add" => "+",
        "sub" => "-",
        "mul" => "*",
        "div" => "/",
        "mod" => "%",
        "and" => "&",
        "or" => "|",
        "xor" => "^",
        "shl" => "<<",
        "shr" | "sar" => ">>",
        "eq" => "==",
        "neq" => "!=",
        "leq" => "<=",
        "geq" => ">=",
        "lt" => "<",
        "gt" => ">",
        _ => "?",
    }
}

struct DecompileState {
    stack: Vec<String>,
    lines: Vec<String>,
    show_stack: bool,
    temp_counter: usize,
}

impl DecompileState {
    fn visit(&mut self, instruction: &BpInstruction) {
        let offset = instruction.offset;
        let name = instruction.opcode_name.as_str();
        match name {
            "push_byte" | "push_word" | "push_dword" => {
                self.push(format!("{}", operand_i32(instruction).unwrap_or_default()));
            }
            "push_base_offset" => {
                let displacement = operand_i32(instruction).unwrap_or_default();
                if displacement < 0 {
                    self.push(format!("base[+0x{:X}]", displacement.unsigned_abs()));
                } else {
                    self.push(format!("base[-0x{displacement:X}]"));
                }
            }
            "push_string" => {
                self.push(match instruction.operands.first() {
                    Some(BpOperand::String(text)) => format!("{text:?}"),
                    Some(BpOperand::Offset(value)) => format!("str_0x{value:08X}"),
                    _ => "\"<bad-string>\"".to_string(),
                });
            }
            "push_offset" => {
                let value = operand_u32(instruction).unwrap_or_default();
                self.push(format!("sub_0x{value:08X}"));
            }
            "load_base" => self.push("base".to_string()),
            "store_base" => {
                let value = self.pop();
                self.emit(offset, format!("base = {value};"));
            }
            "load" => {
                let width = operand_u32(instruction).unwrap_or_default();
                let ptr = self.pop();
                self.push(format!("load{width}({ptr})"));
            }
            "move" => {
                let width = operand_u32(instruction).unwrap_or_default();
                let value = self.pop();
                let ptr = self.pop();
                self.emit(offset, format!("store{width}({ptr}, {value});"));
                self.push(value);
            }
            "move_arg" => {
                let width = operand_u32(instruction).unwrap_or_default();
                let ptr = self.pop();
                let value = self.pop();
                self.emit(offset, format!("store_arg{width}({ptr}, {value});"));
            }
            "copy_inline" => {
                let ptr = self.pop();
                let bytes = instruction
                    .operands
                    .first()
                    .and_then(|operand| match operand {
                        BpOperand::Raw(bytes) => Some(bytes.as_slice()),
                        _ => None,
                    })
                    .unwrap_or_default();
                self.emit(offset, format!("copy_inline({ptr}, {bytes:?});"));
            }
            "copy_stack" => {
                let width = operand_u32(instruction).unwrap_or_default();
                let count = instruction
                    .operands
                    .get(1)
                    .and_then(operand_as_u32)
                    .unwrap_or_default() as usize;
                let mut values = Vec::new();
                for _ in 0..count {
                    values.push(self.pop());
                }
                values.reverse();
                let dst = self.pop();
                self.emit(
                    offset,
                    format!("copy_stack{width}({dst}, [{}]);", values.join(", ")),
                );
            }
            "jmp" => {
                let dest = self.pop();
                self.emit(offset, format!("goto {dest};"));
            }
            "jc" => {
                let kind = operand_u32(instruction).unwrap_or_default();
                let dest = self.pop();
                let condition = self.pop();
                self.emit(
                    offset,
                    format!("if {} goto {dest};", condition_for_jc(kind, &condition)),
                );
            }
            "call" => {
                let dest = self.pop();
                self.emit(offset, format!("call {dest};"));
            }
            "ret" | "script_ret" => self.emit(offset, "return;".to_string()),
            "add" | "sub" | "mul" | "div" | "mod" | "and" | "or" | "xor" | "shl" | "shr"
            | "sar" | "eq" | "neq" | "leq" | "geq" | "lt" | "gt" | "boolean_and" | "boolean_or" => {
                let right = self.pop();
                let left = self.pop();
                self.push(format!("({left} {} {right})", binary_symbol(name)));
            }
            "not" => {
                let value = self.pop();
                self.push(format!("~({value})"));
            }
            "bool_zero" => {
                let value = self.pop();
                self.push(format!("({value} == 0)"));
            }
            "ternary" => {
                let false_value = self.pop();
                let true_value = self.pop();
                let condition = self.pop();
                self.push(format!("({condition} ? {true_value} : {false_value})"));
            }
            "muldiv" => {
                let divisor = self.pop();
                let multiplier = self.pop();
                let multiplicand = self.pop();
                self.push(format!("muldiv({multiplicand}, {multiplier}, {divisor})"));
            }
            "sin" | "cos" => {
                let value = self.pop();
                self.push(format!("{name}({value})"));
            }
            "memcpy" | "memclr" | "memset" | "memory_equal" | "memrepeat" | "memfind"
            | "strfind" | "strreplace" | "strlen" | "streq" | "strcpy" | "strconcat"
            | "getchar" | "tolower" | "sprintf" | "malloc" | "free" | "addmemboundary"
            | "confirm" | "message_box" | "show_number" | "dumpmem" | "atan2" | "vec3_length"
            | "quote_string" | "modal_list" | "clipboard_set" | "engine_state" => {
                self.emit_builtin(offset, name);
            }
            "sys1" | "sys2" | "grp1" | "grp2" | "grp3" | "snd1" | "usr1" | "usr2" => {
                self.emit_dispatch(offset, instruction);
            }
            "script_load" | "script_free" | "script_call" => {
                self.emit(
                    offset,
                    format!("{name}({});", format_operands(&instruction.operands)),
                );
            }
            _ => self.emit(
                offset,
                format!("{}({});", name, format_operands(&instruction.operands)),
            ),
        }
    }

    fn push(&mut self, value: String) {
        self.stack.push(value);
    }

    fn pop(&mut self) -> String {
        self.stack.pop().unwrap_or_else(|| {
            let name = format!("stack_underflow_{}", self.temp_counter);
            self.temp_counter += 1;
            name
        })
    }

    fn emit(&mut self, offset: u64, body: String) {
        if self.show_stack {
            self.lines.push(format!(
                "{offset:08X}: {body:<72} // stack=[{}]",
                self.stack_tail(12)
            ));
        } else {
            self.lines.push(format!("{offset:08X}: {body}"));
        }
    }

    fn stack_tail(&self, limit: usize) -> String {
        let len = self.stack.len();
        let start = len.saturating_sub(limit);
        let mut values = self.stack[start..].to_vec();
        if start > 0 {
            values.insert(0, format!("...{} more", start));
        }
        values.join(", ")
    }

    fn emit_builtin(&mut self, offset: u64, name: &str) {
        let arity = match name {
            "strlen" | "getchar" | "tolower" | "malloc" | "free" | "message_box"
            | "show_number" | "clipboard_set" | "engine_state" => 1,
            "memclr" | "streq" | "strcpy" | "strfind" | "confirm" | "modal_list" | "atan2" => 2,
            "memcpy" | "memset" | "memory_equal" | "strconcat" | "sprintf" | "quote_string"
            | "addmemboundary" | "dumpmem" | "vec3_length" => 3,
            "memrepeat" | "memfind" | "strreplace" => 4,
            _ => 0,
        };
        let mut args = self.pop_args(arity);
        if name == "getchar" {
            // Pushes the character, a double-byte flag and a delimiter flag.
            let call = format!("getchar({})", args.join(", "));
            let (ch, dbcs, delim) = (self.next_temp(), self.next_temp(), self.next_temp());
            self.emit(offset, format!("({ch}, {dbcs}, {delim}) = {call};"));
            self.push(ch);
            self.push(dbcs);
            self.push(delim);
            return;
        }
        if matches!(
            name,
            "memory_equal"
                | "memfind"
                | "strfind"
                | "strlen"
                | "streq"
                | "malloc"
                | "free"
                | "confirm"
                | "strreplace"
                | "atan2"
                | "vec3_length"
                | "addmemboundary"
                | "engine_state"
                | "clipboard_set"
                | "modal_list"
        ) {
            let temp = self.next_temp();
            self.emit(offset, format!("{temp} = {name}({});", args.join(", ")));
            self.push(temp);
            return;
        }
        self.emit(offset, format!("{name}({});", args.join(", ")));
        args.clear();
    }

    fn emit_dispatch(&mut self, offset: u64, instruction: &BpInstruction) {
        let Some(key) = instruction_call_key(instruction) else {
            self.emit(offset, format!("{}(<bad-id>);", instruction.opcode_name));
            return;
        };
        let name = key
            .name()
            .map(str::to_string)
            .unwrap_or_else(|| format!("call_{:02X}_{:02X}", key.group, key.id));
        let args = key
            .arg_count()
            .map(|count| self.pop_args(count))
            .unwrap_or_default();
        let arg_text = if args.is_empty() {
            String::new()
        } else {
            args.join(", ")
        };
        let returns_value = key.returns_value();
        let temp = returns_value.then(|| self.next_temp());
        let statement = if let Some(temp) = &temp {
            format!(
                "{temp} = {}({}) /* group=0x{:02X} id=0x{:02X} argc={} stack_top=[{}] */;",
                name,
                arg_text,
                key.group,
                key.id,
                key.arg_count()
                    .map(|count| count.to_string())
                    .unwrap_or_else(|| "?".to_string()),
                self.stack
                    .iter()
                    .rev()
                    .take(8)
                    .cloned()
                    .collect::<Vec<_>>()
                    .join(", ")
            )
        } else {
            format!(
                "{}({}) /* group=0x{:02X} id=0x{:02X} argc={} stack_top=[{}] */;",
                name,
                arg_text,
                key.group,
                key.id,
                key.arg_count()
                    .map(|count| count.to_string())
                    .unwrap_or_else(|| "?".to_string()),
                self.stack
                    .iter()
                    .rev()
                    .take(8)
                    .cloned()
                    .collect::<Vec<_>>()
                    .join(", ")
            )
        };
        self.emit(offset, statement);
        if let Some(temp) = temp {
            self.push(temp);
        }
    }

    fn pop_args(&mut self, arity: usize) -> Vec<String> {
        let mut args = Vec::with_capacity(arity);
        for _ in 0..arity {
            args.push(self.pop());
        }
        args.reverse();
        args
    }

    fn next_temp(&mut self) -> String {
        let temp = format!("t{}", self.temp_counter);
        self.temp_counter += 1;
        temp
    }
}

fn operand_i32(instruction: &BpInstruction) -> Option<i32> {
    instruction
        .operands
        .first()
        .and_then(|operand| match operand {
            BpOperand::U8(value) => Some(*value as i8 as i32),
            BpOperand::U16(value) => Some(*value as i16 as i32),
            BpOperand::U32(value) => Some(*value as i32),
            BpOperand::I32(value) => Some(*value),
            _ => None,
        })
}

fn operand_u32(instruction: &BpInstruction) -> Option<u32> {
    instruction.operands.first().and_then(operand_as_u32)
}

fn operand_as_u32(operand: &BpOperand) -> Option<u32> {
    match operand {
        BpOperand::U8(value) => Some(*value as u32),
        BpOperand::U16(value) => Some(*value as u32),
        BpOperand::U32(value) => Some(*value),
        BpOperand::I32(value) => Some(*value as u32),
        BpOperand::Offset(value) => Some(*value),
        _ => None,
    }
}

fn format_operands(operands: &[BpOperand]) -> String {
    operands
        .iter()
        .map(|operand| format!("{operand:?}"))
        .collect::<Vec<_>>()
        .join(", ")
}

fn binary_symbol(name: &str) -> &'static str {
    match name {
        "add" => "+",
        "sub" => "-",
        "mul" => "*",
        "div" => "/",
        "mod" => "%",
        "and" => "&",
        "or" => "|",
        "xor" => "^",
        "shl" => "<<",
        "shr" => ">>",
        "sar" => ">>>",
        "eq" => "==",
        "neq" => "!=",
        "leq" => "<=",
        "geq" => ">=",
        "lt" => "<",
        "gt" => ">",
        "boolean_and" => "&&",
        "boolean_or" => "||",
        _ => "?",
    }
}

fn condition_for_jc(kind: u32, condition: &str) -> String {
    match kind {
        0 => format!("({condition} != 0)"),
        1 => format!("({condition} == 0)"),
        2 => format!("({condition} > 0)"),
        3 => format!("({condition} >= 0)"),
        4 => format!("({condition} <= 0)"),
        5 => format!("({condition} < 0)"),
        _ => format!("jc{kind}({condition})"),
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::parse_bp_program;

    #[test]
    fn base_pointer_displacement_is_signed_like_target_sub_4735f0() {
        let program = parse_bp_program(None, &[0x04, 0xff, 0xff, 0x17]);
        let output = decompile_bp(
            &program,
            &DecompileOptions {
                show_stack: true,
                ..DecompileOptions::default()
            },
        );

        assert!(output.contains("base[+0x1]"), "{output}");
    }
}
