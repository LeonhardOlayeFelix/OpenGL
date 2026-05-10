#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <malloc.h>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "ShaderProgram.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

int main(void)
{
    GLFWwindow* window;

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
           -0.5f,  -0.5f,
            0.5f,  -0.5f,
            0.5f,   0.5f,
           -0.5f,   0.5f,
        };

        unsigned int indicies[] = {
            0, 1, 2,
            2, 3, 0
        };

        VertexBuffer vbo(positions, 4 * 2 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);

        IndexBuffer ibo(indicies, 2 * 3);

        VertexArray va;
        va.RecordVBOLayout(vbo, layout);

        ShaderProgram shaderProgram("res/shaders/Basic.shader");
        shaderProgram.Bind();
        shaderProgram.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

        float r = 0.0f;
        float increment = 0.05f;
        while (!glfwWindowShouldClose(window))
        {
        
            glClear(GL_COLOR_BUFFER_BIT);


            va.Bind();
            va.RecordIndexBuffer(ibo);
            shaderProgram.Bind();
            shaderProgram.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;

            r += increment;


            glfwSwapBuffers(window);
            glfwPollEvents();
        }

    }

    glfwTerminate();
    return 0;
}