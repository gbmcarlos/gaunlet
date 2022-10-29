#pragma once

#include "gaunlet/pch.h"

namespace gaunlet::Graphics {

    std::string parseInclude(const std::string& line, const std::filesystem::path& folder);

    std::string parseShaderFile(const std::string& filePath) {

        std::filesystem::path path(filePath);
        auto folder = path.parent_path();

        std::stringstream shaderSource;
        std::ifstream file(path.c_str());

        std::string line;
        while (std::getline(file, line)) {

            std::string lineContent = line;

            if (line.find("#include") != std::string::npos) {
                lineContent = parseInclude(line, folder);
            }

            shaderSource << lineContent << '\n';

        }

        return shaderSource.str();

    }

    std::string parseInclude(const std::string& line, const std::filesystem::path& folder) {

        auto firstQuote = line.find_first_of('"');
        auto lastQuote = line.find_last_of('"');
        std::string includePath = line.substr(firstQuote + 1, lastQuote - firstQuote - 1);
        includePath = std::regex_replace(includePath, std::regex("GL_PREFABS_PATH"), GL_PREFABS_PATH);

        if (std::filesystem::path(includePath).is_relative()) {
            includePath = std::string(folder.c_str()) + "/" + includePath;
        }

        return parseShaderFile(includePath);

    }

}