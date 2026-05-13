#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "ShaderProgram.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Application.h"

int main(void)
{
    GLFWwindow* window;


    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        Renderer renderer;

        float positions[] = {
            100.0f,  100.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            200.0f,  100.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            200.0f,   200.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            100.0f,   200.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        };

        unsigned int indicies[] = {
            0, 1, 2,
            2, 3, 0
        };

        VertexBuffer vbo(positions, 4 * 8 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        layout.Push<float>(4);
        VertexArray va;
        va.RecordVBOLayout(vbo, layout);

        IndexBuffer ibo(indicies, 2 * 3);
        va.RecordIndexBuffer(ibo);

        ShaderProgram shaderProgram("res/shaders/Basic.shader");
        shaderProgram.Bind();

        Texture texture("res/textures/cover.png");
        texture.Bind();

        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f); //mapseach vertex from pixel space to NDC space so it can be shown on screen
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0)); //Transforms each vertex in the world by this matrix to simulate camera/view
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0)); //Transforms each vertex to its position in the world

        while (!glfwWindowShouldClose(window))
        {
            view = glm::translate(view, glm::vec3(10, 0, 0));
            glm::mat4 mvp = proj * view * model;

            renderer.Clear();
            shaderProgram.SetUniform1i("u_Texture", 0);
            shaderProgram.SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(va, shaderProgram);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

    }

    glfwTerminate();
    return 0;
}