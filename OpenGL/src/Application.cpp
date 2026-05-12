#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "ShaderProgram.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Texture.h"

int main(void)
{
    GLFWwindow* window;
    Renderer renderer;


    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(10);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    {


        float positions[] = {
           -0.5f,  -0.5f, 0.0f, 0.0f,
            0.5f,  -0.5f, 1.0f, 0.0f,
            0.5f,   0.5f, 1.0f, 1.0f,
           -0.5f,   0.5f, 0.0f, 1.0f
        };

        unsigned int indicies[] = {
            0, 1, 2,
            2, 3, 0
        };

        VertexBuffer vbo(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);

        VertexArray va;
        va.RecordVBOLayout(vbo, layout);

        IndexBuffer ibo(indicies, 2 * 3);
        va.RecordIndexBuffer(ibo);

        ShaderProgram shaderProgram("res/shaders/Basic.shader");
        shaderProgram.Bind();

        Texture texture("res/textures/cover.png");
        texture.Bind();

        while (!glfwWindowShouldClose(window))
        {
        
            renderer.Clear();
            shaderProgram.SetUniform1i("u_Texture", 0);

            renderer.Draw(va, shaderProgram);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

    }

    glfwTerminate();
    return 0;
}