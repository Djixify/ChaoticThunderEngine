#ifndef ENGINE_STRUCTS
#define ENGINE_STRUCTS
#include <glad\glad.h>
#include <string>
#include "Enums.hpp"
#include "File.hpp"
#include "Controller.hpp"

/// <summary>
/// load_shader contains the content used to read a shader and determine the type
/// opengl should compile to
/// </summary>
struct load_shader {
    shader_type type;
    std::filesystem::path path;

    /// <summary>
    /// Takes a directory path and iterates through the files of the directory
    /// If exactly one vertex (.vert) and fragment shader is found (.frag), then it
    /// is a valid program, other extensions such as tessellation (.tesc and .tese)
    /// and geometry (.geom) are also valid, but optional
    /// </summary>
    /// <param name="path">The directory to be iterated through</param>
    /// <returns>The ordered vector of shaders to create a program</returns>
    static std::vector<load_shader> LoadProgramFolder(std::filesystem::path path) {
        //Mostly for readibility, could just be instanciated in the array instead
        std::string vertex_ext = ".vert";
        std::string control_tessellation_ext = ".tesc";
        std::string evaluation_tessellation_ext = ".tese";
        std::string geometry_ext = ".geom";
        std::string fragment_ext = ".frag";

        std::string extensions[]{
            vertex_ext,
            control_tessellation_ext,
            evaluation_tessellation_ext,
            geometry_ext,
            fragment_ext
        };

        //Used for fancy insertion and management of the program vector (below)
        shader_type pipeline_types[] = { shader_type::VERTEX , shader_type::TESS_CONTROL, shader_type::TESS_EVALUATION, shader_type::GEOMETRY, shader_type::FRAGMENT };
        bool pipeline_indices[] = {false, false, false, false, false};

        std::vector<load_shader> program;

        //Iterate through files to find appropriate programs
        for (std::filesystem::path file : File::GetFiles(path)) {
            for (int file_ext_iter = 0; file_ext_iter < 5; file_ext_iter++) {
                if (file.extension().compare(extensions[file_ext_iter]) == 0) {
                    int position = 0;
                    for (int i = 0; i < 5; i++)
                        position += pipeline_indices[i] ? 1 : 0;

                    if (position == program.size())
                        program.push_back(load_shader{ pipeline_types[file_ext_iter], file });
                    else
                        program.insert(program.begin() + position, load_shader{ pipeline_types[file_ext_iter], file});

                    if (!pipeline_indices[file_ext_iter])
                        pipeline_indices[file_ext_iter] = true;
                    else //Disallow multiple shaders of the same type 
                        Controller::Instance()->ThrowException("Dublicate shader type found with extension: " + extensions[file_ext_iter]);
                    break;
                }
            }
        }

        //If either vertex or fragment shader is missing, then it is an invalid program
        if (!pipeline_indices[0] || !pipeline_indices[4])
            Controller::Instance()->ThrowException(!pipeline_indices[0] ? "Invalid program, missing a vertex shader" : "Invalid program, missing a fragment shader");

        //If only either control tessellation shader or evaluation tessellation shader is set, then it is considered invalid (confirm if this is true?)
        if (pipeline_indices[1] ^ pipeline_indices[2]) //XOR, only true true or false false is accepted as a valid program (returns 0)
            Controller::Instance()->ThrowException(pipeline_indices[1] ? "Invalid program, control tessellation shader (.tesc) needs to have evaluation shader (.tese)" : "Invalid program, evaluation tessellation shader (.tese) needs to have control shader (.tesc)");

        return program;
    }

    static std::vector<std::pair<std::string, std::vector<load_shader>>> LoadProgramSubfolders(std::filesystem::path path) {
        std::vector<std::pair<std::string, std::vector<load_shader>>> programs;
        for (std::filesystem::path programfolder : File::GetDirectories(path)) {
            try {
                std::vector<load_shader> program = LoadProgramFolder(programfolder);
                std::string filename = program[0].path.parent_path().filename().generic_u8string();
                std::pair bundled{ filename, program };
                programs.push_back(bundled);
            }
            catch (Exception& err) {
                Debug::Logger::Console(Debug::Level::WARNING, "Invalid shader program found in %s, error:\n %s", programfolder.generic_u8string().c_str(), err.what());
            }
        }
        return programs;
    }
};

struct vertex_attribute {
    bool normalized;
    GLuint location;
    GLint count;
    GLuint stride;
    attribute_type type;
    GLuint offset;
};
#endif