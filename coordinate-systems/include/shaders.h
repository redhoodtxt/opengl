#ifndef SHADER_H
#define SHADER_H

#include <string>

class Shader {
    public:
        unsigned int ID; // program id

        Shader();
        std::pair<std::string, std::string> processShaderCode(const std::string& vertex_path, const std::string& fragment_path);
        void use(unsigned int program); // function to use program

        // functions to modify existing uniform variables 
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
};

#endif