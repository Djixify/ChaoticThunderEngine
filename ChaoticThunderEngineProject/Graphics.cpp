#include "Graphics.hpp"
#include "Debug.hpp"
#include <vector>
#include <ctime>
#include <filesystem>
#include <string>
#include <iostream>

namespace Graphics {
    bool show_demo_window = false, show_custom_shader_window = false;
    bool fill_mesh = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    float scale_uniform = 1.0f;
    bool has_loaded_shaderfiles = false;
    int selected_program = 0;
    int selected_vertex_shader = 0;
    int selected_fragment_shader = 0;
    int number_of_shaders = 0;
    std::vector<std::string> programs;
    std::vector<std::string> vertex_shaders;
    std::vector<std::string> fragment_shaders;
    clock_t start_time = 0;
    float current_time = 0;

    void UpdateShaderFiles() {
        for (const auto& entry : std::filesystem::directory_iterator("shaderprograms")) {
            std::wstring path = entry.path().native();
            std::string str_path = std::string(path.begin(), path.end());
            std::string vertext = ".vert";
            std::string fragext = ".frag";
            if (entry.is_regular_file() && str_path.substr(str_path.length() - 5, 5).compare(vertext) == 0)
                vertex_shaders.push_back(str_path);
            if (entry.is_regular_file() && str_path.substr(str_path.length() - 5, 5).compare(fragext) == 0)
                fragment_shaders.push_back(str_path);
        }
    }

    void InitializeImGUI(Window* window) {

        //Initialize ImGUI (for parameter testing in window)
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui::StyleColorsDark();
        ImGui_ImplOpenGL3_Init("#version 330");
        ImGui_ImplGlfw_InitForOpenGL(window->GetGLContext(), true);

        UpdateShaderFiles();
        window->AddShader("Custom", 2, load_shader{ VERTEX, vertex_shaders[selected_vertex_shader] }, load_shader{ FRAGMENT, fragment_shaders[selected_fragment_shader] });
        programs = window->GetShaderLabels();

        start_time = std::clock();
    }

    void ClearWindow(Window* window) {
        int display_w, display_h;
        glfwGetFramebufferSize(window->GetGLContext(), &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        Debug::Logger::ConsoleOpenGLError("During setting viewport in ClearWindow");
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        Debug::Logger::ConsoleOpenGLError("During setting clear color in ClearWindow");
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Debug::Logger::ConsoleOpenGLError("During clear call in ClearWindow");
    }

    void UpdateVariablesImGUI(Window* window) {
        glPolygonMode(GL_FRONT_AND_BACK, Graphics::fill_mesh ? GL_FILL : GL_LINE);
        Debug::Logger::ConsoleOpenGLError("During setting polygonmode");

        programs = window->GetShaderLabels();

        if (programs[selected_program].compare("Custom") == 0 && window->GetShader(programs[selected_program]) != nullptr) {
            Shader* activeshader = window->GetShader(std::string(programs[selected_program]));
            activeshader->Use();

            int scaleloc = glGetUniformLocation(activeshader->GetID(), "scale");
            if (scaleloc > -1)
                glUniform1f(scaleloc, scale_uniform);

            clock_t now = std::clock();
            int timeloc = glGetUniformLocation(activeshader->GetID(), "time");
            if (timeloc > -1) {
                current_time = ((float)(now - start_time)) / 1000.f;
                glUniform1f(timeloc, current_time);
            }
            else
                start_time = now;
        }


    }

    void RenderImGUI(Window* window) {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        const char** vert_items = nullptr;
        const char** frag_items = nullptr;
        if (selected_program == programs.size()) {

            if (!has_loaded_shaderfiles) {
                UpdateShaderFiles();
                has_loaded_shaderfiles = true;
            }

            ImGui::Begin("Customs shader window");                          // Create a window called "Hello, world!" and append into it.

            vert_items = new const char* [vertex_shaders.size()];
            for (int i = 0; i < vertex_shaders.size(); i++) {
                vert_items[i] = vertex_shaders[i].c_str();
            }

            frag_items = new const char* [fragment_shaders.size()];
            for (int i = 0; i < fragment_shaders.size(); i++) {
                frag_items[i] = fragment_shaders[i].c_str();
            }

            ImGui::Combo("Vertex shader", &selected_vertex_shader, vert_items, vertex_shaders.size());
            ImGui::Combo("Fragment shader", &selected_fragment_shader, frag_items, fragment_shaders.size());
            if (ImGui::Button("Compile")) {
                window->AddShader("Custom", 2, load_shader{ VERTEX, vertex_shaders[selected_vertex_shader] }, load_shader{ FRAGMENT, fragment_shaders[selected_fragment_shader] });
            }

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
        else {
            has_loaded_shaderfiles = false;
        }
        
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Run-time variables");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Checkbox("ImGUI help window", &show_demo_window);
        ImGui::Text("Rendering");
        ImGui::Checkbox("Fill mesh", &fill_mesh);
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        const char** items = new const char* [programs.size()];
        for (int i = 0; i < programs.size(); i++)
            items[i] = programs[i].c_str();

        ImGui::Combo("Shader program", &selected_program, items, programs.size());

        ImGui::Text("Uniforms");
        ImGui::SliderFloat("Scale", &scale_uniform, 0.1f, 2.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        std::string time_str = "Time: " + std::to_string(current_time) + " sec";
        ImGui::Text(time_str.c_str());


        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        delete[] items;
        delete[] vert_items; 
        delete[] frag_items;
    }

    void TerminateImGUI() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}