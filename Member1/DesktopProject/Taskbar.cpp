#include "Taskbar.h"
#include "imgui.h"

namespace {
    
void DrawAppButton(
    const char* icon,
    const char* tooltip,
    bool& isOpen,
    bool& isMinimized
) {
    const bool isActive = isOpen && !isMinimized;
    const ImVec2 buttonSize(42.0f, 34.0f);

    if (isActive) {
        ImGui::PushStyleColor(
            ImGuiCol_Button,
            ImVec4(0.15f, 0.35f, 0.55f, 1.0f)
        );
    }

    // Draw the clickable button.
    const ImVec2 buttonPos = ImGui::GetCursorScreenPos();

    if (ImGui::InvisibleButton(tooltip, buttonSize)) {
        if (!isOpen) {
            isOpen = true;
            isMinimized = false;
        } else if (isMinimized) {
            isMinimized = false;
        } else {
            isMinimized = true;
        }
    }

    // Draw the button background.
    ImDrawList* draw = ImGui::GetWindowDrawList();

    draw->AddRectFilled(
        buttonPos,
        ImVec2(
            buttonPos.x + buttonSize.x,
            buttonPos.y + buttonSize.y
        ),
        ImGui::GetColorU32(
            isActive ? ImGuiCol_Button : ImGuiCol_Button
        ),
        ImGui::GetStyle().FrameRounding
    );

    // Ignore the hidden ## button ID when measuring the icon.
    const char* iconEnd = icon;
    while (*iconEnd != '\0' &&
           !(iconEnd[0] == '#' && iconEnd[1] == '#')) {
        ++iconEnd;
    }

    const ImVec2 iconSize = ImGui::CalcTextSize(icon, iconEnd);

    // Adjust this value to move the visible glyph up or down.
    constexpr float verticalOffset = 3.0f;

    const ImVec2 iconPos(
        buttonPos.x + (buttonSize.x - iconSize.x) * 0.5f,
        buttonPos.y + (buttonSize.y - iconSize.y) * 0.5f
            + verticalOffset
    );

    draw->AddText(
        iconPos,
        ImGui::GetColorU32(ImGuiCol_Text),
        icon,
        iconEnd
    );

    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("%s", tooltip);
    }

    if (isActive) {
        ImGui::PopStyleColor();
    }
}

}

void RenderTaskbar(AppState& appState) {
    const ImGuiIO& io = ImGui::GetIO();
    constexpr float height = 54.0f;

    ImGui::SetNextWindowPos(
        ImVec2(0.0f, io.DisplaySize.y - height),
        ImGuiCond_Always
    );

    ImGui::SetNextWindowSize(
        ImVec2(io.DisplaySize.x, height),
        ImGuiCond_Always
    );

    const ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBringToFrontOnFocus;

    if (ImGui::Begin("##Taskbar", nullptr, flags)) {
        ImGui::SetCursorPosY(9.0f);

       // 0. Power
    const char* powerIcon = "\xef\x80\x91";
        ImVec2 size(42.0f, 34.0f);
        ImVec2 pos = ImGui::GetCursorScreenPos();

    if (ImGui::InvisibleButton("##power", size))
     appState.shutdownRequested = true;

        ImDrawList* draw = ImGui::GetWindowDrawList();

    draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y),
    ImGui::GetColorU32(ImGuiCol_Button), ImGui::GetStyle().FrameRounding);

    ImVec2 textSize = ImGui::CalcTextSize(powerIcon);
    draw->AddText(ImVec2(pos.x + (size.x - textSize.x) * 0.5f,
        pos.y + (size.y - textSize.y) * 0.5f + 3.0f),
         ImGui::GetColorU32(ImGuiCol_Text), powerIcon);

    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Power Off");

ImGui::SameLine(0.0f, 8.0f);

       
        // 1. System Information - Desktop icon
        DrawAppButton(
            "\xef\x84\x88##system_info",
            "System Information",
            appState.showSystemInformation,
            appState.systemInformationMinimized
        );

        ImGui::SameLine(0.0f, 8.0f);

        // 2. Calculator - Calculator icon
        DrawAppButton(
            "\xef\x87\xac##calculator",
            "Calculator",
            appState.showApplicationOne,
            appState.applicationOneMinimized
        );

        ImGui::SameLine(0.0f, 8.0f);

        // 3. Media Player - Music icon
        DrawAppButton(
            "\xef\x80\x81##media_player",
            "Media Player",
            appState.showApplicationTwo,
            appState.applicationTwoMinimized
        );

        ImGui::SameLine(0.0f, 8.0f);

        // 4. Task Manager - Chart bar icon
        DrawAppButton(
            "\xef\x82\x80##task_manager",
            "Task Manager",
            appState.showTaskManager,
            appState.taskManagerMinimized
        );


        // Right-side taskbar label
        const float labelWidth =
            ImGui::CalcTextSize("CSOPESY OS").x;

        ImGui::SameLine(
            io.DisplaySize.x - labelWidth - 24.0f
        );

        ImGui::SetCursorPosY(19.0f);
        ImGui::TextDisabled("CSOPESY OS");
    }

    ImGui::End();
}
