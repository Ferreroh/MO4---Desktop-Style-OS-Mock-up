
#include "Applications.h"

#include "imgui.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

// ============================================================
// CALCULATOR AND MEDIA PLAYER STATE
// ============================================================

namespace
{
    char calculatorDisplay[64] = "0";

    double storedValue = 0.0;
    char pendingOperator = '\0';
    bool startNewNumber = true;

    double CurrentNumber()
    {
        return std::strtod(calculatorDisplay, nullptr);
    }

    void SetDisplay(double value)
    {
        std::snprintf(
            calculatorDisplay,
            sizeof(calculatorDisplay),
            "%.10g",
            value
        );
    }

    void ClearCalculator()
    {
        std::snprintf(
            calculatorDisplay,
            sizeof(calculatorDisplay),
            "%s",
            "0"
        );

        storedValue = 0.0;
        pendingOperator = '\0';
        startNewNumber = true;
    }

    void EnterDigit(char digit)
    {
        std::size_t length =
            std::char_traits<char>::length(calculatorDisplay);

        if (startNewNumber ||
            length >= sizeof(calculatorDisplay) - 1)
        {
            calculatorDisplay[0] = digit;
            calculatorDisplay[1] = '\0';
            startNewNumber = false;
            return;
        }

        if (length == 1 && calculatorDisplay[0] == '0')
        {
            calculatorDisplay[0] = digit;
            calculatorDisplay[1] = '\0';
            return;
        }

        calculatorDisplay[length] = digit;
        calculatorDisplay[length + 1] = '\0';
    }

    void AddDecimalPoint()
    {
        if (startNewNumber)
        {
            std::snprintf(
                calculatorDisplay,
                sizeof(calculatorDisplay),
                "%s",
                "0."
            );

            startNewNumber = false;
            return;
        }

        if (std::strchr(calculatorDisplay, '.') == nullptr)
        {
            std::size_t length =
                std::char_traits<char>::length(calculatorDisplay);

            if (length < sizeof(calculatorDisplay) - 1)
            {
                calculatorDisplay[length] = '.';
                calculatorDisplay[length + 1] = '\0';
            }
        }
    }

    void CalculateResult()
    {
        if (pendingOperator == '\0')
            return;

        double currentValue = CurrentNumber();
        double result = currentValue;

        switch (pendingOperator)
        {
        case '+':
            result = storedValue + currentValue;
            break;

        case '-':
            result = storedValue - currentValue;
            break;

        case '*':
            result = storedValue * currentValue;
            break;

        case '/':
            if (currentValue == 0.0)
            {
                std::snprintf(
                    calculatorDisplay,
                    sizeof(calculatorDisplay),
                    "%s",
                    "Error"
                );

                pendingOperator = '\0';
                startNewNumber = true;
                return;
            }

            result = storedValue / currentValue;
            break;

        default:
            break;
        }

        SetDisplay(result);
        pendingOperator = '\0';
        startNewNumber = true;
    }

    void ChooseOperator(char op)
    {
        if (pendingOperator != '\0' && !startNewNumber)
            CalculateResult();

        storedValue = CurrentNumber();
        pendingOperator = op;
        startNewNumber = true;
    }

    // ========================================================
    // MEDIA PLAYER STATE
    // ========================================================

    const char* tracks[] =
    {
        "Milky Way",
        "Be With You",
        "Love Is",
        "Someday",
        "Running to Vancouver"
    };

    const char* artists[] =
    {
        "The Ridleys",
        "The Ridleys",
        "The Ridleys",
        "The Ridleys",
        "The Ridleys"
    };

    constexpr int trackCount =
        static_cast<int>(sizeof(tracks) / sizeof(tracks[0]));

    int currentTrack = 0;
    bool isPlaying = false;
    float volume = 0.65f;
    float playbackProgress = 0.0f;

    void ChangeTrack(int direction)
    {
        currentTrack += direction;

        if (currentTrack >= trackCount)
            currentTrack = 0;

        if (currentTrack < 0)
            currentTrack = trackCount - 1;

        playbackProgress = 0.0f;
    }
}

// ============================================================
// CUSTOM WINDOW TITLE BAR
// ============================================================

void DrawCustomTitleBar(
    const char* title,
    bool& isOpen,
    bool& isMinimized
)
{
    const ImVec2 windowPos = ImGui::GetWindowPos();
    const float windowWidth = ImGui::GetWindowWidth();

    constexpr float titleBarHeight = 32.0f;
    constexpr float buttonSize = 24.0f;
    constexpr float buttonTop = 4.0f;
    constexpr float rightPadding = 4.0f;

    ImDrawList* drawList = ImGui::GetWindowDrawList();

    drawList->AddRectFilled(
        windowPos,
        ImVec2(
            windowPos.x + windowWidth,
            windowPos.y + titleBarHeight
        ),
        ImGui::GetColorU32(ImGuiCol_TitleBgActive)
    );

    drawList->AddText(
        ImVec2(windowPos.x + 10.0f, windowPos.y + 9.0f),
        ImGui::GetColorU32(ImGuiCol_Text),
        title
    );

    const float dragAreaWidth = std::max(
        1.0f,
        windowWidth - 2.0f * buttonSize - 12.0f
    );

    ImGui::SetCursorScreenPos(windowPos);

    ImGui::InvisibleButton(
        "##title_drag",
        ImVec2(dragAreaWidth, titleBarHeight)
    );

    if (ImGui::IsItemActive() &&
        ImGui::IsMouseDragging(ImGuiMouseButton_Left))
    {
        const ImVec2 currentPos = ImGui::GetWindowPos();
        const ImVec2 mouseDelta = ImGui::GetIO().MouseDelta;

        ImGui::SetWindowPos(
            ImVec2(
                currentPos.x + mouseDelta.x,
                currentPos.y + mouseDelta.y
            )
        );
    }

    // Minimize button
    ImGui::SetCursorScreenPos(
        ImVec2(
            windowPos.x + windowWidth - 2.0f * buttonSize
                - rightPadding,
            windowPos.y + buttonTop
        )
    );

    if (ImGui::Button("_##minimize", ImVec2(buttonSize, buttonSize)))
        isMinimized = true;

    // Close button
    ImGui::SetCursorScreenPos(
        ImVec2(
            windowPos.x + windowWidth - buttonSize - rightPadding,
            windowPos.y + buttonTop
        )
    );

    if (ImGui::Button("×##close", ImVec2(buttonSize, buttonSize)))
        isOpen = false;

    ImGui::SetCursorPos(
        ImVec2(8.0f, titleBarHeight + 8.0f)
    );
}

// ============================================================
// CALCULATOR APPLICATION
// ============================================================

void RenderCalculator(AppState& appState)
{
    if (!appState.showApplicationOne ||
        appState.applicationOneMinimized)
    {
        return;
    }

    bool isOpen = true;

    ImGui::SetNextWindowSize(
        ImVec2(300.0f, 390.0f),
        ImGuiCond_FirstUseEver
    );

    ImGui::PushStyleVar(
        ImGuiStyleVar_WindowPadding,
        ImVec2(0.0f, 0.0f)
    );

    if (ImGui::Begin(
        "Calculator",
        nullptr,
        ImGuiWindowFlags_NoTitleBar
    ))
    {
        DrawCustomTitleBar(
            "Calculator",
            isOpen,
            appState.applicationOneMinimized
        );

        ImGui::SetNextItemWidth(-1.0f);

        ImGui::InputText(
            "##calculator_display",
            calculatorDisplay,
            sizeof(calculatorDisplay),
            ImGuiInputTextFlags_ReadOnly
        );

        ImGui::Spacing();

        const float buttonWidth = 64.0f;
        const float buttonHeight = 42.0f;

        auto calculatorButton = [&](const char* label, auto action)
        {
            if (ImGui::Button(
                label,
                ImVec2(buttonWidth, buttonHeight)
            ))
            {
                action();
            }
        };

        calculatorButton("C", []()
        {
            ClearCalculator();
        });

        ImGui::SameLine();

        calculatorButton("(", []()
        {
            // Parentheses are not implemented.
        });

        ImGui::SameLine();

        calculatorButton(")", []()
        {
            // Parentheses are not implemented.
        });

        ImGui::SameLine();

        calculatorButton("/", []()
        {
            ChooseOperator('/');
        });

        calculatorButton("7", []() { EnterDigit('7'); });
        ImGui::SameLine();
        calculatorButton("8", []() { EnterDigit('8'); });
        ImGui::SameLine();
        calculatorButton("9", []() { EnterDigit('9'); });
        ImGui::SameLine();
        calculatorButton("*", []() { ChooseOperator('*'); });

        calculatorButton("4", []() { EnterDigit('4'); });
        ImGui::SameLine();
        calculatorButton("5", []() { EnterDigit('5'); });
        ImGui::SameLine();
        calculatorButton("6", []() { EnterDigit('6'); });
        ImGui::SameLine();
        calculatorButton("-", []() { ChooseOperator('-'); });

        calculatorButton("1", []() { EnterDigit('1'); });
        ImGui::SameLine();
        calculatorButton("2", []() { EnterDigit('2'); });
        ImGui::SameLine();
        calculatorButton("3", []() { EnterDigit('3'); });
        ImGui::SameLine();
        calculatorButton("+", []() { ChooseOperator('+'); });

        calculatorButton("±", []()
        {
            if (std::strcmp(calculatorDisplay, "Error") != 0)
                SetDisplay(-CurrentNumber());

            startNewNumber = false;
        });

        ImGui::SameLine();

        calculatorButton("0", []() { EnterDigit('0'); });
        ImGui::SameLine();
        calculatorButton(".", []() { AddDecimalPoint(); });
        ImGui::SameLine();
        calculatorButton("=", []() { CalculateResult(); });
    }

    ImGui::End();
    ImGui::PopStyleVar();

    if (!isOpen)
    {
        appState.showApplicationOne = false;
        appState.applicationOneMinimized = false;
    }
}

// ============================================================
// MEDIA PLAYER APPLICATION
// ============================================================

void RenderMediaPlayer(AppState& appState)
{
    if (!appState.showApplicationTwo ||
        appState.applicationTwoMinimized)
    {
        return;
    }

    bool isOpen = true;

    ImGui::SetNextWindowSize(
        ImVec2(420.0f, 430.0f),
        ImGuiCond_FirstUseEver
    );

    ImGui::PushStyleVar(
        ImGuiStyleVar_WindowPadding,
        ImVec2(0.0f, 0.0f)
    );

    if (ImGui::Begin(
        "Media Player",
        nullptr,
        ImGuiWindowFlags_NoTitleBar
    ))
    {
        DrawCustomTitleBar(
            "Media Player",
            isOpen,
            appState.applicationTwoMinimized
        );

        ImGui::TextUnformatted("Now Playing");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::TextWrapped("%s", tracks[currentTrack]);
        ImGui::TextDisabled("%s", artists[currentTrack]);

        ImGui::Spacing();

        if (isPlaying)
        {
            playbackProgress += ImGui::GetIO().DeltaTime / 180.0f;

            if (playbackProgress >= 1.0f)
                ChangeTrack(1);
        }

        ImGui::ProgressBar(
            playbackProgress,
            ImVec2(-1.0f, 18.0f),
            ""
        );

        ImGui::Spacing();

        if (ImGui::Button("Previous", ImVec2(100.0f, 32.0f)))
            ChangeTrack(-1);

        ImGui::SameLine();

        if (ImGui::Button(
            isPlaying ? "Pause" : "Play",
            ImVec2(100.0f, 32.0f)
        ))
        {
            isPlaying = !isPlaying;
        }

        ImGui::SameLine();

        if (ImGui::Button("Next", ImVec2(100.0f, 32.0f)))
            ChangeTrack(1);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::TextUnformatted("Volume");

        ImGui::SliderFloat(
            "##media_volume",
            &volume,
            0.0f,
            1.0f,
            "%.0f%%"
        );

        ImGui::Spacing();
        ImGui::TextUnformatted("Playlist");

        if (ImGui::BeginListBox(
            "##media_playlist",
            ImVec2(-1.0f, 130.0f)
        ))
        {
            for (int i = 0; i < trackCount; ++i)
            {
                const bool selected = (currentTrack == i);

                char label[128];

                std::snprintf(
                    label,
                    sizeof(label),
                    "%s - %s",
                    tracks[i],
                    artists[i]
                );

                if (ImGui::Selectable(label, selected))
                {
                    currentTrack = i;
                    playbackProgress = 0.0f;
                }

                if (selected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndListBox();
        }
    }

    ImGui::End();
    ImGui::PopStyleVar();

    if (!isOpen)
    {
        appState.showApplicationTwo = false;
        appState.applicationTwoMinimized = false;
    }
}

// ============================================================
// SYSTEM INFORMATION APPLICATION
// ============================================================

void RenderSystemInformation(
    AppState& appState,
    const AppConfig& config
)
{
    (void)config;

    if (!appState.showSystemInformation ||
        appState.systemInformationMinimized)
    {
        return;
    }

    bool isOpen = true;

    ImGui::SetNextWindowSize(
        ImVec2(420.0f, 410.0f),
        ImGuiCond_FirstUseEver
    );

    ImGui::PushStyleVar(
        ImGuiStyleVar_WindowPadding,
        ImVec2(0.0f, 0.0f)
    );

    if (ImGui::Begin(
        "System Information",
        nullptr,
        ImGuiWindowFlags_NoTitleBar
    ))
    {
        DrawCustomTitleBar(
            "System Information",
            isOpen,
            appState.systemInformationMinimized
        );

        ImGui::TextUnformatted("Desktop System");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::TextUnformatted("Operating System");
        ImGui::SameLine(150.0f);
        ImGui::TextUnformatted("Desktop Mock-up");

        ImGui::TextUnformatted("Architecture");
        ImGui::SameLine(150.0f);
        ImGui::TextUnformatted("C++ / OpenGL");

        ImGui::TextUnformatted("Interface");
        ImGui::SameLine(150.0f);
        ImGui::TextUnformatted("Dear ImGui");

        ImGui::TextUnformatted("Window Library");
        ImGui::SameLine(150.0f);
        ImGui::TextUnformatted("GLFW");

        ImGui::Spacing();
        ImGui::Separator();

        ImGui::TextUnformatted("Application Status");

        ImGui::BulletText(
            "Calculator: %s",
            appState.showApplicationOne
                ? (appState.applicationOneMinimized
                    ? "Minimized"
                    : "Open")
                : "Closed"
        );

        ImGui::BulletText(
            "Media Player: %s",
            appState.showApplicationTwo
                ? (appState.applicationTwoMinimized
                    ? "Minimized"
                    : "Open")
                : "Closed"
        );

        ImGui::BulletText(
            "Task Manager: %s",
            appState.showTaskManager
                ? (appState.taskManagerMinimized
                    ? "Minimized"
                    : "Open")
                : "Closed"
        );

        ImGui::BulletText(
            "System Information: %s",
            appState.showSystemInformation
                ? (appState.systemInformationMinimized
                    ? "Minimized"
                    : "Open")
                : "Closed"
        );

        ImGui::Spacing();
        ImGui::Separator();

        ImGui::TextWrapped(
            "This application demonstrates system details "
            "in a simulated desktop environment."
        );
    }

    ImGui::End();
    ImGui::PopStyleVar();

    if (!isOpen)
    {
        appState.showSystemInformation = false;
        appState.systemInformationMinimized = false;
    }
}

// ============================================================
// RENDER ALL APPLICATIONS
// ============================================================

void RenderApplications(
    AppState& appState,
    const AppConfig& config
)
{
    RenderCalculator(appState);
    RenderMediaPlayer(appState);
    RenderSystemInformation(appState, config);
}
