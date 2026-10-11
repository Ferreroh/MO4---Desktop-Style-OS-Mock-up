#pragma once

#include <array>
#include <string>

struct ProcessInfo {
    std::string name;
    float cpu;
    float memory;
};

struct AppConfig {
    int windowWidth = 1280;
    int windowHeight = 720;
    std::string taskbarPosition = "BOTTOM";
    std::string clockCorner = "TOP_RIGHT";
    int wallpaperRed = 25;
    int wallpaperGreen = 35;
    int wallpaperBlue = 55;
    std::array<ProcessInfo, 3> processes{{
        {"Desktop Shell", 2.4f, 45.2f},
        {"Application One", 1.8f, 32.5f},
        {"Application Two", 3.1f, 51.0f}
    }};
};

AppConfig LoadConfig(const std::string& path);
