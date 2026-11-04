#version 460
layout(binding=0) uniform sampler2D tex;
layout(location=0) in vec2 uv;
layout(location=1) in float alpha;
layout(location=2) in float xrgb;
layout(location=0) out vec4 color;
void main(){vec4 c=texture(tex,uv);color=vec4(c.rgb,(xrgb>0.5?1.0:c.a)*alpha);}
