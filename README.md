# OOP Board Games Arena — C++ Console Application

OOP Board Games Arena is a C++ console-based application that combines multiple board games into one integrated system.

The project was built using Object-Oriented Programming concepts and a shared board-game framework. It includes an interactive menu, player setup, Human vs Human and Human vs Computer modes, a scoreboard system, and several game-specific AI strategies.

## Project Overview

This project contains 14 different board games implemented in C++.  
Each game has its own rules, board structure, move validation, win/draw logic, and user interface.

The application starts from a main menu where players can:

- Configure player names
- Choose player types: Human or Computer
- Select one of the available games
- Play multiple games in the same session
- Track scores using an integrated scoreboard
- Reset scores or change players

## Main Features

- Integrated board games arena
- 14 playable board games
- Shared OOP framework for board, move, player, UI, and game manager
- Human vs Human mode
- Human vs Computer mode
- Smart AI logic in several games
- Random computer player support in selected games
- Scoreboard system
- Game result tracking
- Menu-driven console interface
- Visual Studio project setup
- Reusable and extendable game architecture

## Games Included

### 1. X-O Tic-Tac-Toe

Classic 3x3 Tic-Tac-Toe game where players try to form three marks in a row, column, or diagonal.

### 2. Numerical Tic-Tac-Toe

A 3x3 number-based Tic-Tac-Toe variation.

- Player 1 uses odd numbers: 1, 3, 5, 7, 9
- Player 2 uses even numbers: 2, 4, 6, 8
- A player wins by forming a line where the sum equals 15

### 3. SUS Game

A 3x3 word-based game where players try to form the sequence `S-U-S`.

- Players choose either `S` or `U`
- The game counts how many SUS sequences each player forms
- The player with the higher score wins

### 4. 5x5 Tic-Tac-Toe

A 5x5 Tic-Tac-Toe variation.

- Players place X and O on a 5x5 board
- The game ends after 24 moves
- The winner is the player who creates more three-in-a-row sequences

### 5. Misere Tic-Tac-Toe

A reversed Tic-Tac-Toe game.

- Making three in a row means losing
- Players try to avoid completing a winning line
- Includes computer decision-making using minimax logic

### 6. Four-in-a-Row

A Connect Four-style game played on a 6x7 board.

- Players drop pieces into columns
- The piece falls to the lowest available cell
- The goal is to form four in a row
- Includes a smart AI using heuristic evaluation, immediate win/block detection, move ordering, and search logic

### 7. 4x4 Moving Tic-Tac-Toe

A 4x4 movement-based Tic-Tac-Toe variation.

- Players start with movable tokens on the board
- Each move shifts a token to an adjacent empty cell
- The goal is to form three in a row
- Includes AI using board evaluation and minimax search

### 8. Word Tic-Tac-Toe

A 3x3 word-building game.

- Players place letters on the board
- The game checks rows, columns, and diagonals against a dictionary file
- A player wins by forming a valid 3-letter word
- Uses `dic.txt` as the dictionary source

### 9. Pyramid Tic-Tac-Toe

A Tic-Tac-Toe variation played on a pyramid-shaped board.

- The board follows a 1-3-5 layout
- Players aim to form three in a row
- Includes computer logic for winning moves and blocking opponent moves

### 10. Diamond Tic-Tac-Toe

A Tic-Tac-Toe variation played on a diamond-shaped 7x7 board.

- Only diamond-shaped cells are playable
- A player wins by forming both a line of 3 and a line of 4
- Includes AI move selection using win detection, blocking, and board evaluation

### 11. Infinity XO

A 3x3 Tic-Tac-Toe variation with disappearing moves.

- After a number of moves, the oldest mark is removed
- Players must win before their older moves disappear
- Computer player uses random move selection

### 12. Ultimate Tic-Tac-Toe

A large Tic-Tac-Toe game made of nine smaller Tic-Tac-Toe boards.

- Players play inside small boards
- Winning a small board marks the corresponding cell in the big board
- The goal is to win three small boards in a row
- Includes a custom game manager
- Includes AI using minimax with alpha-beta pruning and heuristic evaluation

### 13. Memory XO

A memory-based Tic-Tac-Toe variation.

- Marks are hidden after being placed
- Players must remember previous moves
- The goal is still to form three in a row
- Includes a computer player and AI helper logic

### 14. Obstacle Tic-Tac-Toe

A 6x6 Tic-Tac-Toe variation with obstacles.

- Players try to form four in a row
- After every two moves, random obstacles are added to the board
- Obstacles block available cells
- Computer player uses random move selection

## AI and Computer Players

The project includes different levels of computer logic depending on the game.

Some games use smart AI or heuristic-based decisions, including:

- Four-in-a-Row
- Ultimate Tic-Tac-Toe
- Misere Tic-Tac-Toe
- 4x4 Moving Tic-Tac-Toe
- Diamond Tic-Tac-Toe
- SUS
- Pyramid Tic-Tac-Toe
- Word Tic-Tac-Toe
- 5x5 Tic-Tac-Toe

Some games use random computer moves, including:

- Numerical Tic-Tac-Toe
- Infinity XO
- Obstacle Tic-Tac-Toe

## Tech Stack

- C++
- Object-Oriented Programming
- Visual Studio
- Console-based UI
- Game logic design
- Minimax algorithm
- Alpha-beta pruning
- Heuristic evaluation
- File handling
- STL containers

## OOP Concepts Used

- Classes and objects
- Inheritance
- Polymorphism
- Templates
- Encapsulation
- Dynamic memory allocation
- Abstract base classes
- Reusable game framework
- Custom game managers
- Separation between board logic and UI logic

## Project Structure

```text
oop-board-games-arena-cpp/
├── BoardGame_Classes.h
├── XO_Demo.cpp
├── XO_Classes.cpp
├── XO_Classes.h
├── NumericalTicTacToe.cpp
├── NumericalTicTacToe.h
├── SUS.cpp
├── SUS.h
├── SUS_AI.cpp
├── SUS_AI.h
├── 5x5 Tic Tac Toe.cpp
├── 5x5 Tic Tac Toe.h
├── Misere_Board.cpp
├── Misere_Board.h
├── FourInRow.cpp
├── FourInRow.h
├── T4x4_Classes.cpp
├── T4x4_Classes.h
├── Word_Tic_Tac_Toe.cpp
├── Word_Tic_Tac_Toe.h
├── pyramid.cpp
├── pyramid.h
├── Diamond.cpp
├── Diamond.h
├── Infinity_XO.cpp
├── Infinity_XO.h
├── Ultimate_TTT.cpp
├── Ultimate_TTT.h
├── Ultimate_TTT_AI.cpp
├── Ultimate_TTT_AI.h
├── Memory_XO.cpp
├── Memory_XO.h
├── Memory_XO_AI.h
├── Obstacle.cpp
├── Obstacle.h
├── dic.txt
├── BUILD_NOTES_AR.txt
├── BoardGames.slnx
├── BoardGames.vcxproj
├── BoardGames.vcxproj.filters
└── README.md
```

## Main Entry Point

The main application starts from:

```text
XO_Demo.cpp
```

This file contains the main menu, player setup, scoreboard, game selection system, and the integration logic for all games.

## How to Run

### Option 1: Run using Visual Studio

1. Clone the repository:

```bash
git clone https://github.com/Nagy-API/oop-board-games-arena-cpp.git
```

2. Open the project folder.

3. Open the Visual Studio solution/project file:

```text
BoardGames.slnx
```

or open:

```text
BoardGames.vcxproj
```

4. Build the project:

```text
Build > Rebuild Solution
```

5. Run the project from Visual Studio.

### Option 2: Compile manually using g++

If you want to compile manually, make sure all `.cpp` and `.h` files are in the same folder.

Example command:

```bash
g++ *.cpp -o BoardGames
```

Then run:

```bash
./BoardGames
```

On Windows:

```bash
BoardGames.exe
```

## Dictionary File

The Word Tic-Tac-Toe game depends on:

```text
dic.txt
```

Make sure `dic.txt` stays in the same directory as the executable when running the project.

## Build Notes

The repository includes:

```text
BUILD_NOTES_AR.txt
```

This file contains Arabic build notes for running the project in Visual Studio and avoiding common include/path issues.

## Files Not Included

The repository does not include generated build files such as:

```text
x64/
Debug/
Release/
.vs/
*.exe
*.obj
*.pdb
*.ilk
*.log
*.tlog
```

These files are ignored because they are generated automatically by Visual Studio and should not be stored in the repository.

## Skills Demonstrated

- C++ programming
- Object-Oriented Programming
- Game development fundamentals
- Board game logic design
- AI move selection
- Minimax algorithm
- Alpha-beta pruning
- Heuristic evaluation
- File handling
- STL containers
- Debugging and testing
- Menu-driven console applications
- Team-based academic project development
- Code organization across multiple files

## Future Improvements

- Add a graphical user interface
- Add better AI difficulty levels
- Add save/load game functionality
- Add unit tests for each game
- Improve memory management using smart pointers
- Add more detailed documentation for each game
- Add cross-platform CMake support
- Add screenshots or demo videos

## Notes

This project was created for academic learning purposes.

The project demonstrates how multiple board games can be implemented using a shared OOP framework while keeping each game’s rules and logic separated into its own files.
