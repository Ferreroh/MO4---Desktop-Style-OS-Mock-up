
#include "TaskManager.h"
#include "imgui.h"

void RenderTaskManager(AppState& appState, const AppConfig& config)
{
    if (!appState.showTaskManager || appState.taskManagerMinimized)
        return;

    ImGui::SetNextWindowSize(
        ImVec2(560.0f, 360.0f),
        ImGuiCond_FirstUseEver
    );

    if (ImGui::Begin(
            "Task Manager",
            nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse
        ))
    {
        // Custom title bar
        const float titleBarHeight = 30.0f;
        const float buttonWidth = 30.0f;
        const float windowWidth = ImGui::GetWindowWidth();

        ImGui::BeginGroup();
        ImGui::TextUnformatted("Task Manager");

        // Place the buttons in the upper-right corner.
        ImGui::SameLine(windowWidth - buttonWidth * 2.0f - 20.0f);

        if (ImGui::Button("_", ImVec2(buttonWidth, titleBarHeight)))
        {
            appState.taskManagerMinimized = true;
        }

        ImGui::SameLine();

        if (ImGui::Button("X", ImVec2(buttonWidth, titleBarHeight)))
        {
            appState.showTaskManager = false;
            appState.taskManagerMinimized = false;
        }

        ImGui::EndGroup();
        ImGui::Separator();

        // Processes heading
        ImGui::TextUnformatted("Processes");

        ImGui::Separator();

        // Calculate total CPU and memory usage.
        float totalCpu = 0.0f;
        float totalMemory = 0.0f;

        for (const ProcessInfo& process : config.processes)
        {
            totalCpu += process.cpu;
            totalMemory += process.memory;
        }

        ImGui::Text("Total CPU: %.1f%%", totalCpu);
        ImGui::SameLine();
        ImGui::Text("Total Memory: %.1f MB", totalMemory);

        ImGui::Spacing();

        // Processes table
        const ImGuiTableFlags flags =
            ImGuiTableFlags_Borders |
            ImGuiTableFlags_RowBg |
            ImGuiTableFlags_Resizable |
            ImGuiTableFlags_Reorderable |
            ImGuiTableFlags_SizingStretchProp;

        if (ImGui::BeginTable("ProcessTable", 3, flags))
        {
            ImGui::TableSetupColumn(
                "Process",
                ImGuiTableColumnFlags_WidthStretch
            );

            ImGui::TableSetupColumn(
                "CPU",
                ImGuiTableColumnFlags_WidthFixed,
                85.0f
            );

            ImGui::TableSetupColumn(
                "Memory",
                ImGuiTableColumnFlags_WidthFixed,
                110.0f
            );

            ImGui::TableHeadersRow();

            for (const ProcessInfo& process : config.processes)
            {
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::TextUnformatted(process.name.c_str());

                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%.1f%%", process.cpu);

                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%.1f MB", process.memory);
            }

            ImGui::EndTable();
        }
    }

    ImGui::End();

    // Reset minimized state when the window is closed.
    if (!appState.showTaskManager)
    {
        appState.taskManagerMinimized = false;
    }
}
