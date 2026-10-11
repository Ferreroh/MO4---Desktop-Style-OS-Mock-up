#pragma once

#include "AppState.h"

void RenderDesktop(const AppConfig& config);
void RenderWallpaper(const AppConfig& config);
void RenderClock(const AppConfig& config);
void RenderPowerButton(const AppConfig& config, AppState& state);
