#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

#include "Application.h"
#include "Renderer.h"
#include "scene/MenuScene.h"
#include "scene/ClearColourScene.h"
#include "scene/Texture2DScene.h"


int main(void)
{
    GLFWwindow* window = init();

    Renderer renderer;

    scene::Scene* currentScene = nullptr;

    scene::MenuScene* MenuScene = new scene::MenuScene(currentScene);
    currentScene = MenuScene;

    MenuScene->RegisterScene<scene::ClearColourScene>("Colour");

    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (currentScene) {
            currentScene->OnUpdate(0.0f);
            currentScene->OnRender();

            ImGui::Begin("Application");
            if (currentScene != MenuScene && ImGui::Button("Back to Scene Menu")) {

                delete currentScene;
                currentScene = MenuScene;

            }
            currentScene->OnImGuiRender();
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    delete currentScene;
    if (currentScene != MenuScene) delete MenuScene;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

GLFWwindow* init() {
    GLFWwindow* window;
    if (!glfwInit())
        return 0;

    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(10);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    return window;
}
