#include "Graphics.hpp"

namespace Graphics {
    bool show_demo_window = false;
    bool fill_mesh = true;
    float scale = 1.0f;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    void InitializeImGUI(Window* window) {

        //Initialize ImGUI (for parameter testing in window)
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui::StyleColorsDark();
        ImGui_ImplOpenGL3_Init("#version 330");
        ImGui_ImplGlfw_InitForOpenGL(window->GetGLContext(), true);
    }

    void ClearWindow(Window* window) {
        int display_w, display_h;
        glfwGetFramebufferSize(window->GetGLContext(), &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void UpdateVariablesImGUI(Window* window) {
        glPolygonMode(GL_FRONT_AND_BACK, Graphics::fill_mesh ? GL_FILL : GL_LINE);

        int scaleloc = glGetUniformLocation(window->GetShader("test")->GetID(), "scale");
        if (scaleloc > -1)
            glUniform1f(scaleloc, scale);
    }

    void RenderImGUI(Window* window) {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Fill mesh", &fill_mesh);               // Edit bools storing our window open/close state

        ImGui::SliderFloat("Scale", &scale, 0.1f, 2.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void TerminateImGUI() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}