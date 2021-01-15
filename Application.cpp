#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Assert.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"




int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Demo Renderer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "Error" << std::endl;
    
    float positions[] =
    {
        -0.5f, -0.5f, 0.0f, 0.0f,//0
        0.5f, -0.5f, 1.0f, 0.0f,//1
        0.5f, 0.5f, 1.0f, 1.0f,//2
        -0.5f, 0.5f, 0.0f, 1.0f,//3
    };

    unsigned int indecies[]=
    {
        0, 1, 2,
        2, 3, 0
    };


    //Vertex Buffer
    VertexBuffer vb = VertexBuffer(positions, 4 * 4 * sizeof(float) );

    //SetBlendFunc (to be moved to shader)
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    //Merge VB & Layout into Vertex Array
	VertexArray va;
	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    //IndexBuffer
    IndexBuffer ib = IndexBuffer(indecies, 6);

    //Shader
    Shader shader = Shader("resources/shaders/basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 1.0, 0.0, 0.0, 1.0);
    
	//ortho
    glm::mat4 model = glm::translate((glm::mat4(1.0f)), glm::vec3(0.0, 0.0, 0.0));
    glm::mat4 view = glm::translate(( glm::mat4(1.0f) ), glm::vec3(0.0, 0.0, 0.0));
    glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

    glm::mat4 mvp_xform = model * view * proj;
	shader.SetUniforMat4f("u_MVP", mvp_xform);

    Texture texture("resources/textures/basic_texture.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);



    //Unbinding
    va.UnBind();
    shader.UnBind();
    vb.Unbind();
    ib.Unbind();

    Renderer renderer;

    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();


    while (!glfwWindowShouldClose(window))
    {
        ImGui_ImplGlfwGL3_NewFrame();

        renderer.Clear();
        renderer.Draw(va, ib, shader);


		{
			static float f = 0.0f;
			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}