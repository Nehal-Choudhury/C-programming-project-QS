/*
 * -----------------------------------------------------------------------------
 *
 * Project: 17 - Simple File Encryptor/Decryptor
 *
 * -----------------------------------------------------------------------------
 *
 * Question:
 * Write a C program that can encrypt and decrypt text files using a simple
 * substitution cipher, specifically the Caesar cipher.
 *
 * The program should provide the following functionality:
 * 1.  A menu to choose between Encryption, Decryption, or Exit.
 * 2.  Prompt the user for an input file name and an output file name.
 * 3.  Prompt for an integer key (the shift value, e.g., 1-25).
 * 4.  For encryption, it should read the input file character by character,
 * shift only the alphabetic characters forward by the key, and write the
 * result (including non-alphabetic characters) to the output file.
 * 5.  The shift should "wrap around" the alphabet (e.g., 'Z' with a key of 3
 * becomes 'C').
 * 6.  For decryption, it should perform the reverse operation, shifting
 * characters backward by the key.
 * 7.  The program must handle potential file opening errors.
 *
 * Concepts Covered:
 * - File I/O with text files using fgetc() and fputc().
 * - Character manipulation based on ASCII values.
 * - Implementing a simple cryptographic algorithm.
 * - Using the modulo operator (%) for alphabet wrapping.
 * - Error handling for file operations.
 *
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// --- Function Prototypes ---
void processFile(int mode); // 1 for encrypt, -1 for decrypt

int main() {
    int choice;

    while (1) {
        printf("\n\n--- File Encryptor/Decryptor ---\n");
        printf("1. Encrypt a File\n");
        printf("2. Decrypt a File\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // Clear input buffer

        switch (choice) {
            case 1:
                processFile(1); // Encrypt mode
                break;
            case 2:
                processFile(-1); // Decrypt mode
                break;
            case 3:
                printf("Exiting program.\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

/**
 * @brief Handles the file processing for both encryption and decryption.
 * @param mode 1 for encryption (shift forward), -1 for decryption (shift backward).
 */
void processFile(int mode) {
    char input_filename[100];
    char output_filename[100];
    int key;
    FILE *inputFile, *outputFile;

    const char* operation = (mode == 1) ? "Encrypt" : "Decrypt";

    printf("\n--- File %ssion ---\n", operation);
    printf("Enter input file name: ");
    scanf("%99s", input_filename);
    printf("Enter output file name: ");
    scanf("%99s", output_filename);
    printf("Enter the key (a number from 1 to 25): ");
    scanf("%d", &key);
    while (getchar() != '\n'); // Clear buffer

    if (key < 1 || key > 25) {
        printf("Invalid key. Please use a number between 1 and 25.\n");
        return;
    }

    // --- File Opening ---
    inputFile = fopen(input_filename, "r");
    if (inputFile == NULL) {
        perror("Error opening input file"); // perror provides a system error message
        return;
    }

    outputFile = fopen(output_filename, "w");
    if (outputFile == NULL) {
        perror("Error opening output file");
        fclose(inputFile); // Close the already opened input file
        return;
    }

    // --- Character Processing Loop ---
    int ch;
    while ((ch = fgetc(inputFile)) != EOF) {
        char processed_ch = ch;

        if (isalpha(ch)) {
            char base = isupper(ch) ? 'A' : 'a';
            // The core Caesar cipher logic:
            // 1. (ch - base): Get the 0-25 index of the letter.
            // 2. (+ key * mode): Add or subtract the key.
            // 3. (+ 26): Add 26 to handle negative results in decryption (e.g., 'A' - 3 = -2).
            // 4. (% 26): Wrap the result around the 26-letter alphabet.
            // 5. (+ base): Convert the 0-25 index back to an ASCII character.
            processed_ch = (ch - base + (key * mode) + 26) % 26 + base;
        }

        // Write the processed (or original non-alphabetic) character to the output file.
        fputc(processed_ch, outputFile);
    }

    // --- Cleanup ---
    fclose(inputFile);
    fclose(outputFile);

    printf("\nFile has been %sed successfully!\n", operation);
    printf("Input: %s\n", input_filename);
    printf("Output: %s\n", output_filename);
}
