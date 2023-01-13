#pragma once
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iomanip>
#include <map>
#include <filesystem>
#include <sstream>
#include <string>

#include "ModelLoad.hpp"
#include "Camera.hpp"
class Application {
    
public:
    Application(int& width, int& height, const char* windowTitle);
    ~Application();
    void appFunc();
private:
    int winWidth, winHeight;
    

    GLFWwindow* window;
    
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};
    
