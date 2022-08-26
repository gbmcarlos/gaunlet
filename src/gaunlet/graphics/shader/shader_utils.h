#pragma once

#include "gaunlet/pch.h"

namespace gaunlet::Graphics {

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