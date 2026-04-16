#version 330 core
    in vec2 TexCoord;
    out vec4 FragColor;
    uniform sampler2D texY;
    uniform sampler2D texU;
    uniform sampler2D texV;
    void main() {
        float y = texture(texY, TexCoord).r;
        float u = texture(texU, TexCoord).r - 0.5;
        float v = texture(texV, TexCoord).r - 0.5;
        
        float r = y + 1.402 * v;
        float g = y - 0.34414 * u - 0.71414 * v;
        float b = y + 1.772 * u;
        
        FragColor = vec4(r, g, b, 1.0);
    }