#new_shader vertex
#version 330
in vec4 PosL;
in vec4 NormalL;
in vec4 TexIn;
in vec4 TangentL;

out vec4 PosW;
out vec4 NormalW;
out vec4 TexOut;
out vec4 TangentW;

uniform mat4 g_World;
uniform mat4 g_ViewProj;

void main(){
	PosW = g_World * vec4(PosL.xyz,1);
	NormalW = g_World * vec4(NormalL.xyz,0);
	TangentW = g_World * vec4(TangentL.xyz,0);
	TexOut = TexIn;

	gl_Position = g_ViewProj * PosW;
}
#end_shader

#new_shader fragment
#version 330
in vec4 PosW;
in vec4 NormalW;
in vec4 TexOut;
in vec4 TangentW;

uniform sampler2D g_DiffuseTex; //Texture
out vec4 FragmentColor;
void main(){

	vec3 normal = normalize(NormalW.xyz);

	float light =  dot(normal, vec3(0,1,0)) * 0.5 + 0.5;
	FragmentColor = vec4(texture(g_DiffuseTex, vec2(TexOut.x, 1.0 - TexOut.y)).xyz * light,1.0); //flip y in uv since this is gl 
}
#end_shader