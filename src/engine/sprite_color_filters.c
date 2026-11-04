/* Sprite property11:428940 ->42AF80. Sixteen12-byte records.
 * packed&FF = slot0..15; (packed>>8)&FF = kind0..5;
 * packed>>16 = strength WORD; extra DWORD = RGB parameter.
 * The old port mistook upper WORD for kind and had no renderer consumer.
 * Kind0 disables;1 XOR;2 luma colorization;3 constant color blend;
 *4 saturating add;5 saturating subtract.42B010 applies slots in order,
 * calling419CF0 after sprite sampling/mask/transition. Source for subsequent
 * operations is previous output.410D80/F80,4111E0/3B0,4116F0,4118E0/A00
 * supply scalar16-bit integer equations. Source alpha is preserved;
 * filter processing does not replace the object's final native blit/alpha.
 * RGB sources are promoted into the sprite's RGBA intermediate coverage;
 * transformed sources are processed after current raster geometry/sampling.
 * Rust native_sprite_filters.rs. Mip selection and transformed geometry
 * retain their separate recovery status and are not promoted by this port. */
