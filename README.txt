DESKTOP MOCK-UP

HOW IT WORKS
The program opens a desktop with a background color, a live clock,
and a PWR button. Click PWR to close the program safely.

You can change the window size, background color, and clock position
in config.ini before running. Missing or invalid values use defaults.

AppState.h stores the shared controls for opening, minimizing,
and closing windows. Other members will add the apps, Task Manager,
and taskbar. The process values are sample data.

HOW TO RUN ON MAC (VS CODE)
1. Install Xcode Command Line Tools, CMake, and Git.
2. Open the DesktopProject folder in VS Code.
3. Install the C/C++ and CMake Tools extensions by Microsoft.
4. Press Command + Shift + P.
5. Select CMake: Select a Kit, then choose Clang.
6. Select CMake: Configure and wait.
7. Select CMake: Build and wait.
8. Select CMake: Run Without Debugging.
9. Choose DesktopMockup if asked.
10. Click PWR to close.

HOW TO RUN ON WINDOWS (VISUAL STUDIO)
1. Install Visual Studio with Desktop development with C++,
   including C++ CMake tools for Windows. Install Git too.
2. Open Visual Studio and select Open a local folder.
3. Select the DesktopProject folder.
4. Wait for CMake to finish setting up.
5. Select DesktopMockup as the startup target.
6. Click the green Run button.
7. Click PWR to close.

NOTES
- Extract the ZIP before opening the project.
- Internet is needed during the first setup.
- Use CMake 3.20 or newer and a C++17 compiler.
- Set the run working directory to the DesktopProject folder
  so the program reads the correct config.ini.
- Edit settings before recording, not during the demonstration.

CONFIGURATION
Use one KEY=VALUE per line. Keys and choices must be uppercase.
Lines starting with # or ; are comments.

Allowed values:
- WINDOW_WIDTH: 640-3840.
- WINDOW_HEIGHT: 480-2160.
- TASKBAR_POSITION: TOP or BOTTOM.
- CLOCK_CORNER: TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, or BOTTOM_RIGHT.
- WALLPAPER_RED, WALLPAPER_GREEN, WALLPAPER_BLUE: 0-255.
- PROCESS_1_NAME through PROCESS_3_NAME: nonempty text, up to 64 bytes.
- PROCESS_1_CPU through PROCESS_3_CPU: 0-100 percent.
- PROCESS_1_MEMORY through PROCESS_3_MEMORY: 0-1048576 MB.

Window sizes and color values must be whole numbers.
TASKBAR_POSITION currently controls the PWR button position.