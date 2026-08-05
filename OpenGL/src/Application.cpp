#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"
#include "Application.h"

#include "scene/SceneIncludes.h"

void main(void)
{
    GLFWwindow* window = init();

    scene::Scene* currentScene = nullptr;

    scene::MenuScene* MenuScene = new scene::MenuScene(currentScene);
    currentScene = MenuScene;

    MenuScene->RegisterScene<scene::TriangleScene>("Triangle");
    MenuScene->RegisterScene<scene::Texture2DScene>("Textures");
    MenuScene->RegisterScene<scene::TransformationsScene>("Transformations");
    MenuScene->RegisterScene<scene::Transformations3DScene>("Transformations in 3D");
    MenuScene->RegisterScene<scene::Camera3DScene>("Camera in 3D");
    MenuScene->RegisterScene<scene::LightingScene>("Lighting");
    MenuScene->RegisterScene<scene::MeshLoadingScene>("Assimp");
    MenuScene->RegisterScene<scene::InstancingScene>("Instancing");
    MenuScene->RegisterScene<scene::AsteroidScene>("Asteroid");
	MenuScene->RegisterScene<scene::DepthTestingScene>("Depth Testing");
    MenuScene->RegisterScene<scene::BlendingScene>("Blending");
    MenuScene->RegisterScene<scene::FaceCullingScene>("Face Culling");
    MenuScene->RegisterScene<scene::FramebufferScene>("Frame Buffer");
    MenuScene->RegisterScene<scene::UBOScene>("UBO Scene");
    MenuScene->RegisterScene<scene::GeometryShaderScene>("Geometry Shader");
    MenuScene->RegisterScene<scene::CubeMapScene>("Cube Map");

    MenuScene->SetScene("Cube Map");
    
    Renderer renderer;

    double lastFrame = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        renderer.Clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (currentScene) {

            double currentFrame = glfwGetTime();
            currentScene->OnUpdate(currentFrame - lastFrame, window);
            lastFrame = currentFrame;

            currentScene->OnRender();

            ImGui::Begin("Application");
            if (currentScene != MenuScene) {

                if (ImGui::Button("Back to Scene Menu")) {
                    delete currentScene;
                    currentScene = MenuScene;
                }

            }
            currentScene->OnImGuiRender();
            ImGui::End();
        }

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

GLFWwindow* init() {
    GLFWwindow* window;
    if (!glfwInit())
        return 0;

    glfwWindowHint(GLFW_SAMPLES, 4);
    window = glfwCreateWindow(960, 540, "LearnOpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(window);


    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    glEnable(GL_MULTISAMPLE);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;    
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    glViewport(0, 0, 960, 540);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return window;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
