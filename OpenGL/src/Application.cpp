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
    GLFWwindow* window;
    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(10);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    Renderer renderer;

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    {
        scene::Scene* currentScene = nullptr;

        scene::MenuScene* sceneMenu = new scene::MenuScene(currentScene);
        currentScene = sceneMenu;

        sceneMenu->RegisterScene<scene::ClearColourScene>("Colour");
        sceneMenu->RegisterScene<scene::Texture2DScene>("TEXTURES");

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
                if (currentScene != sceneMenu ) {

                    if (ImGui::Button("Back to Scene Menu")) {
                        delete currentScene;
                        currentScene = sceneMenu;
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
        delete currentScene;
        if (currentScene != sceneMenu) delete sceneMenu;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}