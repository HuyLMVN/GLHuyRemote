#include "GUI.hpp"
#include "imgui.h"

GUI::GUI(float&& winWidth, float&& winHeight): io{ImGui::GetIO()} {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

}
