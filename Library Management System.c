/*
 * -----------------------------------------------------------------------------
 *
 * Project: 2 - Simple Library Management System
 *
 * -----------------------------------------------------------------------------
 *
 * Question:
 * Write a C program to create a simple library management system.
 * The program should be able to perform the following operations:
 *
 * 1.  Add a new book to the library. Each book should have a title,
 * author, and a unique ID.
 * 2.  Display a list of all books currently in the library.
 * 3.  Search for a specific book by its ID.
 * 4.  Save the library records to a file (e.g., "library.dat").
 * 5.  Load the library records from the file when the program starts.
 *
 * The program should present a menu to the user to choose these operations
 * and should continue to run until the user chooses to exit.
 *
 * Concepts Covered:
 * - Structs for data modeling (representing a book).
 * - Arrays of structs to hold the library collection.
 * - Functions for modular code (add, display, search).
 * - File I/O for data persistence (fopen, fwrite, fread, fclose).
 * - A menu-driven user interface using a while loop and switch statement.
 *
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h> // For exit()
#include <string.h> // For strcpy() and strcmp()

// Define a constant for the maximum number of books the library can hold.
#define MAX_BOOKS 100
// Define the filename for storing the library data.
#define FILENAME "library.dat"

// --- Data Structure for a Book ---
// Using a struct to group together related data for a single book.
struct Book {
    int id;
    char title[100];
    char author[100];
};

// --- Global Variables ---
// Array of Book structs to hold all the books in the library.
struct Book library[MAX_BOOKS];
// Variable to keep track of the current number of books in the library.
int book_count = 0;

// --- Function Prototypes ---
// Declaring the functions we will define later in the code.
void addBook();
void displayBooks();
void searchBook();
void saveLibraryToFile();
void loadLibraryFromFile();

int main() {
    // Load existing book data from the file at the start of the program.
    loadLibraryFromFile();

    int choice;

    // --- Main Menu Loop ---
    // The loop continues until the user chooses to exit (option 5).
    while (1) {
        printf("\n--- Library Management System ---\n");
        printf("1. Add a New Book\n");
        printf("2. Display All Books\n");
        printf("3. Search for a Book (by ID)\n");
        printf("4. Save and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        // Consume the newline character left by scanf
        while(getchar() != '\n');

        // --- Process User's Choice ---
        switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                displayBooks();
                break;
            case 3:
                searchBook();
                break;
            case 4:
                saveLibraryToFile();
                printf("Library data saved. Exiting program.\n");
                exit(0); // Exit the program successfully
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

/**
 * @brief Prompts the user for book details and adds it to the library.
 */
void addBook() {
    if (book_count >= MAX_BOOKS) {
        printf("Library is full. Cannot add more books.\n");
        return;
    }

    printf("\n--- Add a New Book ---\n");
    // The ID is automatically assigned.
    library[book_count].id = book_count + 1;

    printf("Enter book title: ");
    // fgets is safer than scanf for reading strings with spaces.
    fgets(library[book_count].title, sizeof(library[book_count].title), stdin);
    // Remove the newline character that fgets reads.
    library[book_count].title[strcspn(library[book_count].title, "\n")] = 0;


    printf("Enter book author: ");
    fgets(library[book_count].author, sizeof(library[book_count].author), stdin);
    library[book_count].author[strcspn(library[book_count].author, "\n")] = 0;


    printf("Book added successfully! (ID: %d)\n", library[book_count].id);
    book_count++;
}

/**
 * @brief Displays all the books currently in the library.
 */
void displayBooks() {
    if (book_count == 0) {
        printf("\nThe library is empty.\n");
        return;
    }

    printf("\n--- List of All Books ---\n");
    printf("%-10s %-40s %-30s\n", "ID", "Title", "Author");
    printf("--------------------------------------------------------------------------\n");
    for (int i = 0; i < book_count; i++) {
        printf("%-10d %-40s %-30s\n", library[i].id, library[i].title, library[i].author);
    }
    printf("--------------------------------------------------------------------------\n");

}

/**
 * @brief Searches for a book in the library by its ID.
 */
void searchBook() {
    if (book_count == 0) {
        printf("\nThe library is empty. Cannot search.\n");
        return;
    }

    int searchId, found = 0;
    printf("\nEnter the Book ID to search for: ");
    scanf("%d", &searchId);
    while(getchar() != '\n'); // Consume newline

    for (int i = 0; i < book_count; i++) {
        if (library[i].id == searchId) {
            printf("\n--- Book Found ---\n");
            printf("ID: %d\n", library[i].id);
            printf("Title: %s\n", library[i].title);
            printf("Author: %s\n", library[i].author);
            found = 1;
            break; // Exit the loop once the book is found
        }
    }

    if (!found) {
        printf("No book found with ID %d.\n", searchId);
    }
}

/**
 * @brief Saves the current library data to a binary file.
 */
void saveLibraryToFile() {
    FILE *file_pointer;

    // Open the file in "write binary" mode ("wb").
    // This will create the file if it doesn't exist, or overwrite it if it does.
    file_pointer = fopen(FILENAME, "wb");

    if (file_pointer == NULL) {
        printf("Error: Unable to open the file for writing.\n");
        return;
    }

    // fwrite writes the entire 'library' array to the file.
    // 1. Address of data to be written (&library)
    // 2. Size of each element (sizeof(struct Book))
    // 3. Number of elements to write (book_count)
    // 4. File pointer
    fwrite(library, sizeof(struct Book), book_count, file_pointer);

    // Always close the file when you're done with it.
    fclose(file_pointer);
}

/**
 * @brief Loads library data from a binary file into the program.
 */
void loadLibraryFromFile() {
    FILE *file_pointer;

    // Open the file in "read binary" mode ("rb").
    file_pointer = fopen(FILENAME, "rb");

    // If the file doesn't exist yet (e.g., first time running), just return.
    if (file_pointer == NULL) {
        // This is not an error, it just means no library has been saved yet.
        return;
    }

    // fread reads data from the file into the 'library' array.
    // It returns the number of items successfully read.
    // We read one book at a time in a loop until we reach the end of the file.
    while(fread(&library[book_count], sizeof(struct Book), 1, file_pointer) == 1) {
        book_count++;
        if (book_count >= MAX_BOOKS) {
            break; // Stop if the library array is full
        }
    }

    fclose(file_pointer);
    printf("Loaded %d book(s) from %s.\n", book_count, FILENAME);
}
