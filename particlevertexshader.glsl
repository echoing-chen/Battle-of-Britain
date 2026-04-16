#version 330 core
layout (location=0) in vec3 position;
layout (location=1) in vec3 color;
layout (location=2) in float alpha;
layout (std140) uniform matrices{
	mat4 view;
	mat4 projection;
};
out vec3 Pcolor;
out float Palpha;


void main(){
	gl_Position=projection*view*vec4(position,1.0);
	gl_PointSize=5.0f;
	Pcolor=color;
	Palpha=alpha;
}