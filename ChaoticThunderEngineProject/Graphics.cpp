#include <vector>
#include <ctime>
#include <filesystem>
#include <string>
#include <iostream>
#include "Graphics.hpp"
#include "Debug.hpp"

#define PI 3.14159265358979323846

namespace Graphics {
    bool show_demo_window = false, show_info_window = false;
    bool fill_mesh = true;
    bool render_normals = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImVec4 camera_position = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    ImVec4 model_color = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);

    ImVec4 light_position = ImVec4(0.0f, 10.0f, 0.0f, 0.0f);
    bool circling_light = false;
    float ambient_light = 0.2f;
    bool use_blinn_lighting = false;
    ImVec4 specular_light = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
    float diffuse_light_threshold = 90.0f;
    float diffuse_falloff = 0.00001f;

    float mouse_sensitivity = 0.0f;
    int camera_fov = 90;

    int selected_program = 0;
    int number_of_shaders = 0;
    std::vector<std::string> programs;
    clock_t start_time = 0;
    float current_time = 0;

    void InitializeImGUI(Window* window) {

        //Initialize ImGUI (for parameter testing in window)
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io; 
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui::StyleColorsDark();
        ImGui_ImplOpenGL3_Init("#version 330");
        ImGui_ImplGlfw_InitForOpenGL(window->GetGLContext(), true);

        programs = window->GetShaderLabels();
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
        if (window->GetShader(programs[selected_program]) != nullptr) {
            Shader* activeshader = window->GetShader(std::string(programs[selected_program]));
            activeshader->Use();
            activeshader->SetUniform("lightpos", glm::vec3(light_position.x, light_position.y, light_position.z));
            activeshader->SetUniform("blinnlighting", use_blinn_lighting);
            activeshader->SetUniform("ambientlight", ambient_light);
            activeshader->SetUniform("specular_color", glm::vec3(specular_light.x, specular_light.y, specular_light.z));
            activeshader->SetUniform("diffuse_cutoff", (diffuse_light_threshold / 90 - 1));
            activeshader->SetUniform("diffuse_falloff", diffuse_falloff);
            activeshader->SetUniform("model_color", glm::vec3(model_color.x, model_color.y, model_color.z));
        }
    }

    bool ShouldRenderNormals() {
        return render_normals;
    }

    void MainSettingsWindow(Window* window) {

        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Run-time variables");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("Rendering");
        ImGui::Checkbox("OpenGL state info window", &show_info_window);
        ImGui::Checkbox("Fill mesh", &fill_mesh);
        ImGui::Checkbox("Render normals", &render_normals);
        ImGui::ColorEdit3("Clear color", (float*)&clear_color); // Edit 3 floats representing a color
        window->ClearColor.x = clear_color.x;
        window->ClearColor.y = clear_color.y;
        window->ClearColor.z = clear_color.z;

        const char** items = new const char* [programs.size()];
        for (int i = 0; i < programs.size(); i++)
            items[i] = programs[i].c_str();

        ImGui::Combo("Shader program", &selected_program, items, programs.size());

        ImGui::Text("Uniforms");
        std::string time_str = "Time: " + std::to_string(window->GetCurrentTimeSec()) + " sec";
        ImGui::Text(time_str.c_str());
        ImGui::ColorEdit3("Model color", (float*)&model_color); // Edit 3 floats representing a color
        ImGui::InputFloat3("Light position", (float*)&light_position); // Edit 3 floats representing a color
        ImGui::Checkbox("Circling light", &circling_light);
        if (circling_light) {
            light_position = ImVec4(glm::sin(window->GetCurrentTimeSec()) * 10, light_position.y, glm::cos(window->GetCurrentTimeSec()) * 10, 0);
        }

        /*
        float ambient_light = 0.2f;
        bool use_blinn_lighting = false;
        ImVec4 specular_light = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
        float diffuse_light_threshold = 0.0;
        bool diffuse_falloff_gradual = false;*/
        ImGui::Checkbox("Blinn lighting", &use_blinn_lighting);
        ImGui::SliderFloat("Ambient light", &ambient_light, 0.0f, 1.0f);
        ImGui::ColorEdit3("Specular light", (float*)&specular_light); // Edit 3 floats representing a color
        ImGui::SliderFloat("Diffuse cutoff angle", &diffuse_light_threshold, 0.f, 180.f);
        ImGui::SliderFloat("Diffuse falloff", &diffuse_falloff, -5, 5);
        if (diffuse_falloff == 0.0f)
            diffuse_falloff += 0.000001f;

        ImGui::Text("Camera (press Q to lock mouse, E to unlock)");
        BaseCamera* camera = window->GetActiveCamera();
        glm::vec3 pos = window->GetActiveCamera()->Position;
        camera_position = ImVec4(pos.x, pos.y, pos.z, 1.0f);
        ImGui::InputFloat3("Position", (float*)&camera_position); // Edit 3 floats representing a color
        camera->Position.x = camera_position.x;
        camera->Position.y = camera_position.y;
        camera->Position.z = camera_position.z;

        //std::string pos_str = "Position: " + std::to_string(pos.x) + " " + std::to_string(pos.y) + " " + std::to_string(pos.z);
        //ImGui::Text(pos_str.c_str());
        mouse_sensitivity = camera->Sensitivity * 1000.f;
        ImGui::SliderFloat("Mouse sensitivity", &mouse_sensitivity, 10.f, 100.f);
        camera->Sensitivity = mouse_sensitivity / 1000.f;
        camera_fov = (int)camera->Fov;
        ImGui::SliderInt("Camera FOV", &camera_fov, 45, 135);
        camera->Fov = (float)camera_fov;
        ImGui::SliderFloat("Near plane", &camera->Near_plane, 0.1f, 10.0f);
        ImGui::SliderFloat("Far plane", &camera->Far_plane, 10.0f, 100.0f);

        //if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        //    counter++;
        //ImGui::SameLine();
        //ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::Checkbox("ImGUI help window", &show_demo_window);
        ImGui::End();

        delete[] items;
    }

    void OpenGLInfoWindow(Window* window) {
        ImGui::Begin("OpenGL state");                          // Create a window called "Hello, world!" and append into it.

        std::string tmp = "Array buffers: " + std::to_string(ArrayBuffer::GetAliveBuffers());
        ImGui::Text(tmp.c_str());
        tmp = "V. Data buffers: " + std::to_string(VertexDataBuffer::GetAliveBuffers());
        ImGui::Text(tmp.c_str());
        tmp = "V. Index buffers: " + std::to_string(VertexIndexBuffer::GetAliveBuffers());
        ImGui::Text(tmp.c_str());

        tmp = "V. Data buffer memory: " + std::to_string(VertexDataBuffer::GetMemoryUsed());
        ImGui::Text(tmp.c_str());
        tmp = "V. Index buffer memory: " + std::to_string(VertexIndexBuffer::GetMemoryUsed());
        ImGui::Text(tmp.c_str());


        unsigned char out_bool = 0;
        glGetBooleanv(GL_CULL_FACE, &out_bool);
        tmp = "Culling polygon faces: " + std::string(out_bool == false ? "false" : "true");
        ImGui::Text(tmp.c_str());

        int out_int = 0;
        glGetIntegerv(GL_CULL_FACE_MODE, &out_int);
        tmp = "Culling polygon faces mode: " + std::string(out_int == GL_FRONT ? "FRONT" : (out_int == GL_BACK ? "BACK" : "UNKNOWN"));
        
        ImGui::Text(tmp.c_str());

        ImGui::End();
    }

    void RenderImGUI(Window* window) {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        if (show_info_window)
            OpenGLInfoWindow(window);

        MainSettingsWindow(window);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    }

   

    void TerminateImGUI() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}
