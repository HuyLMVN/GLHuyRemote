#include "Application.hpp"

int main() {
    int width = 1280;
    int height = 1024;

    Application mainApp(width, height, "Model Viewer");
    mainApp.appFunc(); 
	return 0;
}
