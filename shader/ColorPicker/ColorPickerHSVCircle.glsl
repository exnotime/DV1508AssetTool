#new_shader compute
#version 430
layout(local_size_x = 32, local_size_y = 32) in;

uniform float hue;

layout(binding = 0, rgba8) writeonly uniform image2D Tex;

void main(){
	ivec2 threadIndex = ivec2( gl_GlobalInvocationID.xy );
	ivec2 texSize = imageSize(Tex);
	vec2 uv = vec2(threadIndex) / vec2(texSize);
	vec2 av = vec2(uv.x*-2.0f, uv.y*-2.0f) + vec2(1.0f, 1.0f);

	float vlength = length(av);

	if(vlength <= 1.0f)
	{
		float deg = atan(av.y,av.x);

		float h = (deg+3.14f)*(180/3.14f); // Radian to degrees
		float s = vlength;
		float v = 1.0f;//
		float c = v*s;
		float x = c*(1.0f-abs(mod(h/60.0f,2.0f)-1.0f));
		float m = v-c;


		vec4 color = vec4(m,m,m,1.0f);
		if(h < 60)
		{
			color.x = c + m;
			color.y = x + m;
		}
		else if (h < 120)
		{
			color.y = c + m;
			color.x = x + m;
		}
		else if (h < 180)
		{
			color.y = c + m;
			color.z = x + m;
		}
		else if (h < 240)
		{
			color.z = c + m;
			color.y = x + m;
		}
		else if (h < 300)
		{
			color.z = c + m;
			color.x = x + m;
		}
		else
		{
			color.x = c + m;
			color.z = x + m;
		}
	
		imageStore( Tex, ivec2(uv * texSize), color);
	}
	else
	{
		imageStore( Tex, ivec2(uv * texSize), vec4(0.0f,0.0f,0.0f,0.5f));
	}
}
#end_shader