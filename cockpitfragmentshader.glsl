#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D cockpittexture;
void main() {
    FragColor = texture(cockpittexture, TexCoords);
}