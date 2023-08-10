#pragma once
#include <iostream>
#include <fstream>
#include <string>

class Utils {

public:

    static inline std::string ParseFile(const std::string filePath) {
        std::ifstream file(filePath);
        std::string line;
        std::string content;

        while (std::getline(file, line)) {
            content.append(line + "\n");
        }

        file.close(); // Creo que ifstream cierra automáticamente los archivos fuera de scope. Creo.

        return content;
    }

};