#version 330 core
in vec3 Pcolor;
in float Palpha;
out vec4 FragColor;

void main(){
	FragColor=vec4(Pcolor,Palpha);
}