#include <iostream>
#include <fstream>
#include <numbers>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <cmath>
#include "Debug.hpp"
#include "File.hpp"
#include "Controller.hpp"
#include "imgui.h"
#include "Mesh.hpp"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Graphics.hpp"

#define PI 3.14159265358979323846

#if GL_ES
precision mediump float;
#endif

void TestFunction() {

    // call a function in another file
    std::cout << "Hello other world!" << std::endl;

    Debug::Logger::Console(Debug::Level::INFO, "%10d ::: %-15d ::: %10f", 9001, 42, 23.2);
    Debug::Logger::Console(Debug::Level::INFO, "%10d ::: %-15d ::: %10f", 69, 360, 1000.2);

    std::string currentPath = std::filesystem::current_path().generic_u8string();
    std::string parent = std::filesystem::current_path().parent_path().generic_u8string();
    std::string file = "Somefile.uwu";
    std::string firstuwu = (std::filesystem::current_path() / file).generic_u8string();

    std::cout << currentPath << std::endl;
    std::cout << parent << std::endl;
    std::cout << firstuwu << std::endl;

    Debug::Logger* tmp = new Debug::Logger(std::cout);

    //Below is equivalent, *pointer is dereference, &value is reference, type* referes to a pointer of the type 'type'
    //Dereference will take the value that the pointer points to
    //Reference will provide the address for the value 
    int a = 20;
    int* a_ptr = &a;
    int b = *a_ptr;
    printf("a=%d, addres of a: %p, addres of a_ptr: %p, b=%d, addres of b: %p\n", a, &a, &a_ptr, b, &b);

    tmp->Log(Debug::Level::INFO, "Test");
    (*tmp).Log(Debug::Level::INFO, "Test");

    std::ofstream filestream("Test.log"); //huh, no path required...
    Debug::Logger logger(filestream);

    logger.Log(Debug::Level::INFO, "Hello from the log! 2");

    logger.Log(Debug::Level::INFO, "Im a title", "Hello from the log! 3");

    logger.Log(Debug::Level::INFO, "Im your execution path!: %s", std::filesystem::current_path());

    printf("%d\n", logger.IsValid());

    filestream.close();

    printf("%d\n", logger.IsValid());
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int argc, const char* argv[]) {

    TestFunction();

    glfwSetErrorCallback(glfw_error_callback);

    Controller::Init();

    Debug::SetLogLevels(511 - (int)Debug::Level::CONTEXT);

    Window mainwindow("Main window", 800, 800);
    //Window secondarywindow("Secondary window", 400, 400, &mainwindow);

    try {
        Controller::Instance()->AddWindow(&mainwindow);
        //Controller::Instance()->AddWindow(&secondarywindow);
        // get version info
        const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
        const GLubyte* version = glGetString(GL_VERSION); // version as a string
        const GLubyte* vendor = glGetString(GL_VENDOR); // vecndor as a string
        const GLubyte* GLSLversion = glGetString(GL_SHADING_LANGUAGE_VERSION); // version of GLSL as a string
        Debug::Logger::Console(Debug::Level::INFO, "Renderer: %s", renderer);
        Debug::Logger::Console(Debug::Level::INFO, "OpenGL version supported: %s", version);
        Debug::Logger::Console(Debug::Level::INFO, "Vendor of GPU: %s", vendor);
        Debug::Logger::Console(Debug::Level::INFO, "GLSL version: %s", GLSLversion);
        int nrAttributes;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
        Debug::Logger::Console(Debug::Level::INFO, "Maximum nr of vertex attributes supported: %d", nrAttributes);
        int computeshadersharedmemory;
        glGetIntegerv(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &computeshadersharedmemory);
        Debug::Logger::Console(Debug::Level::INFO, "Compute shader max shared memory: %dB", computeshadersharedmemory);
        int computeshaderuniformblocks;
        glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_BLOCKS, &computeshaderuniformblocks);
        Debug::Logger::Console(Debug::Level::INFO, "Compute shader max uniform components (float, integer, boolean): %d", computeshaderuniformblocks);
        int computeshaderuniformcomponents;
        glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_COMPONENTS, &computeshaderuniformcomponents);
        Debug::Logger::Console(Debug::Level::INFO, "Compute shader max uniform components (float, integer, boolean): %d", computeshaderuniformcomponents);
        glm::ivec3 maxworkgroupsize;
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &maxworkgroupsize.x);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &maxworkgroupsize.y);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &maxworkgroupsize.z);
        Debug::Logger::Console(Debug::Level::INFO, "Compute shader max workgroup size: (%d, %d, %d)", maxworkgroupsize.x, maxworkgroupsize.y, maxworkgroupsize.z);
        glm::ivec3 maxworkgroupcount;
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &maxworkgroupcount.x);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &maxworkgroupcount.y);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &maxworkgroupcount.z);
        Debug::Logger::Console(Debug::Level::INFO, "Compute shader max workgroup count: (%d, %d, %d)", maxworkgroupcount.x, maxworkgroupcount.y, maxworkgroupcount.z);
        int computeshaderworkgroupinvocations;
        glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &computeshaderworkgroupinvocations);
        Debug::Logger::Console(Debug::Level::INFO, "Compute shader single local work group invocations that may be dispatched: %d", computeshaderworkgroupinvocations);
    }
    catch (const char* ex) {
        std::cerr << ex << std::endl;
        glfwTerminate();
        return 0;
    }

    

    //Load shaders into main window
    std::filesystem::path shaderfolder = std::filesystem::current_path() / "shaderprograms";

    for (std::pair<std::string, std::vector<load_shader>> programfolder : load_shader::LoadProgramSubfolders(shaderfolder)) {
        try {
            mainwindow.AddShader(programfolder.first, programfolder.second);
        }
        catch (Exception& err) {
            Debug::Logger::Console(Debug::Level::WARNING, "Invalid program during compilation in %s, error:\n %s", programfolder.first.c_str(), err.what());
        }
        
    }


    Mesh* mesh1 = Mesh::MeshedCube(2.0, 3);
    Mesh* mesh2 = Mesh::SphereWithNormals(4, 4, 0.2);

    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // Closest object to the camera will be drawn

    //Initialize ImGUI (for parameter testing in window)
    Graphics::InitializeImGUI(Controller::Instance()->GetContextWindow());

    bool shouldClose = false;
    while (!shouldClose)
    {
        //Debug::Logger::console("Displaying window " + i);
        Window* window = Controller::Instance()->GetContextWindow();

        //Run at the start of every render loop
        window->Clear();
        window->UpdateTime();
        window->ProcessKeyContinuous();
        Graphics::UpdateVariablesImGUI(window);

        //Render stuff her
        mesh1->Draw();
        mesh2->Draw();

        window->GetShader("normaltest")->Use();
        //Render stuff her
        mesh1->Draw();
        mesh2->Draw();

        //After rendering stuff
        Graphics::RenderImGUI(window);

        glfwSwapBuffers(window->GetGLContext());
        glfwPollEvents();

        shouldClose |= glfwWindowShouldClose(window->GetGLContext());
    }

    Graphics::TerminateImGUI();

    Controller::Instance()->ClearWindows();

    delete mesh1;
    delete mesh2;

    glfwTerminate();
}