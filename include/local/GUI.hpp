#pragma once
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

class GUI {
public:
    GUI(float&& winWidth, float&& winHeight);
    void setWinPos(float x, float y);
    void addCustomFont(const char* fontPath, int fontSize);
    void newFrame();
    void windowText(const char* windowName, const char* windowText);
    void windowSliderF(const char* sliderLabel, float minVal, float maxVal, float* defVal);
    void renderWindow();
    ImGuiIO& io;
private:
    ImFont* textFont{nullptr};
    ImVec2 GuiWindowSize;
};
