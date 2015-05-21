#new_shader compute
#version 430
layout(local_size_x = 32, local_size_y = 32) in;
uniform vec3 xaxis;
uniform vec3 yaxis;
uniform vec3 selected; // the selected one

layout(binding = 0, rgba8) writeonly uniform image2D Tex;

void main(){
	ivec2 threadIndex = ivec2( gl_GlobalInvocationID.xy );
	ivec2 texSize = imageSize(Tex);
	vec2 uv = vec2(threadIndex) / vec2(texSize);
	
	vec4 color;
	
	color = vec4(xaxis*uv.x + yaxis*uv.y + selected, 1.0f);
	
	imageStore( Tex, ivec2(uv * texSize), color);
}
#end_shader