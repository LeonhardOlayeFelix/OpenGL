#include <GL/glew.h>

#include "imgui.h"
#include "ClearColourScene.h"
#include "ErrorHandling.h"


namespace scene {

    ClearColourScene::ClearColourScene() : m_ClearColor{0.0f, 0.0f, 0.0f, 1.0f}
    {

    }
    ClearColourScene::~ClearColourScene() {}

    void ClearColourScene::OnUpdate(float deltaTime, GLFWwindow* window) {}
    void ClearColourScene::OnRender()
    {
        GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }    

    void ClearColourScene::OnImGuiRender()
    {
        ImGui::ColorEdit4("Clear color", m_ClearColor);
    }           

}

