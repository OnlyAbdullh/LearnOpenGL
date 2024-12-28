#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <string>
  
#include "IndexBuffer.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
 

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Remove GLCall for functions returning void
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(800, 800, "BlueExam", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSwapInterval(1);
    glfwMakeContextCurrent(window);
   
    if (!gladLoadGL()) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 800);

    {
        float positions[] = {
            -0.5f, -0.5f,
             0.5f, -0.5f,
             0.5f,  0.5f,
            -0.5f,  0.5f,
        };
        unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

        unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        VertexArray va;
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb,layout);

        IndexBuffer ib(indices, 6);

        Shader shader("basic.shader");
        shader.Bind();
        shader.SetUniform4("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        float r = 0.0f;
        float increament = 0.05f;

        while (!glfwWindowShouldClose(window)) {
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            shader.Bind();
            shader.SetUniform4("u_Color", r, 0.3f, 0.8f, 1.0f); 
            va.Bind();
            vb.Bind();
            ib.Bind();
           
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            if (r > 1.0f)
                increament = -0.05f;
            else if (r < 0.0f)
                increament = 0.05f;

            r += increament;
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
         
    }
    GLCall(glfwTerminate());

    return 0;
}
