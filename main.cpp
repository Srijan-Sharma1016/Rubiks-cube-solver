// main.cpp

#include <iostream>     // For input/output operations (cout, cin)
#include <vector>       // For std::vector
#include <string>       // For std::string
#include <limits>       // For std::numeric_limits (input validation)
#include <chrono>       // For timing solver execution
#include <algorithm>    // For std::transform (string to uppercase)
#include <fstream>      // For file handling (checking Pattern DB)

// Include your base RubiksCube header
#include "Model/RubiksCube.h"
#include "Solver/IDAstarSolver.h"
#include "PatternDatabases/CornerDBMaker.h"

// Include the specific Rubik's Cube implementation you want to use
// For solvers, RubiksCube1dArray is generally preferred due to its simpler hashing.
// NOTE: If you get "Multiple Definition" errors during 'make', change .cpp to .h below
#include "Model/RubiksCube1dArray.cpp" 
// #include "Model/RubiksCube3dArray.h"

// Include your solver headers
#include "Solver/BFSSolver.h"
#include "Solver/DFSSolver.h"
#include "Solver/IDDFSSolver.h"

// --- Type Aliases ---
using CubeType = RubiksCube1dArray; // Use RubiksCube1dArray by default
using HashType = Hash1d; // Use Hash1d for RubiksCube1dArray

// --- Constants ---
const int MAX_SHUFFLE_MOVES = 10; // Increased limit for IDA* (it can handle much more!)
const int IDDFS_MAX_DEPTH_DEFAULT = 7; 

// --- Function Prototypes ---
void clearScreen();
void pressEnterToContinue();
void displayMainMenu();
int getValidatedIntegerInput(const std::string& prompt, int min, int max);
bool parseMoveInput(const std::string& moveStr, RubiksCube::MOVE& outMove);

void handleDisplayCube(const CubeType& cube);
void handleShuffleCube(CubeType& cube);
void handleResetCube(CubeType& cube);
void handleApplySingleMove(CubeType& cube);
void handleCheckSolved(const CubeType& cube);
void handleBFSSolve(CubeType& cube);
void handleIDDFSSolve(CubeType& cube);
void handleIDAstarSolve(CubeType& cube); // <-- NEW PROTOYPE

// --- Main Function ---
int main()
{
    CubeType myCube; // Initialize a solved cube
    int choice;

    std::cout << "Welcome to the Rubik's Cube Solver!\n";
    std::cout << "This application demonstrates various cube operations and solving algorithms.\n";

    do
    {
        clearScreen();
        handleDisplayCube(myCube); // Always display the current cube state
        displayMainMenu();
        
        // Updated max input to 7
        choice = getValidatedIntegerInput("Enter your choice: ", 0, 7); 

        switch (choice)
        {
        case 1: handleShuffleCube(myCube);
            break;
        case 2: handleResetCube(myCube);
            break;
        case 3: handleApplySingleMove(myCube);
            break;
        case 4: handleCheckSolved(myCube);
            break;
        case 5: handleBFSSolve(myCube);
            break;
        case 6: handleIDDFSSolve(myCube);
            break;
        case 7: handleIDAstarSolve(myCube); // <-- NEW OPTION
            break;
        case 0: std::cout << "Exiting Rubik's Cube Solver. Goodbye!\n";
            break;
        default: std::cout << "Invalid choice. Please try again.\n";
            break;
        }

        if (choice != 0)
        {
            pressEnterToContinue();
        }
    }
    while (choice != 0);

    return 0;
}

// --- Function Implementations ---

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pressEnterToContinue()
{
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string dummy; 
    std::getline(std::cin, dummy);
}

void displayMainMenu()
{
    std::cout << "\n--- Main Menu ---\n";
    std::cout << "1. Shuffle Cube (Random Moves)\n";
    std::cout << "2. Reset Cube (to Solved State)\n";
    std::cout << "3. Apply a Single Move (e.g., U, L', F2)\n";
    std::cout << "4. Check if Cube is Solved\n";
    std::cout << "5. Solve using BFS (Breadth-First Search) [Fast for <7 moves]\n";
    std::cout << "6. Solve using IDDFS (Iterative Deepening DFS) [Good for <10 moves]\n";
    std::cout << "7. Solve using IDA* (Uses Pattern Database) [Fastest!]\n"; // <-- NEW
    std::cout << "0. Exit\n";
    std::cout << "-----------------\n";
}

int getValidatedIntegerInput(const std::string& prompt, int min, int max)
{
    int value;
    while (true)
    {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail() || value < min || value > max)
        {
            std::cout << "Invalid input. Please enter a number between " << min << " and " << max << ".\n";
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        }
        else
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            return value;
        }
    }
}

bool parseMoveInput(const std::string& moveStr, RubiksCube::MOVE& outMove)
{
    std::string upperMoveStr = moveStr;
    std::transform(upperMoveStr.begin(), upperMoveStr.end(), upperMoveStr.begin(), ::toupper);

    if (upperMoveStr == "U") { outMove = RubiksCube::MOVE::U; return true; }
    if (upperMoveStr == "U'") { outMove = RubiksCube::MOVE::UPRIME; return true; }
    if (upperMoveStr == "U2") { outMove = RubiksCube::MOVE::U2; return true; }
    if (upperMoveStr == "L") { outMove = RubiksCube::MOVE::L; return true; }
    if (upperMoveStr == "L'") { outMove = RubiksCube::MOVE::LPRIME; return true; }
    if (upperMoveStr == "L2") { outMove = RubiksCube::MOVE::L2; return true; }
    if (upperMoveStr == "F") { outMove = RubiksCube::MOVE::F; return true; }
    if (upperMoveStr == "F'") { outMove = RubiksCube::MOVE::FPRIME; return true; }
    if (upperMoveStr == "F2") { outMove = RubiksCube::MOVE::F2; return true; }
    if (upperMoveStr == "R") { outMove = RubiksCube::MOVE::R; return true; }
    if (upperMoveStr == "R'") { outMove = RubiksCube::MOVE::RPRIME; return true; }
    if (upperMoveStr == "R2") { outMove = RubiksCube::MOVE::R2; return true; }
    if (upperMoveStr == "B") { outMove = RubiksCube::MOVE::B; return true; }
    if (upperMoveStr == "B'") { outMove = RubiksCube::MOVE::BPRIME; return true; }
    if (upperMoveStr == "B2") { outMove = RubiksCube::MOVE::B2; return true; }
    if (upperMoveStr == "D") { outMove = RubiksCube::MOVE::D; return true; }
    if (upperMoveStr == "D'") { outMove = RubiksCube::MOVE::DPRIME; return true; }
    if (upperMoveStr == "D2") { outMove = RubiksCube::MOVE::D2; return true; }

    return false; 
}

void handleDisplayCube(const CubeType& cube)
{
    std::cout << "--- Current Cube State ---\n";
    cube.print();
    std::cout << "--------------------------\n";
}

void handleShuffleCube(CubeType& cube)
{
    int numMoves = getValidatedIntegerInput(
        "Enter number of random moves to shuffle (1-" + std::to_string(MAX_SHUFFLE_MOVES) + "): ",
        1, MAX_SHUFFLE_MOVES
    );
    std::cout << "Shuffling cube with " << numMoves << " random moves...\n";
    std::vector<RubiksCube::MOVE> shuffleMoves = cube.randomShuffleCube(numMoves);
    std::cout << "Shuffle sequence applied: ";
    for (const auto& move : shuffleMoves)
    {
        std::cout << RubiksCube::getMove(move) << " ";
    }
    std::cout << "\nCube shuffled!\n";
}

void handleResetCube(CubeType& cube)
{
    cube = CubeType(); 
    std::cout << "Cube has been reset to its solved state.\n";
}

void handleApplySingleMove(CubeType& cube)
{
    std::string moveStr;
    std::cout << "Enter a single move (e.g., U, L', F2, D): ";
    std::cin >> moveStr; 

    RubiksCube::MOVE move;

    if (parseMoveInput(moveStr, move)) 
    {
        cube.move(move);
        std::cout << "Applied move: " << RubiksCube::getMove(move) << "\n";
    }
    else 
    {
        std::cout << "Invalid move entered. Please use standard notation (U, U', U2, L, L', L2, etc.).\n";
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void handleCheckSolved(const CubeType& cube)
{
    if (cube.isSolved())
    {
        std::cout << "The cube is currently SOLVED!\n";
    }
    else
    {
        std::cout << "The cube is NOT solved yet.\n";
    }
}

void handleBFSSolve(CubeType& cube)
{
    if (cube.isSolved())
    {
        std::cout << "Cube is already solved. No need for BFS.\n";
        return;
    }

    std::cout << "Attempting to solve cube using BFS...\n";
    BFSSolver<CubeType, HashType> solver(cube);

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<RubiksCube::MOVE> solution = solver.solve();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    if (solver.rubiksCube.isSolved())
    {
        cube = solver.rubiksCube; 
        std::cout << "\nBFS Solution Found!\n";
        std::cout << "Solution Path (" << solution.size() << " moves): ";
        for (const auto& move : solution)
        {
            std::cout << RubiksCube::getMove(move) << " ";
        }
        std::cout << "\nTime taken: " << duration.count() << " seconds.\n";
    }
    else
    {
        std::cout << "\nBFS could not find a solution (might be too deep or memory limits reached).\n";
        std::cout << "Time taken: " << duration.count() << " seconds.\n";
    }
}

void handleIDDFSSolve(CubeType& cube)
{
    if (cube.isSolved())
    {
        std::cout << "Cube is already solved. No need for IDDFS.\n";
        return;
    }

    int depthLimit = getValidatedIntegerInput(
        "Enter maximum search depth for IDDFS (e.g., 7 for quick tests, up to 10-12 for deeper): ",
        1, 12 
    );

    std::cout << "Attempting to solve cube using IDDFS (max depth: " << depthLimit << ")...\n";
    IDDFSSolver<CubeType, HashType> solver(cube, depthLimit);

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<RubiksCube::MOVE> solution = solver.solve();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    if (solver.rubiksCube.isSolved())
    {
        cube = solver.rubiksCube; 
        std::cout << "\nIDDFS Solution Found!\n";
        std::cout << "Solution Path (" << solution.size() << " moves): ";
        for (const auto& move : solution)
        {
            std::cout << RubiksCube::getMove(move) << " ";
        }
        std::cout << "\nTime taken: " << duration.count() << " seconds.\n";
    }
    else
    {
        std::cout << "\nIDDFS could not find a solution within depth " << depthLimit << ".\n";
        std::cout << "Time taken: " << duration.count() << " seconds.\n";
    }
}

// Handles solving using IDA* (Requires a Pattern Database file)
void handleIDAstarSolve(CubeType& cube)
{
    if (cube.isSolved()) {
        std::cout << "Cube is already solved.\n";
        return;
    }

    // Define the file path for the pattern database
    std::string fileName = "/Users/srijan/Documents/Rubik-Cube-Solver/Databases/cornerDepth5V1.txt";

    std::cout << "Checking for Pattern Database at: " << fileName << "...\n";
    
    // Check if file exists (basic check), if not, create it.
    std::ifstream f(fileName.c_str());
    if (!f.good()) {
        std::cout << "Database not found! Creating it now (this happens only once)...\n";
        std::cout << "Please wait, this might take 10-20 seconds...\n";
        
        // Create the database (Depth 0x99 is a hex limit, standard for this repo)
        CornerDBMaker dbMaker(fileName, 0x99);
        dbMaker.bfsAndStore(); 
        std::cout << "Database created successfully!\n";
    } else {
        std::cout << "Database found. Loading...\n";
    }

    std::cout << "Attempting to solve using IDA*...\n";
    
    // Initialize IDA* with the cube and the database file
    IDAstarSolver<CubeType, HashType> solver(cube, fileName);

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<RubiksCube::MOVE> solution = solver.solve();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    if (solver.rubiksCube.isSolved()) {
        cube = solver.rubiksCube; // Update main cube
        std::cout << "\nIDA* Solution Found!\n";
        std::cout << "Solution Path (" << solution.size() << " moves): ";
        for (const auto& move : solution) std::cout << RubiksCube::getMove(move) << " ";
        std::cout << "\nTime taken: " << duration.count() << " seconds.\n";
    } else {
        std::cout << "IDA* could not find a solution.\n";
    }
}