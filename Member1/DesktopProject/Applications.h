
#pragma once

#include "AppState.h"
#include "Config.h"

void RenderCalculator(AppState& appState);
void RenderMediaPlayer(AppState& appState);
void RenderSystemInformation(
    AppState& appState,
    const AppConfig& config
);

void RenderApplications(
    AppState& appState,
    const AppConfig& config
);

