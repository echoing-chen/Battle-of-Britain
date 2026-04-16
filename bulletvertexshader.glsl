#version 330 core

layout(location=0) in vec4 position;
layout (std140) uniform matrices{
	mat4 view;
	mat4 projection;
};
out float n;

void main(){
	gl_Position=projection*view*vec4(vec3(position),1.0);
	n=position.w;
}