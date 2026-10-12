#include "TaskManager.h"
#include "imgui.h"

namespace
{
    // -1 means no process is selected.
    int selectedProcess = -1;
}

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
        
        // End Task button
        const char* endTaskIcon = "\xef\x87\xb8";
        const char* endTaskLabel = "\xef\x87\xb8  End Task";
        ImGui::PushStyleColor(
            ImGuiCol_Text,
            ImVec4(0.0f, 0.0f, 0.0f, 0.0f)
        );
        const bool endTaskClicked = ImGui::Button(endTaskLabel);
        ImGui::PopStyleColor();

        const ImVec2 itemMin = ImGui::GetItemRectMin();
        const ImVec2 framePadding = ImGui::GetStyle().FramePadding;
        const ImVec2 labelPos(
            itemMin.x + framePadding.x,
            itemMin.y + framePadding.y
        );
        const ImVec2 iconSize = ImGui::CalcTextSize(endTaskIcon);
        const ImVec2 spacingSize = ImGui::CalcTextSize("  ");
        const ImU32 textColor = ImGui::GetColorU32(ImGuiCol_Text);
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        drawList->AddText(
            ImVec2(labelPos.x, labelPos.y + 2.5f),
            textColor,
            endTaskIcon
        );
        drawList->AddText(
            ImVec2(labelPos.x + iconSize.x + spacingSize.x, labelPos.y),
            textColor,
            "End Task"
        );

        if (endTaskClicked)
        {
            if (selectedProcess == 1)
            {
                // End the simulated Calculator process.
                appState.showApplicationOne = false;
                appState.applicationOneMinimized = false;
                selectedProcess = -1;
            }
            else if (selectedProcess == 2)
            {
                // End the simulated Media Player process.
                appState.showApplicationTwo = false;
                appState.applicationTwoMinimized = false;
                selectedProcess = -1;
            }
        }

        ImGui::Separator();

        // Only count processes that are currently running.
        float totalCpu = 0.0f;
        float totalMemory = 0.0f;

        if (!config.processes.empty())
        {
            totalCpu += config.processes[0].cpu;
            totalMemory += config.processes[0].memory;
        }

        if (config.processes.size() > 1 &&
            appState.showApplicationOne)
        {
            totalCpu += config.processes[1].cpu;
            totalMemory += config.processes[1].memory;
        }

        if (config.processes.size() > 2 &&
            appState.showApplicationTwo)
        {
            totalCpu += config.processes[2].cpu;
            totalMemory += config.processes[2].memory;
        }

        ImGui::Text("Total CPU: %.1f%%", totalCpu);
        ImGui::SameLine();
        ImGui::Text("Total Memory: %.1f MB", totalMemory);

        ImGui::Spacing();

        // Process table
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

            // System process: always present and cannot be ended.
            if (!config.processes.empty())
            {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);

                if (ImGui::Selectable(
                        "System",
                        selectedProcess == 0,
                        ImGuiSelectableFlags_SpanAllColumns
                    ))
                {
                    selectedProcess = 0;
                }

                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%.1f%%", config.processes[0].cpu);

                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%.1f MB", config.processes[0].memory);
            }

            // Calculator process: shown only while open.
            if (config.processes.size() > 1 &&
                appState.showApplicationOne)
            {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);

                if (ImGui::Selectable(
                        "Calculator",
                        selectedProcess == 1,
                        ImGuiSelectableFlags_SpanAllColumns
                    ))
                {
                    selectedProcess = 1;
                }

                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%.1f%%", config.processes[1].cpu);

                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%.1f MB", config.processes[1].memory);
            }

            // Media Player process: shown only while open.
            if (config.processes.size() > 2 &&
                appState.showApplicationTwo)
            {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);

                if (ImGui::Selectable(
                        "Media Player",
                        selectedProcess == 2,
                        ImGuiSelectableFlags_SpanAllColumns
                    ))
                {
                    selectedProcess = 2;
                }

                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%.1f%%", config.processes[2].cpu);

                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%.1f MB", config.processes[2].memory);
            }

            ImGui::EndTable();
        }

        if (selectedProcess == 0)
        {
            ImGui::TextDisabled(
                "System is protected and cannot be ended."
            );
        }
    }

    ImGui::End();

    if (!appState.showTaskManager)
    {
        appState.taskManagerMinimized = false;
    }
}
