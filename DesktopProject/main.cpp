#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Desktop.h"
#include "Taskbar.h"
#include "Applications.h"

#include <cstdio>

int main(int argc, char** argv) {
    const AppConfig config = LoadConfig(argc > 1 ? argv[1] : "config.ini");
    AppState appState;
    appState.processes = config.processes;

    // Initialize GLFW and OpenGL.
    glfwSetErrorCallback([](int, const char* message) {
        std::fprintf(stderr, "GLFW: %s\n", message);
    });
    if (!glfwInit()) return 1;
#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    const char* glslVersion = "#version 150";
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    const char* glslVersion = "#version 130";
#endif
    GLFWwindow* window = glfwCreateWindow(config.windowWidth, config.windowHeight,
                                         "Desktop Mock-up", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return 1;
    }
    glfwSetWindowSizeLimits(window, 640, 480, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Initialize Dear ImGui.
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;

    // Load the default ImGui font.
    io.Fonts->AddFontDefault();

    // Load Font Awesome icons and merge them into the default font.
    ImFontConfig iconConfig;
    iconConfig.MergeMode = true;
    iconConfig.PixelSnapH = true;

    static const ImWchar iconRanges[] = {
        0xf011, 0xf011, // Power icon
        0xf001, 0xf001, // Music icon
        0xf080, 0xf080, // Chart bar icon
        0xf108, 0xf108, // Desktop icon
        0xf1ec, 0xf1ec, // Calculator icon
        0
    };

    ImFont* iconFont = io.Fonts->AddFontFromFileTTF(
        "fonts/Font Awesome 7 Free-Solid-900.otf",
        18.0f,
        &iconConfig,
        iconRanges
    );

    if (iconFont == nullptr) {
        std::fprintf(
            stderr,
            "ERROR: Could not load Font Awesome font. "
            "Check the fonts folder and filename.\n"
        );
    }

    ImGui::StyleColorsDark();

    const bool glfwReady = ImGui_ImplGlfw_InitForOpenGL(window, true);
    const bool glReady = glfwReady && ImGui_ImplOpenGL3_Init(glslVersion);

    if (!glReady) {
        if (glfwReady) ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    while (!appState.shutdownRequested && !glfwWindowShouldClose(window)) {
        glfwPollEvents();
        if (glfwWindowShouldClose(window)) break;
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED)) {
            glfwWaitEventsTimeout(0.1);
            continue;
        }

        // Start and draw the frame.
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        RenderDesktop(config);
        // Add application windows and the taskbar here.

        RenderApplications(appState, config);
        RenderTaskbar(appState);

        RenderClock(config);

        // Display the frame.
        ImGui::Render();
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(config.wallpaperRed / 255.0f, config.wallpaperGreen / 255.0f,
                     config.wallpaperBlue / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Release all resources.
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
