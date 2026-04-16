#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SDL.h>
#include "shader.hpp"
#include <iostream>
#include <fstream>
#include <vector>


extern int SCR_WIDTH;
extern int SCR_HEIGHT;
extern GLFWwindow* window;


bool begin_state = 1;
const double targetFPS = 30.0;
const double frameDelay = 1 / targetFPS;
double thisframe;
double lastframe;
GLuint texY, texU, texV;
std::vector<unsigned char> yData, uData, vData;
std::ifstream yuvFile;
int frameSize;
SDL_AudioSpec audio_spec;
Uint8* audio_buf;
Uint32 audio_length;
Uint32 audio_pos = 0;
SDL_AudioDeviceID device_id;

void initTextures();
bool initYUV(const char* filePath);
bool readNextFrame();
void callback(void* userdata, Uint8* stream, int len);

int beginvideo() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        SDL_Log("SDL init failed! %s", SDL_GetError());
        return -1;
    }
    float vertices[] = {
        // 位置         // 纹理坐标
        1.0f,  1.0f,    1.0f, 0.0f, // 右上
        1.0f, -1.0f,    1.0f, 1.0f, // 右下
       -1.0f, -1.0f,    0.0f, 1.0f, // 左下
       -1.0f,  1.0f,    0.0f, 0.0f  // 左上
    };
    unsigned int indices[] = { 0, 1, 3, 1, 2, 3 }; // 矩形索引
    Shader myshader("videovertexshader.glsl", "videofragmentshader.glsl");
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 顶点位置属性
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 纹理坐标属性
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);


    initTextures();
    if (!initYUV("begin/begin.yuv")) return -1; // 替换为你的YUV文件路径
    if (SDL_LoadWAV("begin/begin.WAV", &audio_spec, &audio_buf, &audio_length) == NULL) {
        SDL_Log("SDL_LoadWAV failed! %s", SDL_GetError());
        return -1;
    }
    audio_spec.callback = callback;
    device_id = SDL_OpenAudioDevice(NULL, 0, &audio_spec, NULL, 0);
    SDL_PauseAudioDevice(device_id, 0);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    while (!glfwWindowShouldClose(window)&& glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS&&begin_state) {
        thisframe = glfwGetTime();
        if (thisframe - lastframe < frameDelay) {
            continue;
        }
        lastframe = thisframe;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (!readNextFrame()) {
            yuvFile.clear();
            //yuvFile.seekg(0, std::ios::beg);
            //readNextFrame();
        }
        
        myshader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texY);
        myshader.setInt("texY", 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texU);
        myshader.setInt("texU", 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texV);
        myshader.setInt("texV", 2);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1, &texY);
    yuvFile.close();
    glDeleteTextures(1, &texU);
    glDeleteTextures(1, &texV);
    SDL_FreeWAV(audio_buf);
    SDL_CloseAudioDevice(device_id);
    SDL_Quit();
}




void initTextures() {
    // 创建Y纹理（亮度分量，单通道）
    glGenTextures(1, &texY);
    glBindTexture(GL_TEXTURE_2D, texY);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH, SCR_HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

    // 创建U纹理（色度分量，1/2分辨率）
    glGenTextures(1, &texU);
    glBindTexture(GL_TEXTURE_2D, texU);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH / 2, SCR_HEIGHT / 2, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

    // 创建V纹理（同上）
    glGenTextures(1, &texV);
    glBindTexture(GL_TEXTURE_2D, texV);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH / 2, SCR_HEIGHT / 2, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
}
bool initYUV(const char* filePath) {
    yuvFile.open(filePath, std::ios::binary);
    if (!yuvFile.is_open()) {
        std::cout << "Failed to open YUV file!" << std::endl;
        return false;
    }

    // 计算单帧大小（YUV420P：Y=W*H，U=V=W*H/4）
    frameSize = SCR_WIDTH * SCR_HEIGHT * 3 / 2;
    yData.resize(SCR_WIDTH * SCR_HEIGHT);
    uData.resize(SCR_WIDTH * SCR_HEIGHT / 4);
    vData.resize(SCR_WIDTH * SCR_HEIGHT / 4);
    return true;
}
bool readNextFrame() {
    if (!yuvFile.read((char*)yData.data(), yData.size())) return false;
    if (!yuvFile.read((char*)uData.data(), uData.size())) return false;
    if (!yuvFile.read((char*)vData.data(), vData.size())) return false;

    // 更新纹理数据
    glBindTexture(GL_TEXTURE_2D, texY);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RED, GL_UNSIGNED_BYTE, yData.data());

    glBindTexture(GL_TEXTURE_2D, texU);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, SCR_WIDTH / 2, SCR_HEIGHT / 2, GL_RED, GL_UNSIGNED_BYTE, uData.data());

    glBindTexture(GL_TEXTURE_2D, texV);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, SCR_WIDTH / 2, SCR_HEIGHT / 2, GL_RED, GL_UNSIGNED_BYTE, vData.data());
    return true;
}

void callback(void* userdata, Uint8* stream, int len) {
    int remain = audio_length - audio_pos;
    if (remain > len) {
        SDL_memcpy(stream, audio_buf + audio_pos, len);
        audio_pos += len;
    }
    else {
        SDL_memcpy(stream, audio_buf + audio_pos, remain);
        begin_state=0;
    }
}