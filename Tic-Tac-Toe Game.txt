/*
 * -----------------------------------------------------------------------------
 *
 * Project: 9 - Tic-Tac-Toe Game
 *
 * -----------------------------------------------------------------------------
 *
 * Question:
 * Write a C program that allows two players to play a game of Tic-Tac-Toe
 * on the command line.
 *
 * The program's functionality should include:
 * 1.  Displaying a 3x3 grid for the game board.
 * 2.  Alternating turns between Player X and Player O.
 * 3.  Prompting the current player to enter their move by choosing a
 * number from 1 to 9 (corresponding to the grid positions).
 * 4.  Validating player input to ensure the chosen spot is valid and not
 * already taken.
 * 5.  After each move, checking for a win condition (three of the same
 * marks in a row, column, or diagonal).
 * 6.  Checking for a draw condition (if the board is full and no one has won).
 * 7.  Declaring the winner or announcing a draw when the game ends.
 * 8.  Offering the players the option to play another game.
 *
 * Concepts Covered:
 * - Using 2D arrays to represent a game board.
 * - Implementing turn-based game logic.
 * - Input validation and error handling.
 * - Writing functions to check for complex game states (win/draw conditions).
 * - Structuring a game loop that can be reset for multiple playthroughs.
 *
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// --- Global Board ---
// Using numbers '1' through '9' as placeholders for empty cells.
char board[3][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'}
};

// --- Function Prototypes ---
void displayBoard();
int checkWin();
void resetBoard();

int main() {
    int player = 1;
    int choice;
    char mark; // 'X' or 'O'
    char play_again;

    do {
        resetBoard();
        int game_status;

        // --- Main Game Loop ---
        do {
            displayBoard();
            player = (player % 2) ? 1 : 2; // Alternate between players
            mark = (player == 1) ? 'X' : 'O';

            printf("Player %d (%c), enter a number: ", player, mark);
            scanf("%d", &choice);
            while(getchar() != '\n'); // Clear buffer

            // Map the user's choice (1-9) to board coordinates
            int row = (choice - 1) / 3;
            int col = (choice - 1) % 3;

            // --- Move Validation ---
            if (choice < 1 || choice > 9) {
                printf("Invalid move. Please choose a number between 1 and 9.\n");
                player--; // Keep it the same player's turn
            } else if (board[row][col] == 'X' || board[row][col] == 'O') {
                printf("That spot is already taken. Try again.\n");
                player--; // Keep it the same player's turn
            } else {
                board[row][col] = mark;
            }

            game_status = checkWin();
            player++;

        } while (game_status == -1); // Continue if game is not over

        displayBoard();

        // --- Announce Result ---
        if (game_status == 1) {
            printf("==> Player %d (%c) wins! <==\n", --player, mark);
        } else {
            printf("==> Game is a draw! <==\n");
        }

        printf("\nDo you want to play again? (y/n): ");
        scanf(" %c", &play_again);
        while(getchar() != '\n'); // Clear buffer

    } while (play_again == 'y' || play_again == 'Y');

    printf("Thanks for playing!\n");
    return 0;
}

/**
 * @brief Displays the Tic-Tac-Toe board in its current state.
 */
void displayBoard() {
    // system("clear"); // Use "cls" on Windows
    printf("\n\n--- Tic Tac Toe ---\n\n");
    printf("Player 1 (X)  -  Player 2 (O)\n\n");

    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c \n", board[0][0], board[0][1], board[0][2]);
    printf("_____|_____|_____\n");
    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c \n", board[1][0], board[1][1], board[1][2]);
    printf("_____|_____|_____\n");
    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c \n", board[2][0], board[2][1], board[2][2]);
    printf("     |     |     \n\n");
}

/**
 * @brief Checks the board for a win, draw, or ongoing game.
 * @return 1 if there is a winner, 0 for a draw, -1 if the game is still in progress.
 */
int checkWin() {
    // Check rows
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) return 1;
    }
    // Check columns
    for (int i = 0; i < 3; i++) {
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) return 1;
    }
    // Check diagonals
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) return 1;
    if (board[0][2] == board[1][1] && board[1][0] == board[0][2]) return 1;

    // Check for draw (if all cells are filled)
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != 'X' && board[i][j] != 'O') {
                return -1; // Game is still in progress
            }
        }
    }

    return 0; // Game is a draw
}

/**
 * @brief Resets the board to its initial state for a new game.
 */
void resetBoard() {
    char initial_board[3][3] = {
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'}
    };
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = initial_board[i][j];
        }
    }
}
