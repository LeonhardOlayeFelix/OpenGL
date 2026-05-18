#include <GL/glew.h>

#include "imgui.h"
#include "Texture2DScene.h"
#include "ErrorHandling.h"
#include "Common.h"

namespace scene {

    Texture2DScene::Texture2DScene() 
    { 
    
    }

    Texture2DScene::~Texture2DScene()
    {
    
    }

    void Texture2DScene::OnUpdate(float deltaTime)
    {
    
    }

    void Texture2DScene::OnRender()
    {
        GLCall(glClearColor(0.12f, 0.12f, 0.12f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));





    }    

    void Texture2DScene::OnImGuiRender()
    {

    }           

}

