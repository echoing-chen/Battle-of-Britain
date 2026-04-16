#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 model;

out vec2 TexCoords;

layout (std140) uniform matrices{
	mat4 view;
	mat4 projection;
};

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0); 
    TexCoords = aTexCoords;
}