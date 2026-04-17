#include <iostream>
#include <GLFW/glfw3.h>

#include "external/imgui/imgui-master/imgui.h"
#include "external/imgui/imgui-master/backends/imgui_impl_glfw.h"
#include "external/imgui/imgui-master/backends/imgui_impl_opengl3.h"

#include "include/Component.h"
#include "include/Manager.h"
#include "include/VFSInterface.h"

int main()
{
    if(!glfwInit())
    {
        std::cerr << "Failed to init GLFW\n";
        return 1;
    }

    const char* glslVersion = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "VFS Simulator", nullptr, nullptr);
    if(window == nullptr)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return 1;
    }

    //fereastra ramane la dimensiune fixa
    glfwSetWindowSizeLimits(window, 1280, 720, 1280, 720);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsLight();

    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    //tema deschisa cu accente portocalii
    colors[ImGuiCol_WindowBg] = ImVec4(0.98f, 0.98f, 0.98f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.92f, 0.92f, 0.92f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(1.00f, 0.56f, 0.16f, 0.70f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(1.00f, 0.50f, 0.08f, 1.00f);

    colors[ImGuiCol_Button] = ImVec4(1.00f, 0.55f, 0.14f, 0.90f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(1.00f, 0.64f, 0.24f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.95f, 0.45f, 0.06f, 1.00f);

    colors[ImGuiCol_Header] = ImVec4(1.00f, 0.60f, 0.20f, 0.45f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(1.00f, 0.66f, 0.28f, 0.70f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.95f, 0.48f, 0.10f, 0.88f);

    colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.42f, 0.04f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 0.56f, 0.16f, 0.95f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.94f, 0.44f, 0.06f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.65f, 0.65f, 0.65f, 0.65f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(1.00f, 0.58f, 0.16f, 0.90f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.94f, 0.46f, 0.08f, 1.00f);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);

    VFSInterface vfsUi;

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        vfsUi.render();

        ImGui::Render();
        int displayW = 0;
        int displayH = 0;
        glfwGetFramebufferSize(window, &displayW, &displayH);
        glViewport(0, 0, displayW, displayH);
        glClearColor(0.96f, 0.96f, 0.96f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    Manager::destroyInstance();

    std::cout << "\nFinal Instance Count: " << Component::getInstanceCount() << '\n';
    if(Component::getInstanceCount() == 0)
    {
        std::cout << "Memory clean: Success\n";
    }
    else
    {
        std::cout << "Memory clean: Failed\n";
    }

    return 0;
}
