#include "Graphics.hpp"
#include "Debug.hpp"
#include <vector>

namespace Graphics {
    bool show_demo_window = false;
    bool fill_mesh = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    float scale_uniform = 1.0f;
    int selected_shader = 0;
    int number_of_shaders = 0;
    std::vector<std::string> shaders;

    void InitializeImGUI(Window* window) {

        //Initialize ImGUI (for parameter testing in window)
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui::StyleColorsDark();
        ImGui_ImplOpenGL3_Init("#version 330");
        ImGui_ImplGlfw_InitForOpenGL(window->GetGLContext(), true);

        shaders = window->GetShaderLabels();
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

        if (selected_shader < shaders.size()) {
            Shader* activeshader = window->GetShader(std::string(shaders[selected_shader]));
            activeshader->Use();

            activeshader->SetUniform("scale", scale_uniform);
        }
    }

    void RenderImGUI(Window* window) {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Run-time variables");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Checkbox("ImGUI help window", &show_demo_window);
        ImGui::Text("Rendering");
        ImGui::Checkbox("Fill mesh", &fill_mesh);
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        const char** items = new const char* [shaders.size()];
        for (int i = 0; i < shaders.size(); i++)
            items[i] = shaders[i].c_str();

        ImGui::Combo("Shader program", &selected_shader, items, shaders.size());

        ImGui::Text("Uniforms");
        ImGui::SliderFloat("Scale", &scale_uniform, 0.1f, 2.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        delete[] items;
    }

    void TerminateImGUI() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}