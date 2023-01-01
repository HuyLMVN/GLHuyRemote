#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iomanip>
#include <map>
#include <sstream>
#include <string>

#include "ModelLoad.hpp"
#include "Camera.hpp"

static const int width = 1280;
static const int height = 1024;
int currentWidth = width; 
int currentHeight = height;

void framebuffer_size_callback(GLFWwindow* window, int p_width, int p_height)
{
    glViewport(0, 0, p_width, p_height);
}

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(width, height, "Model Viewer", NULL, NULL);
	glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1);	
	gladLoadGL(glfwGetProcAddress);
	
	glViewport(0, 0, currentWidth, currentHeight);
    

    // Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("./res/Shaders/default.vert", "./res/Shaders/default.frag");
    

	// Take care of all the light related things
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.0f, 10.0f, 0.0f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


	// Enables the Depth Buffer
    glEnable(GL_DEPTH_TEST);
    
    std::map<std::string, std::string>modelPath {
        {"teapot", "./res/Models/teapot/scene.gltf"},
        {"bwteapot", "./res/Models/teapot2/scene.gltf"},
        {"t-10gltf", "./res/Models/t-10m/t_10M.gltf"},
        {"backpack", "res/Models/backpack/backpack.obj"}
    };

    // Creates camera object:
	Camera camera(&currentWidth, &currentHeight, glm::vec3(0.0f, 0.0f, 10.0f));
    shaderProgram.Activate();
    Model tank = Model(modelPath["t-10gltf"]);
    tank.setPos(shaderProgram, glm::vec3(0.0f, -2.0f, 0.0f));

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImFont* textFont = io.Fonts->AddFontFromFileTTF("res/GuiFont/RobotoMono.ttf", 20);
    //static_cast<void>(io);
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    double prevTime = 0.0;
    double crntTime = 0.0;

    double timeDiff;
    unsigned int counter = 0;
    std::string newTitle;

	while(!glfwWindowShouldClose(window)) {
        crntTime = glfwGetTime();
        timeDiff = crntTime - prevTime;
        counter++;
        if(timeDiff >= 1.0 / 5.0) {
            std::stringstream FPS;
            FPS << std::setprecision(4) << (1.0 / timeDiff) * counter;
            std::stringstream ms;
            ms << std::setprecision(4) << (timeDiff / counter) * 1000;
            newTitle = (FPS.str() + "FPS / " + ms.str() + "ms");
            prevTime = crntTime;
            counter = 0;
        }

        glfwGetFramebufferSize(window, &currentWidth, &currentHeight);
		glClearColor(0.0f, 0.75f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        shaderProgram.Activate();
        camera.Inputs(window);
        camera.updateMatrix(60.0f, 0.1f, 1000.0f);
        tank.Draw(shaderProgram, camera);
        ImGui::SetNextWindowPos({0.0f, 1.0f});
        ImGui::SetNextWindowSize({425.0f, 120.0f});
        ImGui::PushFont(textFont);
        ImGui::Begin(("Model: T-10M - " + newTitle).c_str());
        ImGui::Text("%s", ("x: " + std::to_string(camera.Position.x)).c_str());
        ImGui::Text("%s", ("y: " + std::to_string(camera.Position.y)).c_str());
        ImGui::Text("%s", ("z: " + std::to_string(camera.Position.z)).c_str());
        ImGui::End();
        ImGui::PopFont();
        ImGui::Render();
        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();
	} 
    

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	gladLoaderUnloadGL();
	return 0;
}
