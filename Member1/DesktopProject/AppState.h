#pragma once

#include "Config.h"

struct AppState {
    bool showApplicationOne = false; 
    bool showApplicationTwo = false;
    bool showTaskManager = false;
    bool showSystemInformation = false;

    bool applicationOneMinimized = false;
    bool applicationTwoMinimized = false;
    bool taskManagerMinimized = false;
    bool systemInformationMinimized = false;

    bool shutdownRequested = false;
    std::array<ProcessInfo, 3> processes;
};
