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

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    int width = 1280;
    int height = 1024;

    glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(width, height, "Model Viewer", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1);	
	gladLoadGL(glfwGetProcAddress);
	glViewport(0, 0, width, height);
    
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
    // std::map of loadable models' path 
    std::map<std::string, std::string>modelPath {
        {"teapot", "./res/Models/teapot/scene.gltf"},
        {"bwteapot", "./res/Models/teapot2/scene.gltf"},
        {"t-10gltf", "./res/Models/t-10m/t_10M.gltf"},
        {"cargo_ship", "./res/Models/cargo_ship/scene.gltf"},
        // Backpack model requires stbi_set_flip_vertically_on_load(false) in texture loading code;
        {"backpack", "res/Models/backpack/backpack.obj"}
    };

    // Creates camera object:
	Camera camera(&width, &height, glm::vec3(0.0f, 0.0f, 10.0f));
    shaderProgram.Activate();
    Model model1 = Model(modelPath["t-10gltf"]);
    model1.setPos(shaderProgram, glm::vec3(0.0f, -2.0f, 0.0f));

    std::filesystem::path modelDir = model1.directory; 


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImFont* textFont = io.Fonts->AddFontFromFileTTF("res/GuiFont/RobotoMono.ttf", 20);
    ImVec2 GuiWindowSize = ImVec2(425.0f, 145.0f);
    //static_cast<void>(io);
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    double prevTime = 0.0;
    double crntTime = 0.0;

    double timeDiff;
    float scale{1.0};
    unsigned int counter = 0;
    std::string newTitle;

	while(!glfwWindowShouldClose(window)) {

        glm::mat4 sclMat = glm::mat4(1.0f);
        crntTime = glfwGetTime();
        timeDiff = crntTime - prevTime;
        counter++;

        if(timeDiff >= 1.0 / 2.0) {
            std::stringstream FPS;
            FPS << std::fixed << std::setprecision(2) << (1.0 / timeDiff) * counter;
            std::stringstream ms;
            ms << std::fixed << std::setprecision(2) << (timeDiff / counter) * 1000;
            newTitle = ("Model Viewer - " + FPS.str() + "FPS / " + ms.str() + "ms");
            glfwSetWindowTitle(window, newTitle.c_str());
            prevTime = crntTime;
            counter = 0;
        }
        glfwGetFramebufferSize(window, &width, &height);
		glClearColor(0.0f, 0.75f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (!io.WantCaptureMouse) {
            camera.Inputs(window);
            camera.updateMatrix(60.0f, 0.1f, 1000.0f);
        }
        
        shaderProgram.Activate();
        model1.Draw(shaderProgram, camera);
        
        std::stringstream outCoords;
        outCoords << std::fixed << std::setprecision(2) << "Coordinates: " << camera.Position.x << ", " << camera.Position.y << ", " << camera.Position.z; 
        // Set ImGui position, size and constraints
        ImGui::SetNextWindowPos(ImVec2{0.0f, 1.0f});
        ImGui::SetNextWindowSize(GuiWindowSize);
        ImGui::SetNextWindowSizeConstraints(GuiWindowSize, GuiWindowSize);
        // Push custom font
        ImGui::PushFont(textFont);
        // Start of window
        ImGui::Begin(("Model: " + modelDir.filename().string()).c_str());
        ImGui::Text("%s", outCoords.str().c_str()); 
        ImGui::SliderFloat("Model Scale", &scale, 0.5f, 2.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
        // End of window
        ImGui::End();
        // Pop current font and render window
        ImGui::PopFont();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Set model scaling
        sclMat = glm::scale(glm::vec3(scale, scale, scale));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "scale"), 1, GL_FALSE, glm::value_ptr(sclMat));
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


