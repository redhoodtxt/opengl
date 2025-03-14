#include "shaders.h"

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(){
    ID = 0;
}

std::pair<std::string, std::string>  Shader::processShaderCode(const std::string& vertex_path, const std::string& fragment_path){
    std::string vertex_code;
    std::string fragment_code;
    std::ifstream vshader_file, fshader_file;

    // tells streams to throw exceptions when a file operation fails
    vshader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit); // check if either non fatal or fatal exceptions occur
    fshader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit); // check if either non fatal or fatal exceptions occur
    
    try{
        // open files
        vshader_file.open(vertex_path);
        fshader_file.open(fragment_path);   
        std::stringstream vshader_stream, fshader_stream;

        // read file contents and throw them into stream
        vshader_stream << vshader_file.rdbuf();
        fshader_stream << fshader_file.rdbuf();

        // close file handlers 
        vshader_file.close();
        fshader_file.close();

        // convert the stream into strings
        vertex_code = vshader_stream.str();
        fragment_code = fshader_stream.str();
    }
    // if exception is thrown at any stage in the try block, it is caught and thrown here as a failure object. e gives error details
    catch(std::ifstream::failure e){ 
        std::cerr << "ERROR: shader file was not read successfully" <<std::endl;
        return {"", ""};
    }

    // checks to see if the source code is parsed through properly. it should output the source code in the terminal
    if(!vertex_code.empty()){
        std::cout << vertex_code << std::endl;
    }
    if(!fragment_code.empty()){
        std::cout << fragment_code << std::endl;
    }

    return {vertex_code, fragment_code};

};

void Shader::use(unsigned int program){
    glUseProgram(program);

}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), int(value));
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}