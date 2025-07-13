/*
 * -----------------------------------------------------------------------------
 *
 * Project: 7 - Hangman Game
 *
 * -----------------------------------------------------------------------------
 *
 * Question:
 * Write a C program that allows a user to play the classic game of Hangman.
 *
 * The program should perform the following steps:
 * 1.  Have a predefined list of words.
 * 2.  Randomly select one word from the list for the player to guess.
 * 3.  Display the word to the player as a series of underscores (e.g., "_ _ _ _").
 * 4.  Keep track of the player's remaining guesses (e.g., start with 6).
 * 5.  Prompt the player to guess a letter.
 * 6.  If the guessed letter is in the word, reveal its position(s).
 * 7.  If the guessed letter is not in the word, decrement the remaining guesses
 * and draw a part of the hangman figure.
 * 8.  Keep track of letters already guessed to prevent repeated guesses.
 * 9.  The game ends when the player either correctly guesses the entire word
 * or runs out of guesses.
 * 10. Display a "You Win!" or "You Lose!" message at the end.
 *
 * Concepts Covered:
 * - String manipulation (strlen, strcmp, etc.).
 * - Random number generation (srand, rand).
 * - Arrays for storing words, guessed letters, and the display state.
 * - Game loop logic and state management (tracking guesses, win/loss conditions).
 * - Character input/output and validation.
 * - Using functions to handle specific tasks like drawing the hangman.
 *
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// --- Constants ---
#define MAX_WRONG_GUESSES 6
#define MAX_WORD_LENGTH 50

// --- Function Prototypes ---
void drawHangman(int wrong_guesses);
int isAlreadyGuessed(char letter, const char* guessed_letters);

int main() {
    // List of words for the game
    const char *word_list[] = {
        "programming", "computer", "developer", "hangman", "language",
        "keyboard", "software", "algorithm", "database", "interface"
    };
    const int word_count = sizeof(word_list) / sizeof(word_list[0]);

    // Seed the random number generator with the current time
    srand(time(NULL));

    // Select a random word from the list
    const char *secret_word = word_list[rand() % word_count];
    int word_length = strlen(secret_word);

    // --- Game State Variables ---
    char player_view[MAX_WORD_LENGTH];
    char guessed_letters[27] = ""; // 26 letters + null terminator
    int wrong_guesses = 0;
    int correct_guesses = 0;
    int guessed_count = 0;

    // Initialize the player's view with underscores
    for (int i = 0; i < word_length; i++) {
        player_view[i] = '_';
    }
    player_view[word_length] = '\0'; // Null-terminate the string

    printf("--- Welcome to Hangman! ---\n");

    // --- Main Game Loop ---
    while (wrong_guesses < MAX_WRONG_GUESSES && correct_guesses < word_length) {
        // Display current game state
        printf("\n");
        drawHangman(wrong_guesses);
        printf("\nWord: ");
        for (int i = 0; i < word_length; i++) {
            printf("%c ", player_view[i]);
        }
        printf("\nGuessed letters: %s\n", guessed_letters);
        printf("You have %d wrong guesses left.\n", MAX_WRONG_GUESSES - wrong_guesses);

        // Get player input
        char guess;
        printf("Enter your guess: ");
        scanf(" %c", &guess);
        while (getchar() != '\n'); // Clear input buffer
        guess = tolower(guess); // Standardize to lowercase

        // --- Input Validation ---
        if (!isalpha(guess)) {
            printf("Please enter a valid letter.\n");
            continue;
        }

        if (isAlreadyGuessed(guess, guessed_letters)) {
            printf("You have already guessed that letter. Try another.\n");
            continue;
        }

        // Add the new guess to the list of guessed letters
        guessed_letters[guessed_count++] = guess;
        guessed_letters[guessed_count] = '\0';

        // Check if the guess is in the word
        int found = 0;
        for (int i = 0; i < word_length; i++) {
            if (secret_word[i] == guess) {
                if (player_view[i] == '_') { // Only update if not already revealed
                    player_view[i] = guess;
                    correct_guesses++;
                }
                found = 1;
            }
        }

        if (found) {
            printf("Good guess!\n");
        } else {
            printf("Sorry, that letter is not in the word.\n");
            wrong_guesses++;
        }
    }

    // --- End of Game ---
    printf("\n\n=========================\n");
    if (correct_guesses == word_length) {
        printf("  CONGRATULATIONS! YOU WIN!\n");
    } else {
        drawHangman(wrong_guesses);
        printf("  GAME OVER! YOU LOSE!\n");
    }
    printf("The word was: %s\n", secret_word);
    printf("=========================\n");

    return 0;
}

/**
 * @brief Checks if a letter has already been guessed.
 * @param letter The letter to check.
 * @param guessed_letters A string containing all previously guessed letters.
 * @return 1 if already guessed, 0 otherwise.
 */
int isAlreadyGuessed(char letter, const char* guessed_letters) {
    for (int i = 0; i < strlen(guessed_letters); i++) {
        if (guessed_letters[i] == letter) {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Draws the hangman figure based on the number of wrong guesses.
 * @param wrong_guesses The count of incorrect guesses made by the player.
 */
void drawHangman(int wrong_guesses) {
    printf("  +---+\n");
    printf("  |   |\n");
    printf("  |   %c\n", (wrong_guesses >= 1) ? 'O' : ' ');
    printf("  |  %c%c%c\n", (wrong_guesses >= 3) ? '/' : ' ', (wrong_guesses >= 2) ? '|' : ' ', (wrong_guesses >= 4) ? '\\' : ' ');
    printf("  |  %c %c\n", (wrong_guesses >= 5) ? '/' : ' ', (wrong_guesses >= 6) ? '\\' : ' ');
    printf("  |\n");
    printf("=========\n");
}
