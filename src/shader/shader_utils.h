#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>

namespace engine {

    unsigned int compileShaderProgram(unsigned int type, const std::string& source) {

        unsigned int id = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)alloca(length * sizeof(char));
            glGetShaderInfoLog(id, length, &length, message);
            std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader. Error:" << std::endl;
            std::cout << message << std::endl;
            glDeleteShader(id);
            return 0;
        }

        return id;

    }

    std::string readFromFile(const std::string& filePath) {

        std::string fileContent;
        std::ifstream file;

        // Make sure to throw exception if anything fails
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            file.open(filePath.c_str());
            std::stringstream fileStream;
            fileStream << file.rdbuf();
            file.close();
            fileContent = fileStream.str();
        } catch (std::ifstream::failure& e) {
            std::cout << "Error while reading from file '" << filePath.c_str() << "': " << e.what() << std::endl;
        }

        return fileContent;

    }

}