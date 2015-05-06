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
uniform 	sampler2D 	g_NormalTex;
uniform 	sampler2D 	g_RoughnessTex;
uniform		samplerCube	g_IrradianceCubeTex;
uniform		samplerCube	g_SkyCubeTex;
uniform  	vec3 		g_Campos;
uniform 	float		g_Roughness;
uniform 	float		g_Metallic;
uniform 	vec3 		g_LightDir;
uniform		vec3		g_CamDir;

out vec4 FragmentColor;
#include LightingFunc.txt

vec3 CalcBumpedNormal(vec3 Normal, vec3 Tangent, sampler2D normalMap, vec2 uv){
	vec3 normal = normalize(Normal);
	vec3 tangent = normalize(Tangent);
	tangent = normalize(tangent - dot(tangent,normal) * normal);
	vec3 bitangent = cross(tangent,normal);

	vec3 bump = texture(normalMap,uv).xyz;
	bump = (bump * 2.0) - 1.0;

	vec3 newNormal;
	mat3 TBN = mat3(tangent,bitangent,normal);
	newNormal = TBN * bump;
	return normalize(newNormal);
}
void main(){

	float roughness = 1.0 - texture(g_RoughnessTex,TexOut.xy).r;
	// Only things we need for a directional light
	Light l;
	l.Color = vec4(5.0f);
	l.Direction = normalize(g_LightDir);

	vec3 normal = CalcBumpedNormal(NormalW.xyz, TangentW.xyz, g_NormalTex, TexOut.xy);
	vec4 albedo = texture(g_DiffuseTex, TexOut.xy);
	if(albedo.a <= 0.01f){
		discard;
	}
	vec3 baseColor = pow(albedo.xyz, vec3(2.2)); //raise to 2.2(gamma) to be in linear space

	vec4 LightColor = CalcDLight(l, normal, PosW.xyz, g_Campos, baseColor, roughness, g_Metallic);
	FragmentColor = vec4(pow(LightColor.xyz, vec3(1.0 / 2.2)), albedo.a); 
}
#end_shader