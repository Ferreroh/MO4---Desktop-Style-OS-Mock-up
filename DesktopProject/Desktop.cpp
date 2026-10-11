#include "Desktop.h"
#include "imgui.h"

#include <ctime>

static constexpr float TaskbarHeight = 44.0f;
static constexpr ImGuiWindowFlags FixedWindow =
    ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoSavedSettings;

void RenderWallpaper(const AppConfig& config) {
    const auto* viewport = ImGui::GetMainViewport();
    ImGui::GetBackgroundDrawList()->AddRectFilled(
        viewport->Pos,
        ImVec2(viewport->Pos.x + viewport->Size.x, viewport->Pos.y + viewport->Size.y),
        IM_COL32(config.wallpaperRed, config.wallpaperGreen, config.wallpaperBlue, 255));
}

void RenderDesktop(const AppConfig& config) {
    RenderWallpaper(config);
    const auto* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::Begin("Desktop", nullptr, FixedWindow | ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBringToFrontOnFocus);
    ImGui::End();
    ImGui::PopStyleVar(2);
}

void RenderClock(const AppConfig& config) {
    const std::time_t now = std::time(nullptr);
    std::tm local{};
#ifdef _WIN32
    if (localtime_s(&local, &now) != 0) return;
#else
    if (localtime_r(&now, &local) == nullptr) return;
#endif
    char timeText[32];
    char dateText[64];
    std::strftime(timeText, sizeof(timeText), "%I:%M:%S %p", &local);
    std::strftime(dateText, sizeof(dateText), "%B %d, %Y", &local);

    const auto* viewport = ImGui::GetMainViewport();
    const bool right = config.clockCorner.find("RIGHT") != std::string::npos;
    const bool bottom = config.clockCorner.find("BOTTOM") != std::string::npos;
    const float width = 210;
    const float height = 58;
    float x = viewport->Pos.x + (right ? viewport->Size.x - width - 12 : 12);
    float y = viewport->Pos.y + (bottom ? viewport->Size.y - height - 12 : 12);
    if (bottom && config.taskbarPosition == "BOTTOM") y -= TaskbarHeight;
    if (!bottom && config.taskbarPosition == "TOP") y += TaskbarHeight;

    // Draw on top so the clock stays visible.
    auto* draw = ImGui::GetForegroundDrawList();
    draw->AddRectFilled(ImVec2(x, y), ImVec2(x + width, y + height), IM_COL32(15, 18, 25, 240), 6);
    draw->AddText(ImVec2(x + 12, y + 9), IM_COL32_WHITE, timeText);
    draw->AddText(ImVec2(x + 12, y + 32), IM_COL32_WHITE, dateText);
}

void RenderPowerButton(const AppConfig& config, AppState& state) {
    const auto* viewport = ImGui::GetMainViewport();
    const float y = viewport->Pos.y + (config.taskbarPosition == "TOP"
        ? 8.0f : viewport->Size.y - 36.0f);
    ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x + 8, y));
    ImGui::Begin("Power", nullptr, FixedWindow | ImGuiWindowFlags_AlwaysAutoResize);
    if (ImGui::Button("PWR")) state.shutdownRequested = true;
    ImGui::End();
}
