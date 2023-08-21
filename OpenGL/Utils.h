#pragma once
#include <iostream>
#include <fstream>
#include <string>

#include "GLM/glm/glm.hpp"

class Utils {

public:

    static inline std::string ParseFile(const std::string filePath) {
        std::ifstream file(filePath);
        std::string line{};
        std::string content{};


        while (std::getline(file, line)) {
            content.append(line + "\n");
        } 

        return content;
    }

    static inline void PrintVec3(const glm::vec3& vector) {
        std::cout << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")" << '\n';
    }

};