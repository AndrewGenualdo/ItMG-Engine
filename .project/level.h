//
// Created by cobble on 12/27/2024.
//

#ifndef ENGINE_2_LEVEL_H
#define ENGINE_2_LEVEL_H

#include <vector>
#include <fstream>
#include <sstream>
#include "platform.h"


class Level {
public:
    std::vector<Platform> platforms;
    Level() {
        platforms = std::vector<Platform>();
    }

    void load(const std::string& path) {

        std::ifstream file(path);
        if(!file.is_open()) {
            std::cerr << "Failed to load level file '" + path+"'" << std::endl;
            return;
        }

        platforms.clear();

        std::string line;
        while(std::getline(file, line)) {
            std::istringstream stream(line);
            float x, y, w, h;
            char solid;

            if(stream >> x >> y >> w >> h >> solid) {
                platforms.emplace_back(x, y, w, h, solid == 'T');
            } else {
                std::cerr << "Failed to load line: '" << line << "' in file '" << path << "'" << std::endl;
            }
        }
        file.close();
        std::cout << "Successfully loaded level file: " << path << std::endl;
    }

    void save(const std::string& path) {
        std::ofstream file(path);
        if(!file.is_open()) {
            std::cerr << "Failed to write to file '" << path << "'" << std::endl;
            return;
        }

        for(const auto & platform : platforms) {
            file << platform.x << " " << platform.y << " " << platform.w << " " << platform.h << " " << (platform.solid ? "T" : "F") << "\n";
        }
        file.close();
        std::cout << "Successfully saved level file: " << path << std::endl;
    }
};

#endif //ENGINE_2_LEVEL_H
