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

uniform 	sampler2D 	g_DiffuseTex;
uniform		samplerCube	g_IrradianceCubeTex;
uniform		samplerCube	g_SkyCubeTex;
uniform  	vec3 		g_Campos;
uniform 	float		g_Roughness;
uniform 	float		g_Metallic;
uniform 	vec3 		g_LightDir;
uniform		vec3		g_CamDir;

out vec4 FragmentColor;
#include LightingFunc.txt
void main(){

	// Only things we need for a directional light
	Light l;
	l.Color = vec4(1.0f);
	l.Direction = normalize(g_LightDir);
	vec3 normal = normalize(NormalW.xyz);
	vec3 baseColor = pow(texture(g_DiffuseTex, vec2(TexOut.x,TexOut.y)).xyz, vec3(2.2)); //flip y in uv since this is gl, also raise to 2.2(gamma) to be in linear

	vec4 LightColor = CalcDLight(l, normal, PosW.xyz, g_Campos, baseColor, g_Roughness, g_Metallic);
	FragmentColor = pow(LightColor, vec4(1.0 / 2.2)); 
}
#end_shader