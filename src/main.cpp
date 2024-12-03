#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <sstream>
#include "encode.h"
#include "decode.h"

// Custom stream buffer to capture cerr messages
class ErrorBuffer : public std::streambuf {
private:
    std::ostringstream oss; // String stream to store captured messages

public:
    // Override sputc to capture characters
    int overflow(int c) override {
        if (c != EOF) {
            oss.put(static_cast<char>(c));
        }
        return c;
    }

    // Get the current buffer content
    std::string getBuffer() const {
        return oss.str();
    }

    // Clear the buffer
    void clear() {
        oss.str("");
        oss.clear();
    }
};

// Class to encapsulate main window logic
class MainWindow {
private:
    // State variables
    bool encode = true;               // Toggle between Encode/Decode
    bool encodeTextFile = false;      // Toggle between encoding a message or a text file
    char filePath[512] = "";          // File path input
    char textFilePath[512] = "";      // Text file path input
    char message[512] = "";           // Message input
    char password[512] = "";          // Password input
    std::string output = "";          // Output message for success or decoding

    ErrorBuffer errorBuffer;          // Custom buffer for capturing cerr messages
    std::ostream errorStream{&errorBuffer}; // Stream linked to the custom buffer

    // Render options for encoding
    void RenderEncodeOptions() {
        ImGui::Checkbox("Encode Text File", &encodeTextFile);
        if (encodeTextFile) {
            ImGui::Text("Enter the path to the text file to encode into the target file.");
            ImGui::InputText("Text File Path", textFilePath, sizeof(textFilePath));
        } else {
            ImGui::Text("Enter the message to encode into the target file.");
            ImGui::InputText("Message", message, sizeof(message));
        }

        // Password input
        ImGui::Text("Enter a password:");
        ImGui::InputText("Password", password, sizeof(password), ImGuiInputTextFlags_Password);
    }

    // Handle form submission
    void HandleSubmit() {
        try {
            if (encode) {
                if (encodeTextFile) {
                    std::string result = encodeText(filePath, textFilePath, password);
                    output = "Text file encoded successfully: " + result;
                } else {
                    std::string result = encodeMessage(filePath, message, password);
                    output = "Message encoded successfully: " + result;
                }
            } else {
                output = "Decoded message: " + decode(filePath);
            }
        } catch (const std::exception& e) {
            errorStream << "Error: " << e.what() << std::endl;
        }
    }

    // Render the output and error messages
    void RenderOutput() {
        if (!output.empty()) {
            ImGui::Text("Output:");
            ImGui::Separator();
            ImGui::TextWrapped("%s", output.c_str());
        }

        // Display captured error messages
        std::string errorMessages = errorBuffer.getBuffer();
        if (!errorMessages.empty()) {
            ImGui::Text("Errors:");
            ImGui::Separator();
            ImGui::TextWrapped("%s", errorMessages.c_str());
        }
    }

    // Apply custom styling for the UI
    void ApplyStyling() {
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 5.0f;
        style.FrameRounding = 5.0f;
        style.FramePadding = ImVec2(10, 8);
        style.ItemSpacing = ImVec2(10, 10);

        // Custom colors
        ImVec4* colors = style.Colors;
        colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.2f, 1.0f);
        colors[ImGuiCol_Button] = ImVec4(0.2f, 0.5f, 0.8f, 1.0f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.6f, 0.9f, 1.0f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.1f, 0.4f, 0.7f, 1.0f);
        colors[ImGuiCol_Header] = ImVec4(0.2f, 0.5f, 0.8f, 1.0f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.3f, 0.6f, 0.9f, 1.0f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.1f, 0.4f, 0.7f, 1.0f);
    }

public:
    // Render the main window
    void Render() {
        ApplyStyling();

        // Match GLFW window size and position
        int display_w, display_h;
        glfwGetFramebufferSize(glfwGetCurrentContext(), &display_w, &display_h);
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2((float)display_w, (float)display_h));

        // Start ImGui window
        ImGui::Begin("Steganography", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        // Encode or Decode toggle
        ImGui::Text("Mode:");
        if (ImGui::RadioButton("Encode", encode)) {
            encode = true;
            output = "";
            errorBuffer.clear();
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("Decode", !encode)) {
            encode = false;
            output = "";
            errorBuffer.clear();
        }

        // File path for the target file (image/audio)
        ImGui::Text("Enter the path to the target file (.png or .wav).");
        ImGui::InputText("Target File Path", filePath, sizeof(filePath));

        if (encode) {
            RenderEncodeOptions();
        } else {
            // Password input
            ImGui::Text("Enter the password used for encoding:");
            ImGui::InputText("Password", password, sizeof(password), ImGuiInputTextFlags_Password);
        }

        // Submit button
        if (ImGui::Button("Submit")) {
            HandleSubmit();
        }

        // Render output and error messages
        RenderOutput();

        ImGui::End();
    }
};
