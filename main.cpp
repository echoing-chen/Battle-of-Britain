#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "shader.hpp"
#include "model.hpp"
#include "begin.hpp"
#include "bgm.hpp"
#include "camera.hpp"
#include "NPC.hpp"
#include "bullet.hpp"
#include "scene.hpp"
#include "sky.hpp"
#include "UI.hpp"
#include "cockpit.hpp"
#include <iostream>
#include <fstream>
#include <vector>


#undef main


int SCR_WIDTH = 1280;
int SCR_HEIGHT = 720;
GLFWwindow* window;
float currentframe=0.0f;
float lastframe = 0.0f;
float deltatime=0.0f;
glm::vec3 initialDir(0, 0, -1);
glm::vec3 worldUp(0, 1, 0);
int gamestate=0;//0 开始，1 stage1开始，2 stagefail重开，3 stage2开始
int windowwidth = SCR_WIDTH;
int windowheight = SCR_HEIGHT;
GLuint startTexture;
GLuint dieTexture;
GLuint failTexture1;
GLuint winTexture1;
GLuint failTexture2;
GLuint winTexture2;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Battle of Britain", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "GLEW初始化失败: %s\n", glewGetErrorString(err));
        return -1;
    }


    beginvideo();
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    loadBackGroundTexture("picture/start.jpeg",startTexture);
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    while (gamestate == 0) {
        if ((glfwWindowShouldClose(window))||glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            return 0;
        }
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        renderStartMenu();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    destroyStartMenu();
    Shader modelshader("modelvertexshader.glsl", "modelfragmentshader.glsl");
    Shader npcsshader("npcsvertexshader.glsl", "npcsfragmentshader.glsl");
    Shader particleshader("particlevertexshader.glsl", "particlefragmentshader.glsl");
    Shader bulletshader("bulletvertexshader.glsl", "bulletfragmentshader.glsl");
    Shader skyshader("skyvertexshader.glsl", "skyfragmentshader.glsl");
    Shader cockpitshader("cockpitvertexshader.glsl", "cockpitfragmentshader.glsl");
    Model ally_model("model/spitfire/spitfire.obj");
    Model bf109_model("model/germany/germany.obj");
    Model bomber_model("model/bomber/HZJ_M.FBX");
    Model scene("model/scene/scene.obj");
    NPCs npcs;
    Bullets bullets;
    Skies skies;
    Camera mycamera;


    GLuint UBO;
    glGenBuffers(1, &UBO);
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, 2 * sizeof(glm::mat4));
    GLuint modelshader_matrices_index = glGetUniformBlockIndex(modelshader.ID, "matrices");
    GLuint particleshader_matrices_index = glGetUniformBlockIndex(particleshader.ID, "matrices");
    GLuint bulletshader_matrices_index = glGetUniformBlockIndex(bulletshader.ID, "matrices");
    GLuint skyshader_matrices_index = glGetUniformBlockIndex(skyshader.ID, "matrices");
    glUniformBlockBinding(modelshader.ID, modelshader_matrices_index, 0);
    glUniformBlockBinding(particleshader.ID, particleshader_matrices_index, 0);
    glUniformBlockBinding(bulletshader.ID, bulletshader_matrices_index, 0);
    glUniformBlockBinding(skyshader.ID, skyshader_matrices_index, 0);
    loadBackGroundTexture("picture/die.jpg", dieTexture);
    loadBackGroundTexture("picture/fail1.jpg", failTexture1);
    loadBackGroundTexture("picture/win1.jpg", winTexture1);
    loadBackGroundTexture("picture/fail2.jpg", failTexture2);
    loadBackGroundTexture("picture/win2.jpg", winTexture2);
    stbi_set_flip_vertically_on_load(true);
    glfwSetCursorPos(window, windowwidth / 2.0f, windowheight / 2.0f);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    start_bgm();
    npcs.initnpcs(ally_model, bf109_model, bomber_model);
    npcs.stage1();
    skies.initsky();
    initcockpit();
    lastframe = static_cast<float>(glfwGetTime());
    while (gamestate==1||gamestate==2) {
        if (glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            return 0;
        }
        if (gamestate == 2) {
            npcs.stage1();
            gamestate = 1;
        }
        currentframe = static_cast<float>(glfwGetTime());
        deltatime = currentframe - lastframe;
        lastframe = currentframe;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindBuffer(GL_UNIFORM_BUFFER, UBO);
        glm::mat4 view = mycamera.GetViewMatrix();
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
        glm::mat4 projection = mycamera.GetProjectionMatrix();
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        renderscene(scene,modelshader);
        bullets.managebullet(mycamera, npcs);
        bullets.renderbullets(bulletshader);
        mycamera.ProcessKeyBoard(bullets, mycamera);
        mycamera.ProcessMouse();
        mycamera.UpdateCamera();
        mycamera.UpdateCameraVectors();
        npcs.stage1AI(bullets,mycamera);
        npcs.updatestage1npc(mycamera);
        npcs.renderstage1npc(npcsshader, ally_model, bf109_model);
        npcs.updateParticles();
        npcs.renderParticles(particleshader);
        skies.rendersky1(skyshader, mycamera);
        rendercockpit(cockpitshader);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        rendermystate(mycamera,npcs);
        renderevent1(mycamera.state);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    bf109_model.clear();
    npcs.stage2();
    while (gamestate == 3 || gamestate == 2) {
        if (glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            return 0;
        }
        if (gamestate == 2) {
            npcs.stage2();
            gamestate = 3;
        }
        currentframe = static_cast<float>(glfwGetTime());
        deltatime = currentframe - lastframe;
        lastframe = currentframe;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindBuffer(GL_UNIFORM_BUFFER, UBO);
        glm::mat4 view = mycamera.GetViewMatrix();
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
        glm::mat4 projection = mycamera.GetProjectionMatrix();
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        renderscene(scene, modelshader);
        bullets.managebullet(mycamera, npcs);
        bullets.renderbullets(bulletshader);
        mycamera.ProcessKeyBoard(bullets, mycamera);
        mycamera.ProcessMouse();
        mycamera.UpdateCamera();
        mycamera.UpdateCameraVectors();
        npcs.stage2AI(bullets, mycamera);
        npcs.updatestage2npc(mycamera);
        npcs.renderstage2npc(npcsshader, ally_model, bomber_model);
        npcs.updateParticles();
        npcs.renderParticles(particleshader);
        skies.rendersky2(skyshader, mycamera);
        rendercockpit(cockpitshader);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        rendermystate(mycamera,npcs);
        renderevent2(mycamera.state);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    windowwidth = width;
    windowheight = height;
}