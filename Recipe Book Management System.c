/*
 * -----------------------------------------------------------------------------
 *
 * Project: 14 - Recipe Book Management System
 *
 * -----------------------------------------------------------------------------
 *
 * Question:
 * Create a C program that acts as a digital recipe book. The user should be
 * able to add, view, and search for recipes.
 *
 * The system must support the following operations:
 * 1.  Add a new recipe. Each recipe includes:
 * - A title.
 * - A list of ingredients (as a single block of text).
 * - The cooking instructions (as a single block of text).
 * 2.  Display all saved recipes in a well-formatted manner.
 * 3.  Search for a recipe by a keyword in its title.
 * 4.  Save the entire recipe book to a file ("recipes.dat").
 * 5.  Load the recipe book from the file upon startup.
 *
 * Concepts Covered:
 * - Handling large, multi-line string inputs.
 * - String searching with strstr().
 * - Storing and retrieving more complex text-based data structures.
 * - File I/O and menu-driven application structure.
 *
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Constants ---
#define MAX_RECIPES 50
#define MAX_TEXT_LENGTH 2048 // For ingredients and instructions
#define FILENAME "recipes.dat"

// --- Data Structures ---
struct Recipe {
    char title[100];
    char ingredients[MAX_TEXT_LENGTH];
    char instructions[MAX_TEXT_LENGTH];
};

// --- Global Data ---
struct Recipe recipe_book[MAX_RECIPES];
int recipe_count = 0;

// --- Function Prototypes ---
void addRecipe();
void displayAllRecipes();
void searchRecipe();
void readMultiLineInput(char* buffer, int size);
void saveData();
void loadData();

int main() {
    loadData();
    int choice;

    while (1) {
        printf("\n\n--- Digital Recipe Book ---\n");
        printf("1. Add New Recipe\n");
        printf("2. Display All Recipes\n");
        printf("3. Search for a Recipe (by Title)\n");
        printf("4. Save and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // Clear input buffer

        switch (choice) {
            case 1: addRecipe(); break;
            case 2: displayAllRecipes(); break;
            case 3: searchRecipe(); break;
            case 4:
                saveData();
                printf("Recipe book saved. Bon appétit!\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

/**
 * @brief Adds a new recipe to the book.
 */
void addRecipe() {
    if (recipe_count >= MAX_RECIPES) {
        printf("Recipe book is full.\n");
        return;
    }

    struct Recipe *recipe = &recipe_book[recipe_count];
    printf("\n--- Add New Recipe ---\n");

    printf("Enter Recipe Title: ");
    fgets(recipe->title, sizeof(recipe->title), stdin);
    recipe->title[strcspn(recipe->title, "\n")] = 0;

    printf("\nEnter Ingredients (type 'END' on a new line to finish):\n");
    readMultiLineInput(recipe->ingredients, sizeof(recipe->ingredients));

    printf("\nEnter Instructions (type 'END' on a new line to finish):\n");
    readMultiLineInput(recipe->instructions, sizeof(recipe->instructions));

    recipe_count++;
    printf("Recipe for '%s' added successfully!\n", recipe->title);
}

/**
 * @brief Displays all recipes in the book.
 */
void displayAllRecipes() {
    if (recipe_count == 0) {
        printf("\nYour recipe book is empty.\n");
        return;
    }
    printf("\n\n--- All Saved Recipes ---\n");
    for (int i = 0; i < recipe_count; i++) {
        struct Recipe r = recipe_book[i];
        printf("\n==================================================\n");
        printf("  %s\n", r.title);
        printf("==================================================\n\n");
        printf("--- Ingredients ---\n%s\n", r.ingredients);
        printf("--- Instructions ---\n%s\n", r.instructions);
        printf("==================================================\n");
    }
}

/**
 * @brief Searches for recipes by a keyword in their title.
 */
void searchRecipe() {
    if (recipe_count == 0) {
        printf("\nRecipe book is empty, nothing to search.\n");
        return;
    }

    char keyword[100];
    printf("Enter a keyword to search for in recipe titles: ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = 0; // Remove newline

    int found_count = 0;
    printf("\n--- Search Results for '%s' ---\n", keyword);
    for (int i = 0; i < recipe_count; i++) {
        // strstr finds the first occurrence of a substring.
        // It's case-sensitive.
        if (strstr(recipe_book[i].title, keyword) != NULL) {
            struct Recipe r = recipe_book[i];
            printf("\n==================================================\n");
            printf("  %s\n", r.title);
            printf("==================================================\n\n");
            printf("--- Ingredients ---\n%s\n", r.ingredients);
            printf("--- Instructions ---\n%s\n", r.instructions);
            printf("==================================================\n");
            found_count++;
        }
    }

    if (found_count == 0) {
        printf("No recipes found with that keyword.\n");
    }
}

/**
 * @brief Helper function to read multiple lines of text from the user.
 * @param buffer The character array to store the input.
 * @param size The maximum size of the buffer.
 */
void readMultiLineInput(char* buffer, int size) {
    char line[200];
    buffer[0] = '\0'; // Start with an empty string

    while (fgets(line, sizeof(line), stdin)) {
        // Use strncmp to safely compare the first 3 characters
        if (strncmp(line, "END", 3) == 0) {
            break;
        }
        if (strlen(buffer) + strlen(line) < size) {
            strcat(buffer, line);
        } else {
            printf("Input is too long, cannot add more text.\n");
            break;
        }
    }
}

/**
 * @brief Saves the recipe book to a file.
 */
void saveData() {
    FILE *fp = fopen(FILENAME, "wb");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    fwrite(recipe_book, sizeof(struct Recipe), recipe_count, fp);
    fclose(fp);
}

/**
 * @brief Loads the recipe book from a file.
 */
void loadData() {
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        return;
    }
    recipe_count = fread(recipe_book, sizeof(struct Recipe), MAX_RECIPES, fp);
    fclose(fp);
    if (recipe_count > 0) {
        printf("Loaded %d recipe(s).\n", recipe_count);
    }
}
