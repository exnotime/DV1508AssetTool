#new_shader vertex
#version 330
in vec2 pos;
uniform vec2 g_ScreenSize;
uniform vec2 g_ScreenPos;
void main(){
	gl_Position = vec4((pos.xy / g_ScreenSize.xy) * 2 - 1, 0.5, 1); //translate and scale to fit in viewport
}
#end_shader

#new_shader fragment
#version 330
uniform vec4 g_Color;
out vec4 FragmentColor;
void main(){
	FragmentColor = g_Color;
}
#end_shader