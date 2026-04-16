#ifndef UI_HPP
#define UI_HPP

class NPCs;
class Camera;
void loadBackGroundTexture(const char* path,GLuint& texture);
void renderStartMenu();
void destroyStartMenu();
void rendermystate(Camera& mycamera,NPCs& npcs);
void renderevent1(int& state);
void renderevent2(int& state);

#endif