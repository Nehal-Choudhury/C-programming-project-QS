/*
 * -----------------------------------------------------------------------------
 *
 * Project: 11 - Personal Diary Management System
 *
 * -----------------------------------------------------------------------------
 *
 * Question:
 * Write a C program that functions as a simple personal diary. The user
 * should be able to create, view, and save their diary entries.
 *
 * The system must provide the following functionalities:
 * 1.  Add a new diary entry. Each entry should be automatically timestamped
 * with the date and time it was created.
 * 2.  The entry itself can be a multi-line text.
 * 3.  View all diary entries in chronological order.
 * 4.  Search for entries on a specific date.
 * 5.  Save all diary entries to a file ("diary.dat") and load them when
 * the program starts.
 *
 * Concepts Covered:
 * - Using the <time.h> library for date and time manipulation (time_t, tm, time, strftime).
 * - Handling multi-line text input.
 * - Searching based on date criteria.
 * - Reinforcing file I/O and data structure management.
 *
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constants ---
#define MAX_ENTRIES 100
#define MAX_ENTRY_LENGTH 1024
#define FILENAME "diary.dat"

// --- Data Structures ---
struct DiaryEntry {
    time_t timestamp; // Stores the date and time as a single value
    char content[MAX_ENTRY_LENGTH];
};

// --- Global Data ---
struct DiaryEntry diary[MAX_ENTRIES];
int entry_count = 0;

// --- Function Prototypes ---
void addEntry();
void viewAllEntries();
void searchEntriesByDate();
void saveData();
void loadData();

int main() {
    loadData();
    int choice;

    while (1) {
        printf("\n\n--- Personal Diary System ---\n");
        printf("1. Add New Diary Entry\n");
        printf("2. View All Entries\n");
        printf("3. Search Entries by Date\n");
        printf("4. Save and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // Clear input buffer

        switch (choice) {
            case 1: addEntry(); break;
            case 2: viewAllEntries(); break;
            case 3: searchEntriesByDate(); break;
            case 4:
                saveData();
                printf("Diary saved. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

/**
 * @brief Adds a new, timestamped entry to the diary.
 */
void addEntry() {
    if (entry_count >= MAX_ENTRIES) {
        printf("Diary is full. Cannot add more entries.\n");
        return;
    }

    struct DiaryEntry *new_entry = &diary[entry_count];

    // Get the current time and store it
    new_entry->timestamp = time(NULL);

    printf("\n--- Add New Entry ---\n");
    printf("Enter your thoughts for today. Type 'END' on a new line to finish.\n");
    printf("------------------------------------------------------------------\n");

    char buffer[MAX_ENTRY_LENGTH] = "";
    char line[200];

    while (fgets(line, sizeof(line), stdin)) {
        // Check if the user typed "END" to finish the entry
        if (strcmp(line, "END\n") == 0) {
            break;
        }
        // Ensure we don't overflow the buffer
        if (strlen(buffer) + strlen(line) < MAX_ENTRY_LENGTH) {
            strcat(buffer, line);
        } else {
            printf("Entry is too long, cannot add more text.\n");
            break;
        }
    }

    strcpy(new_entry->content, buffer);
    entry_count++;
    printf("------------------------------------------------------------------\n");
    printf("Diary entry saved successfully!\n");
}

/**
 * @brief Displays all diary entries.
 */
void viewAllEntries() {
    if (entry_count == 0) {
        printf("\nYour diary is empty.\n");
        return;
    }

    printf("\n--- All Diary Entries ---\n");
    for (int i = 0; i < entry_count; i++) {
        char time_str[100];
        // Format the timestamp into a readable string: "YYYY-MM-DD HH:MM:SS"
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&diary[i].timestamp));

        printf("\n========================================\n");
        printf("Entry Date: %s\n", time_str);
        printf("----------------------------------------\n");
        printf("%s", diary[i].content);
        printf("========================================\n");
    }
}

/**
 * @brief Searches for and displays entries from a specific date.
 */
void searchEntriesByDate() {
    if (entry_count == 0) {
        printf("\nDiary is empty, nothing to search.\n");
        return;
    }

    char search_date_str[20];
    printf("Enter date to search for (YYYY-MM-DD): ");
    fgets(search_date_str, sizeof(search_date_str), stdin);
    search_date_str[strcspn(search_date_str, "\n")] = 0; // Remove newline

    int found = 0;
    printf("\n--- Entries for %s ---\n", search_date_str);

    for (int i = 0; i < entry_count; i++) {
        char entry_date_str[20];
        // Extract just the YYYY-MM-DD part from the entry's timestamp
        strftime(entry_date_str, sizeof(entry_date_str), "%Y-%m-%d", localtime(&diary[i].timestamp));

        if (strcmp(search_date_str, entry_date_str) == 0) {
            char time_str[100];
            strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&diary[i].timestamp));

            printf("\n========================================\n");
            printf("Entry Date: %s\n", time_str);
            printf("----------------------------------------\n");
            printf("%s", diary[i].content);
            printf("========================================\n");
            found = 1;
        }
    }

    if (!found) {
        printf("No entries found for the date %s.\n", search_date_str);
    }
}


/**
 * @brief Saves the current diary data to a binary file.
 */
void saveData() {
    FILE *fp = fopen(FILENAME, "wb");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    fwrite(diary, sizeof(struct DiaryEntry), entry_count, fp);
    fclose(fp);
}

/**
 * @brief Loads diary data from a binary file.
 */
void loadData() {
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        return; // File doesn't exist yet.
    }
    entry_count = fread(diary, sizeof(struct DiaryEntry), MAX_ENTRIES, fp);
    fclose(fp);
    if (entry_count > 0) {
        printf("Loaded %d diary entries.\n", entry_count);
    }
}
