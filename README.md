# Connect Four
A cross-platform Connect Four game for the terminal.

![screenshot](https://raw.githubusercontent.com/MaxMutantMayer/connect-four/master/res/screenshot.png)

## Features
* 3 Game Modes (Player vs. Player, Player vs. Computer, Computer vs. Computer)
* AI using [NegaMax algorithm with alpha beta pruning](https://en.wikipedia.org/wiki/Negamax#Negamax_with_alpha_beta_pruning) and [Zugzwang](https://en.wikipedia.org/wiki/Zugzwang) awareness
* Colored console output

## Requirements
* CMake
* Terminal with color support

## Build
### Unix & Unix-Like
```
git clone https://github.com/MaxMutantMayer/connect-four.git
cd connect-four
mkdir build && cd build
cmake ..
make
```
### Windows
#### CMake GUI and Visual Studio
1. Run CMake GUI and specify paths to source and build directory, e.g. `C:/Games/connect-four` and `C:/Games/connect-four/build`.
2. Click `Configure` and choose wished toolchain
3. Click `Generate` to create VS solution
4. Open solution and build `connect_four` project

#### Visual Studio only
VS 17 introduced built-in support for CMake. Simply open the project via `File -> Open -> CMake..` to build and run it.

## Usage
You can either use in-game menus or program arguments to specify settings.
```
Usage:
connect_four <Options>

Options:
    None: Use program with in-game menus
    --pvp: Play PlayerVsPlayer GameMode
    --pvc: Play PlayerVsComputer GameMode
    --cvp: Play ComputerVsPlayer GameMode
    --pvp: Play ComputerVsComputer GameMode
    --alphabeta: Disable alpha beta pruning
    --difficulty <easy|normal|hard>: Specify AI difficulty
```
