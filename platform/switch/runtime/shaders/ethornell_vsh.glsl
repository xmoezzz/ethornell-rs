#version 460
layout(location=0) in vec2 position;
layout(location=1) in vec2 texcoord;
layout(location=2) in float opacity;
layout(location=3) in float ignore_alpha;
layout(location=0) out vec2 uv;
layout(location=1) out float alpha;
layout(location=2) out float xrgb;
void main(){gl_Position=vec4(position,0.0,1.0);uv=texcoord;alpha=opacity;xrgb=ignore_alpha;}
