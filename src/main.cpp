#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream> 
#include "encode.h"
#include "decode.h"

void ShowMainWindow(GLFWwindow* window) {
    static bool encode = true;              // Toggle between Encode/Decode
    static bool encodeTextFile = false;    // Toggle between encoding a message or a text file
    static char filePath[256] = "";        // File path input (target file for encoding/decoding)
    static char textFilePath[256] = "";    // Text file path input (for encoding text files)
    static char message[512] = "";         // Message input (for encoding messages)
    static std::string output = "";        // Output message for decoding or success

    // Get the current size of the GLFW window
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);

    // Set ImGui window size to match the GLFW window
    ImGui::SetNextWindowPos(ImVec2(0, 0)); // Position at the top-left corner
    ImGui::SetNextWindowSize(ImVec2((float)display_w, (float)display_h)); // Fullscreen size

    // Begin ImGui window
    ImGui::Begin("Steganography", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);


    // Encode or Decode toggle
    if (ImGui::RadioButton("Encode", encode)) {
        encode = true;
        output = "";
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Decode", !encode)) {
        encode = false;
        output = "";
    }

    // File path for the target file (image/audio)
    ImGui::InputText("Target File Path", filePath, sizeof(filePath));

    if (encode) {
        // Toggle between encoding a message or a text file
        ImGui::Checkbox("Encode Text File", &encodeTextFile);

        if (encodeTextFile) {
            // Input for the text file path
            ImGui::InputText("Text File Path", textFilePath, sizeof(textFilePath));
        } else {
            // Input for the message to encode
            ImGui::InputText("Message", message, sizeof(message));
        }
    }

    // Submit button
    if (ImGui::Button("Submit")) {
        try {
            if (encode) {
                if (encodeTextFile) {
                    // Call the function to encode a text file
                    std::string result = encodeText(filePath, textFilePath);
                    output = "Text file encoded successfully: " + result;
                } else {
                    // Call the function to encode a direct message
                    std::string result = encodeMessage(filePath, message);
                    output = "Message encoded successfully: " + result;
                }
            } else {
                // Call the function to decode a file
                output = "Decoded message: " + decode(filePath);
            }

        } catch (const std::exception& e) {
            output = std::string("Error: ") + e.what();
        }
    }

    // Display output
    if (!output.empty()) {
        ImGui::TextWrapped("%s", output.c_str());
    }

    ImGui::End();
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }

    // Setup OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Core Profile
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required for macOS

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Steganography Frontend", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    // Initialize ImGui backend
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ShowMainWindow(window);

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}