# Super Mario Bros Clone

## Project Timeline
- 📅 **Originally developed and completed:** January 2024 (Course Project, NED University) 
- 📤 **Uploaded to GitHub:** December 2025

A simple Super Mario Bros-like game implemented in C++ using the SFML (Simple and Fast Multimedia Library) framework. This project recreates classic platformer gameplay with Mario, enemies like Goombas, power-ups like Mushrooms, and various interactive elements such as bricks, pipes, and flags.

## Features

- **Player Character**: Control Mario with keyboard inputs (Left, Right, Up arrows).
- **Enemies**: Face off against Goombas that patrol platforms.
- **Power-ups**: Collect Mushrooms to grow Mario and gain new abilities.
- **Interactive Objects**: Break bricks, collect coins from blocks, navigate pipes, and reach the flag to complete levels.
- **Level Design**: Custom level loaded from a text file (`Levels/NEW MAP.txt`).
- **Audio**: Background music and sound effects for jumps, coin collection, and more.
- **Animations**: Smooth sprite animations for characters and objects.
- **Physics**: Gravity, collision detection, and movement mechanics.

## Screenshots

### Main Menu

![Main Menu](Output%20Images/Menu.png)

### Gameplay

![Gameplay](Output%20Images/Gameplay%201.png)

### End Game

![End Game](Output%20Images/end%20game.png)

## Prerequisites

- **C++ Compiler**: GCC (g++) or any compatible compiler that supports C++11 or later.
- **SFML Library**: Version 2.x. You need to download and set up SFML for your system.

### Installing SFML

1. Download SFML from the official website: [https://www.sfml-dev.org/download.php](https://www.sfml-dev.org/download.php).
2. Choose the appropriate version for your operating system (Windows in this case).
3. Extract the SFML files to a directory (e.g., `C:\SFML`).
4. Set up the include and library paths as needed (see compilation instructions below).

## Setup and Installation

1. **Clone or Download the Repository**:

   - Ensure you have the project files in a directory.

2. **Directory Structure**:

   - The executable and resources should be in the same folder.
   - Ensure the `Resources/` folder contains all assets (images, audio, fonts).
   - The level file is located in `Levels/NEW MAP.txt`.

3. **SFML DLLs**:

   - To run the game, you need the SFML dynamic link libraries (DLLs) in the same directory as the executable (`main.exe`).
   - Copy the following DLLs from your SFML installation's `bin/` folder to the project's root directory:
     - `openal32.dll`
     - `sfml-audio-2.dll`
     - `sfml-graphics-2.dll`
     - `sfml-system-2.dll`
     - `sfml-window-2.dll`
   - If you have debug versions, also include:
     - `sfml-audio-d-2.dll`
     - `sfml-graphics-d-2.dll`
     - `sfml-network-d-2.dll`
     - `sfml-system-d-2.dll`
     - `sfml-window-d-2.dll`

4. **Compilation**:

   - The project includes a `Makefile` for building on Windows with MinGW or similar.
   - Update the paths in `Makefile` to match your SFML installation:
     - Change `-IC:/path/to/sfml/include` to your SFML include directory.
     - Change `-LC:/path/to/sfml/lib` to your SFML lib directory.
   - Run `make all` to compile and link the project.
     - `make compile`: Compiles the source files.
     - `make link`: Links the object files with SFML libraries.
   - Alternatively, compile manually:
     ```
     g++ -I/path/to/sfml/include -c coding/main.cpp -o main.o
     g++ main.o -o main -L/path/to/sfml/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lopengl32
     ```

5. **Running the Game**:
   - Execute `main.exe`.
   - Press Enter in the main menu to start the game.
   - Use arrow keys: Left/Right to move, Up to jump.
   - Press Escape to exit.

## Project Structure

- `coding/main.cpp`: Main source file containing all game logic.
- `Resources/`: Folder for assets (images, audio, fonts).
  - `audio/`: General audio files.
  - `marioAudio/`: Mario-specific audio.
  - `Tilesheet.png`: Sprite sheet for tiles.
  - `Title.png`: Main menu image.
  - `mariosheet.png`: Mario sprite sheet.
  - `enemies_sprites.png`: Enemy sprites.
  - `arial.ttf`, `mario.ttf`: Fonts.
- `Levels/`: Contains level definition files (e.g., `NEW MAP.txt`).
- `Output Images/`: Screenshots and recordings.
- `Makefile`: Build script.

## Maintainability Notes

- **Code Organization**: All code is in a single file (`main.cpp`). For larger projects, consider splitting into multiple files (e.g., separate headers and sources for each class).
- **Dependencies**: Ensure SFML is properly linked. If issues arise, check that the correct library versions are used.
- **Level Editing**: Levels are defined in text files. Modify `Levels/NEW MAP.txt` to create new levels using the specified format.
- **Testing**: Run the game after changes to verify functionality. The `main.exe` is already compiled, but recompile after code edits.
- **Version Control**: Avoid committing large assets or DLLs to the repository. Use `.gitignore` to exclude them.

## License

This project is for educational purposes.

## Acknowledgments

- Inspired by the classic Super Mario Bros. game by Nintendo.
- Built using SFML for graphics and audio handling.
