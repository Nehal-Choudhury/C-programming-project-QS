/*
 * -----------------------------------------------------------------------------
 *
 * Project: 8 - Contact Management System
 *
 * -----------------------------------------------------------------------------
 *
 * Question:
 * Write a C program to create a command-line contact management system.
 * The program should allow the user to store and manage their contacts.
 *
 * The system must support the following core operations:
 * 1.  Add a new contact, storing their name, phone number, and email address.
 * 2.  Display a list of all saved contacts.
 * 3.  Search for a contact by name and display their details.
 * 4.  Update the information (phone, email) of an existing contact.
 * 5.  Delete a contact from the system.
 * 6.  Ensure all contact data is saved to a file ("contacts.dat") upon exiting
 * and loaded from the file upon starting the program.
 *
 * Concepts Covered:
 * - Reinforcement of CRUD (Create, Read, Update, Delete) operations.
 * - String manipulation for searching and updating.
 * - Data persistence with file I/O.
 * - Structuring a complete, menu-driven application.
 *
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// --- Constants ---
#define MAX_CONTACTS 100
#define FILENAME "contacts.dat"

// --- Data Structures ---
struct Contact {
    char name[100];
    char phone[20];
    char email[100];
};

// --- Global Data ---
struct Contact contacts[MAX_CONTACTS];
int contact_count = 0;

// --- Function Prototypes ---
void addContact();
void displayAllContacts();
void searchContact();
void updateContact();
void deleteContact();
int findContactByName(const char* name);
void saveData();
void loadData();

int main() {
    loadData();
    int choice;

    while (1) {
        printf("\n\n--- Contact Management System ---\n");
        printf("1. Add New Contact\n");
        printf("2. Display All Contacts\n");
        printf("3. Search for a Contact\n");
        printf("4. Update a Contact\n");
        printf("5. Delete a Contact\n");
        printf("6. Save and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // Clear input buffer

        switch (choice) {
            case 1: addContact(); break;
            case 2: displayAllContacts(); break;
            case 3: searchContact(); break;
            case 4: updateContact(); break;
            case 5: deleteContact(); break;
            case 6:
                saveData();
                printf("Contact data saved. Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

/**
 * @brief Adds a new contact to the address book.
 */
void addContact() {
    if (contact_count >= MAX_CONTACTS) {
        printf("Contact book is full.\n");
        return;
    }

    struct Contact *c = &contacts[contact_count];
    printf("\n--- Add New Contact ---\n");

    printf("Enter Name: ");
    fgets(c->name, sizeof(c->name), stdin);
    c->name[strcspn(c->name, "\n")] = 0;

    // Check for duplicate name
    if (findContactByName(c->name) != -1) {
        printf("Error: A contact with this name already exists.\n");
        return;
    }

    printf("Enter Phone Number: ");
    fgets(c->phone, sizeof(c->phone), stdin);
    c->phone[strcspn(c->phone, "\n")] = 0;

    printf("Enter Email Address: ");
    fgets(c->email, sizeof(c->email), stdin);
    c->email[strcspn(c->email, "\n")] = 0;

    contact_count++;
    printf("Contact added successfully!\n");
}

/**
 * @brief Displays all saved contacts.
 */
void displayAllContacts() {
    if (contact_count == 0) {
        printf("\nYour contact book is empty.\n");
        return;
    }
    printf("\n--- All Contacts ---\n");
    printf("%-30s %-20s %-30s\n", "Name", "Phone Number", "Email Address");
    printf("--------------------------------------------------------------------------\n");
    for (int i = 0; i < contact_count; i++) {
        printf("%-30s %-20s %-30s\n", contacts[i].name, contacts[i].phone, contacts[i].email);
    }
    printf("--------------------------------------------------------------------------\n");
}

/**
 * @brief Searches for a contact by name and displays their details.
 */
void searchContact() {
    if (contact_count == 0) {
        printf("\nNo contacts to search.\n");
        return;
    }
    char name_to_find[100];
    printf("Enter the name to search for: ");
    fgets(name_to_find, sizeof(name_to_find), stdin);
    name_to_find[strcspn(name_to_find, "\n")] = 0;

    int index = findContactByName(name_to_find);

    if (index != -1) {
        struct Contact c = contacts[index];
        printf("\n--- Contact Found ---\n");
        printf("Name:  %s\n", c.name);
        printf("Phone: %s\n", c.phone);
        printf("Email: %s\n", c.email);
    } else {
        printf("No contact found with the name '%s'.\n", name_to_find);
    }
}

/**
 * @brief Updates an existing contact's information.
 */
void updateContact() {
    if (contact_count == 0) {
        printf("\nNo contacts to update.\n");
        return;
    }
    char name_to_update[100];
    printf("Enter the name of the contact to update: ");
    fgets(name_to_update, sizeof(name_to_update), stdin);
    name_to_update[strcspn(name_to_update, "\n")] = 0;

    int index = findContactByName(name_to_update);

    if (index != -1) {
        struct Contact *c = &contacts[index];
        printf("--- Updating Contact: %s ---\n", c->name);

        printf("Enter new Phone Number (or press Enter to keep '%s'): ", c->phone);
        char newPhone[20];
        fgets(newPhone, sizeof(newPhone), stdin);
        if (strcmp(newPhone, "\n") != 0) {
            newPhone[strcspn(newPhone, "\n")] = 0;
            strcpy(c->phone, newPhone);
        }

        printf("Enter new Email Address (or press Enter to keep '%s'): ", c->email);
        char newEmail[100];
        fgets(newEmail, sizeof(newEmail), stdin);
        if (strcmp(newEmail, "\n") != 0) {
            newEmail[strcspn(newEmail, "\n")] = 0;
            strcpy(c->email, newEmail);
        }

        printf("Contact updated successfully!\n");
    } else {
        printf("No contact found with the name '%s'.\n", name_to_update);
    }
}

/**
 * @brief Deletes a contact from the system.
 */
void deleteContact() {
    if (contact_count == 0) {
        printf("\nNo contacts to delete.\n");
        return;
    }
    char name_to_delete[100];
    printf("Enter the name of the contact to delete: ");
    fgets(name_to_delete, sizeof(name_to_delete), stdin);
    name_to_delete[strcspn(name_to_delete, "\n")] = 0;

    int index = findContactByName(name_to_delete);

    if (index != -1) {
        // Shift all subsequent elements one position to the left
        for (int i = index; i < contact_count - 1; i++) {
            contacts[i] = contacts[i + 1];
        }
        contact_count--; // Decrement the total count
        printf("Contact '%s' deleted successfully.\n", name_to_delete);
    } else {
        printf("No contact found with the name '%s'.\n", name_to_delete);
    }
}

/**
 * @brief Finds a contact by name (case-insensitive).
 * @param name The name to search for.
 * @return The index of the contact in the array, or -1 if not found.
 */
int findContactByName(const char* name) {
    for (int i = 0; i < contact_count; i++) {
        // strcasecmp is a non-standard, case-insensitive version of strcmp
        // For portability, we can convert both strings to lower/upper case first
        // But for simplicity here, we'll use a direct comparison.
        if (strcasecmp(contacts[i].name, name) == 0) {
            return i;
        }
    }
    return -1; // Not found
}

/**
 * @brief Saves the current contact list to a binary file.
 */
void saveData() {
    FILE *fp = fopen(FILENAME, "wb");
    if (fp == NULL) {
        printf("Error: Could not open file for writing.\n");
        return;
    }
    fwrite(contacts, sizeof(struct Contact), contact_count, fp);
    fclose(fp);
}

/**
 * @brief Loads the contact list from a binary file.
 */
void loadData() {
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        return; // File doesn't exist on first run, which is normal.
    }
    contact_count = fread(contacts, sizeof(struct Contact), MAX_CONTACTS, fp);
    fclose(fp);
    if (contact_count > 0) {
        printf("Loaded %d contact(s) from file.\n", contact_count);
    }
}
