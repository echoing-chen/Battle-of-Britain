#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "stb_image.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Camera.hpp"
#include "NPC.hpp"
#include "UI.hpp"

extern GLFWwindow* window;
extern int gamestate;
extern int SCR_WIDTH;
extern int SCR_HEIGHT;
extern int windowwidth;
extern int windowheight;
extern GLuint startTexture;
extern GLuint dieTexture;
extern GLuint failTexture1;
extern GLuint winTexture1;
extern GLuint failTexture2;
extern GLuint winTexture2;

void loadBackGroundTexture(const char* path,GLuint& texture) {
	int width;
	int height;
	int channels;
	unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
	if (!data) {
		return;
	}
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	GLenum format;
	if (channels == 1)
		format = GL_RED;
	else if (channels == 3)
		format = GL_RGB;
	else if (channels == 4)
		format = GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
}
void renderStartMenu() {
	float ratio = (float)(windowwidth + windowheight) / (SCR_WIDTH + SCR_HEIGHT);
	ImGui::SetNextWindowSize(ImVec2(windowwidth, windowheight));
	ImGui::SetNextWindowPos(ImVec2(0, 0));

	ImGui::Begin("StartMenu", nullptr,
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);
	ImGui::Image((ImTextureID)(intptr_t)startTexture, ImVec2(windowwidth,windowheight));
	ImGui::SetWindowFontScale(8.0f * ratio);
	ImGui::SetCursorPosX((windowwidth - ImGui::CalcTextSize("Battle of Britain").x) / 2);
	ImGui::SetCursorPosY(windowheight / 4);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.6f, 0.2f, 1.0f));
	ImGui::Text("Battle of Britain");
	ImGui::SetWindowFontScale(3.0f*ratio);
	ImGui::PopStyleColor();

	float buttonwidth = 220.0f*ratio;
	float buttonheight = 60.0f*ratio;
	ImGui::SetCursorPosX((windowwidth - buttonwidth) / 2);
	ImGui::SetCursorPosY(windowheight / 2);
	if (ImGui::Button("Start Game", ImVec2(buttonwidth, buttonheight)) ||
		ImGui::IsKeyPressed(ImGuiKey_Enter)) {
		gamestate = 1;
	}

	ImGui::SetCursorPosX((windowwidth - buttonwidth) / 2);
	ImGui::SetCursorPosY(windowheight / 2 + buttonheight + 20);
	if (ImGui::Button("Quit Game", ImVec2((buttonwidth), buttonheight))) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	ImGui::SetCursorPosX((windowwidth - ImGui::CalcTextSize("Press [Enter] to Start").x) / 2);
	ImGui::SetCursorPosY(windowheight * 0.9f);
	if ((int)(ImGui::GetTime() / 2.0f) % 2 == 0){
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.7f), "Press [Enter] to Start");
	}

	ImGui::End();
}

void destroyStartMenu() {
	glDeleteTextures(1, &startTexture);
}



void rendermystate(Camera& mycamera,NPCs& npcs) {
	float ratio = (float)(windowwidth + windowheight) / (SCR_WIDTH + SCR_HEIGHT);
	if (mycamera.HP > 0) {
		ImGui::Begin("Fuel", nullptr,
			ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);
		ImGui::SetWindowPos("Fuel", ImVec2(windowwidth-220*ratio, windowheight - 45 * ratio));
		ImGui::SetWindowFontScale(ratio);
		ImGui::Text("Fuel : %d / 100000", mycamera.fuel);
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ((float)mycamera.fuel / 100000) > 0.3f ? IM_COL32(0, 255, 0, 255) : IM_COL32(255, 0, 0, 255));
		ImGui::ProgressBar((float)mycamera.fuel / 100000, ImVec2(200 * (float)(windowwidth / SCR_WIDTH), 20 * (float)(windowheight / SCR_HEIGHT)), "");
		ImGui::PopStyleColor();
		ImGui::End();

		ImGui::Begin("Ammunition", nullptr,
			ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);
		ImGui::SetWindowPos("Ammunition", ImVec2(windowwidth - 220 * ratio, windowheight - 120 * ratio));
		ImGui::SetWindowFontScale(ratio);
		ImGui::Text("Ammunition : %d /5000", mycamera.ammunition);
		ImGui::End();

		ImGui::Begin("HP", nullptr,
			ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);
		ImGui::SetWindowPos("HP", ImVec2(windowwidth - 220 * ratio, windowheight - 80 * ratio));
		ImGui::SetWindowFontScale(ratio);
		ImGui::Text("HP : %d / 100", mycamera.HP);
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ((float)mycamera.HP / 100) > 0.3f ? IM_COL32(0, 255, 0, 255) : IM_COL32(255, 0, 0, 255));
		ImGui::ProgressBar((float)mycamera.HP / 100, ImVec2(200 * (float)(windowwidth / SCR_WIDTH), 20 * (float)(windowheight / SCR_HEIGHT)), "");
		ImGui::PopStyleColor();
		ImGui::End();

		ImGui::Begin("Map", nullptr,
			ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus);
		float mapwidth = 150 * ratio;
		float mapheight = 150 * ratio;
		ImGui::SetWindowPos("Map", ImVec2(10, windowheight - mapheight));
		ImGui::SetWindowSize("Map", ImVec2(mapwidth, mapheight));
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		draw_list->AddRectFilled(ImVec2(10, windowheight - mapheight), ImVec2(10 + mapwidth, windowheight),IM_COL32(0,150,0,100));
		draw_list->AddRect(ImVec2(10, windowheight - mapheight), ImVec2(10 + mapwidth, windowheight), IM_COL32(200,200,200,255),0.0f,0,1.0f);
		draw_list->AddCircle(ImVec2(10 + mapwidth / 2, windowheight - mapheight/2), 2.0f, IM_COL32(0, 120, 255, 255), 16);
		glm::vec2 front = glm::normalize(glm::vec2(-glm::sin(glm::radians(mycamera.Yaw)), -glm::cos(glm::radians(mycamera.Yaw))));
		glm::vec2 right = glm::normalize(glm::vec2(glm::cos(glm::radians(mycamera.Yaw)), -glm::sin(glm::radians(mycamera.Yaw))));
		for (Npc& ally : npcs.allies) {
			glm::vec2 position = glm::vec2(ally.position.x, ally.position.z)-glm::vec2(mycamera.Position.x,mycamera.Position.z);
			draw_list->AddCircleFilled(ImVec2(10 + mapwidth / 2 + glm::dot(right, position)/6.0f, windowheight - mapheight / 2 - glm::dot(front, position)/6.0f), 2.0f, IM_COL32(0, 0, 255, 255), 16);
		}
		for (Npc& bf109 : npcs.bf109s) {
			glm::vec2 position = glm::vec2(bf109.position.x, bf109.position.z) - glm::vec2(mycamera.Position.x, mycamera.Position.z);
			draw_list->AddCircleFilled(ImVec2(10 + mapwidth / 2 + glm::dot(right, position) / 6.0f, windowheight - mapheight / 2 - glm::dot(front, position) / 6.0f), 2.0f, IM_COL32(255, 0, 0, 255), 16);
		}
		for (Npc& bomber : npcs.bombers) {
			glm::vec2 position = glm::vec2(bomber.position.x, bomber.position.z) - glm::vec2(mycamera.Position.x, mycamera.Position.z);
			draw_list->AddCircleFilled(ImVec2(10 + mapwidth / 2 + glm::dot(right, position) / 6.0f, windowheight - mapheight / 2 - glm::dot(front, position) / 6.0f), 2.0f, IM_COL32(255, 0, 0, 255), 16);
		}
		glm::vec2 position = glm::vec2(0, 0) - glm::vec2(mycamera.Position.x, mycamera.Position.z);
		draw_list->AddLine(ImVec2(10 + mapwidth / 2, windowheight - mapheight / 2), ImVec2(10 + mapwidth / 2 + glm::dot(right, position) / 6.0f, windowheight - mapheight / 2 - glm::dot(front, position) / 6.0f), IM_COL32(255, 255, 0, 100), 1.0f);
		ImGui::End();

		if (mycamera.state == 0 && mycamera.Velocity == 0) {
			float buttonwidth = 220.0f * ratio;
			float buttonheight = 60.0f * ratio;
			ImGui::Begin("Recover", nullptr,
				ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);
			ImGui::SetWindowPos("Recover", ImVec2((windowwidth - buttonwidth) / 2, windowheight / 2));
			ImGui::SetWindowFontScale(ratio);
			ImGui::Text("Press [Enter] to Recover");
			if (ImGui::Button("Recover", ImVec2(buttonwidth, buttonheight)) ||
				ImGui::IsKeyPressed(ImGuiKey_Enter)) {
				mycamera.fuel = 100000;
				mycamera.ammunition = 5000;
				mycamera.HP = 100;
			}
			ImGui::End();
		}
	}


	if (mycamera.HP<=0) {
		float buttonwidth = 220.0f * ratio;
		float buttonheight = 60.0f * ratio;
		ImGui::SetNextWindowSize(ImVec2(windowwidth, windowheight));
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::Begin("die", nullptr,
			ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);
		ImGui::Image((ImTextureID)(intptr_t)dieTexture, ImVec2(windowwidth, windowheight));
		ImGui::SetWindowFontScale(5.0f * ratio);
		ImGui::SetCursorPosX((windowwidth - ImGui::CalcTextSize("You died").x) / 2);
		ImGui::SetCursorPosY(windowheight / 4);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.6f, 0.2f, 1.0f));
		ImGui::Text("You died");
		ImGui::SetWindowFontScale(2.0f*ratio);
		ImGui::PopStyleColor();

		ImGui::SetCursorPosX((windowwidth - buttonwidth) / 2);
		ImGui::SetCursorPosY(windowheight / 2);
		if (ImGui::Button("Revive", ImVec2(buttonwidth, buttonheight))) {
			mycamera.state = -2;
		}
		ImGui::End();
	}
	ImGui::Begin("situation", nullptr,
		ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBringToFrontOnFocus| ImGuiWindowFlags_NoBackground);
	ImGui::SetWindowFontScale(2*ratio);
	ImGui::SetWindowPos("situation", ImVec2((windowwidth - ImGui::CalcTextSize("Ally 10 / 10 Enemy").x)/2, 40 * ratio));
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 255, 255));
	ImGui::Text("Ally %d / ", npcs.allies.size());
	ImGui::PopStyleColor();
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255, 0, 0, 255));
	ImGui::Text("%d Enemy", npcs.bf109s.size() + npcs.bombers.size());
	ImGui::PopStyleColor();
	ImGui::End();

}

void renderevent1(int& state) {
	float ratio = (float)(windowwidth + windowheight) / (SCR_WIDTH + SCR_HEIGHT);
	float buttonwidth = 220.0f * ratio;
	float buttonheight = 60.0f * ratio;
	if (state == -3) {
		ImGui::SetNextWindowSize(ImVec2(windowwidth, windowheight));
		ImGui::SetNextWindowPos(ImVec2(0, 0));

		ImGui::Begin("fail", nullptr,
			ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);
		ImGui::Image((ImTextureID)(intptr_t)failTexture1, ImVec2(windowwidth, windowheight));
		ImGui::SetWindowFontScale(5.0f * ratio);
		ImGui::SetCursorPosX((windowwidth - ImGui::CalcTextSize("We failed").x) / 2);
		ImGui::SetCursorPosY(windowheight / 4);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.6f, 0.2f, 1.0f));
		ImGui::Text("We failed");
		ImGui::SetWindowFontScale(2.0f * ratio);
		ImGui::PopStyleColor();

		ImGui::SetCursorPosX((windowwidth - buttonwidth) / 2);
		ImGui::SetCursorPosY(windowheight / 2);
		if (ImGui::Button("Retry", ImVec2(buttonwidth, buttonheight))) {
			state = -2;
			gamestate = 2;
		}
		ImGui::End();
	}

	if (state == 2) {
		ImGui::SetNextWindowSize(ImVec2(windowwidth, windowheight));
		ImGui::SetNextWindowPos(ImVec2(0, 0));

		ImGui::Begin("win", nullptr,
			ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);
		ImGui::Image((ImTextureID)(intptr_t)winTexture1, ImVec2(windowwidth, windowheight));
		ImGui::SetWindowFontScale(5.0f * ratio);
		ImGui::SetCursorPosX((windowwidth - ImGui::CalcTextSize("We made it!").x) / 2);
		ImGui::SetCursorPosY(windowheight / 4);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.6f, 0.2f, 1.0f));
		ImGui::Text("We made it!");
		ImGui::SetWindowFontScale(2.0f * ratio);
		ImGui::PopStyleColor();

		ImGui::SetCursorPosX((windowwidth - buttonwidth) / 2);
		ImGui::SetCursorPosY(windowheight / 2);
		if (ImGui::Button("Go On", ImVec2(buttonwidth, buttonheight))) {
			state = -2;
			gamestate = 3;
		}
		ImGui::End();
	}
}

void renderevent2(int& state) {
	float ratio = (float)(windowwidth + windowheight) / (SCR_WIDTH + SCR_HEIGHT);
	float buttonwidth = 220.0f * ratio;
	float buttonheight = 60.0f * ratio;
	if (state == -3) {
		ImGui::SetNextWindowSize(ImVec2(windowwidth, windowheight));
		ImGui::SetNextWindowPos(ImVec2(0, 0));

		ImGui::Begin("fail", nullptr,
			ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);
		ImGui::Image((ImTextureID)(intptr_t)failTexture2, ImVec2(windowwidth, windowheight));
		ImGui::SetWindowFontScale(5.0f * ratio);
		ImGui::SetCursorPosX((windowwidth - ImGui::CalcTextSize("We failed").x) / 2);
		ImGui::SetCursorPosY(windowheight / 4);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.6f, 0.2f, 1.0f));
		ImGui::Text("We failed");
		ImGui::SetWindowFontScale(2.0f * ratio);
		ImGui::PopStyleColor();
		ImGui::SetCursorPosX((windowwidth - buttonwidth) / 2);
		ImGui::SetCursorPosY(windowheight / 2);
		if (ImGui::Button("Retry", ImVec2(buttonwidth, buttonheight))) {
			state = -2;
			gamestate = 2;
		}
		ImGui::End();
	}

	if (state == 2) {
		ImGui::SetNextWindowSize(ImVec2(windowwidth, windowheight));
		ImGui::SetNextWindowPos(ImVec2(0, 0));

		ImGui::Begin("win", nullptr,
			ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);
		ImGui::Image((ImTextureID)(intptr_t)winTexture2, ImVec2(windowwidth, windowheight));
		ImGui::SetWindowFontScale(5.0f * ratio);
		ImGui::SetCursorPosX((windowwidth - ImGui::CalcTextSize("We made it!").x) / 2);
		ImGui::SetCursorPosY(windowheight / 4);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.6f, 0.2f, 1.0f));
		ImGui::Text("We made it!");
		ImGui::SetWindowFontScale(2.0f * ratio);
		ImGui::PopStyleColor();
		ImGui::SetCursorPosX((windowwidth - buttonwidth) / 2);
		ImGui::SetCursorPosY(windowheight / 2);
		if (ImGui::Button("Game Over", ImVec2(buttonwidth, buttonheight))) {
			state = -2;
			gamestate = 4;
		}
		ImGui::End();
	}
}