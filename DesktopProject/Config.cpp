#include "Config.h"

#include <cmath>
#include <fstream>
#include <sstream>

static std::string Trim(const std::string& text) {
    const auto first = text.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    return text.substr(first, text.find_last_not_of(" \t\r\n") - first + 1);
}

// Keep the default when a number is invalid.
template <typename T>
static void ReadNumber(const std::string& text, T& target, T low, T high) {
    std::istringstream input(text);
    T value;
    if (!(input >> value)) return;
    input >> std::ws;
    if (input.eof() && std::isfinite(static_cast<double>(value)) &&
        value >= low && value <= high) {
        target = value;
    }
}

AppConfig LoadConfig(const std::string& path) {
    AppConfig config;
    std::ifstream file(path);
    std::string line;

    while (std::getline(file, line)) {
        line = Trim(line);
        if (line.empty() || line[0] == '#' || line[0] == ';') continue;
        const auto split = line.find('=');
        if (split == std::string::npos) continue;
        const std::string key = Trim(line.substr(0, split));
        const std::string value = Trim(line.substr(split + 1));

        if (key == "WINDOW_WIDTH") ReadNumber(value, config.windowWidth, 640, 3840);
        else if (key == "WINDOW_HEIGHT") ReadNumber(value, config.windowHeight, 480, 2160);
        else if (key == "WALLPAPER_RED") ReadNumber(value, config.wallpaperRed, 0, 255);
        else if (key == "WALLPAPER_GREEN") ReadNumber(value, config.wallpaperGreen, 0, 255);
        else if (key == "WALLPAPER_BLUE") ReadNumber(value, config.wallpaperBlue, 0, 255);
        else if (key == "TASKBAR_POSITION" && (value == "TOP" || value == "BOTTOM"))
            config.taskbarPosition = value;
        else if (key == "CLOCK_CORNER" &&
                 (value == "TOP_LEFT" || value == "TOP_RIGHT" ||
                  value == "BOTTOM_LEFT" || value == "BOTTOM_RIGHT"))
            config.clockCorner = value;

        for (int i = 0; i < 3; ++i) {
            const std::string prefix = "PROCESS_" + std::to_string(i + 1);
            auto& process = config.processes[i];
            if (key == prefix + "_NAME" && !value.empty() && value.size() <= 64)
                process.name = value;
            else if (key == prefix + "_CPU") ReadNumber(value, process.cpu, 0.0f, 100.0f);
            else if (key == prefix + "_MEMORY") ReadNumber(value, process.memory, 0.0f, 1048576.0f);
        }
    }
    return config;
}
