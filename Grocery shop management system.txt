/*
 * -----------------------------------------------------------------------------
 *
 * Project: 4 - Grocery Shop Management System
 *
 * -----------------------------------------------------------------------------
 *
 * Question:
 * Create a C program that simulates a simple grocery shop management and
 * billing system.
 *
 * The system should support the following operations:
 * 1.  Add a new item to the shop's inventory (Item ID, Name, Price, Quantity).
 * 2.  Display all items currently in the inventory.
 * 3.  Create a new bill for a customer. The user should be able to add
 * multiple items to the bill by entering the item ID and desired quantity.
 * 4.  The system must check if the requested quantity is available in stock.
 * 5.  After billing is complete, the system should generate a formatted bill
 * and update the inventory (reduce the stock quantity of purchased items).
 * 6.  Save the inventory to a file and load it when the program starts.
 *
 * Concepts Covered:
 * - Inventory management (adding, displaying, updating items).
 * - Transactional logic (creating and finalizing a bill).
 * - Input validation (checking for sufficient stock).
 * - Structs, arrays, functions, and file I/O.
 *
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Constants ---
#define MAX_ITEMS 100
#define FILENAME "inventory.dat"

// --- Data Structures ---
struct Item {
    int id;
    char name[50];
    float price;
    int quantity;
};

// --- Global Data ---
struct Item inventory[MAX_ITEMS];
int item_count = 0;

// --- Function Prototypes ---
void addItem();
void displayInventory();
void createBill();
void searchItem();
void saveData();
void loadData();

int main() {
    loadData();
    int choice;

    while (1) {
        printf("\n\n--- Grocery Shop Management System ---\n");
        printf("1. Add New Item to Inventory\n");
        printf("2. Display Inventory\n");
        printf("3. Create New Bill\n");
        printf("4. Search for an Item\n");
        printf("5. Save and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // Clear input buffer

        switch (choice) {
            case 1: addItem(); break;
            case 2: displayInventory(); break;
            case 3: createBill(); break;
            case 4: searchItem(); break;
            case 5:
                saveData();
                printf("Inventory saved. Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

/**
 * @brief Adds a new item to the inventory.
 */
void addItem() {
    if (item_count >= MAX_ITEMS) {
        printf("Inventory is full.\n");
        return;
    }

    struct Item *newItem = &inventory[item_count];
    newItem->id = item_count + 1; // Simple auto-incrementing ID

    printf("\n--- Add New Item (ID: %d) ---\n", newItem->id);
    printf("Enter Item Name: ");
    fgets(newItem->name, sizeof(newItem->name), stdin);
    newItem->name[strcspn(newItem->name, "\n")] = 0;

    printf("Enter Price: ");
    scanf("%f", &newItem->price);

    printf("Enter Quantity: ");
    scanf("%d", &newItem->quantity);
    while (getchar() != '\n'); // Clear buffer

    item_count++;
    printf("Item added successfully!\n");
}

/**
 * @brief Displays all items in the inventory.
 */
void displayInventory() {
    if (item_count == 0) {
        printf("\nInventory is empty.\n");
        return;
    }
    printf("\n--- Current Inventory ---\n");
    printf("%-5s %-30s %-10s %-10s\n", "ID", "Name", "Price", "Quantity");
    printf("------------------------------------------------------------\n");
    for (int i = 0; i < item_count; i++) {
        printf("%-5d %-30s %-10.2f %-10d\n", inventory[i].id, inventory[i].name, inventory[i].price, inventory[i].quantity);
    }
    printf("------------------------------------------------------------\n");
}

/**
 * @brief Handles the process of creating a customer bill.
 */
void createBill() {
    if (item_count == 0) {
        printf("\nCannot create a bill, inventory is empty.\n");
        return;
    }

    struct Item bill_items[MAX_ITEMS];
    int bill_item_count = 0;
    float total_amount = 0.0;
    int item_id, quantity;
    char add_more;

    do {
        displayInventory();
        printf("\nEnter Item ID to add to bill: ");
        scanf("%d", &item_id);

        if (item_id <= 0 || item_id > item_count) {
            printf("Invalid Item ID.\n");
            while (getchar() != '\n'); // Clear buffer
            continue;
        }

        int item_index = item_id - 1; // Array index is ID - 1

        printf("Enter Quantity for %s: ", inventory[item_index].name);
        scanf("%d", &quantity);

        // --- Stock Validation ---
        if (quantity <= 0) {
            printf("Invalid quantity.\n");
        } else if (quantity > inventory[item_index].quantity) {
            printf("Error: Not enough stock. Available: %d\n", inventory[item_index].quantity);
        } else {
            // Add item to the temporary bill list
            bill_items[bill_item_count] = inventory[item_index];
            bill_items[bill_item_count].quantity = quantity; // Store the purchased quantity
            bill_item_count++;
            
            // Update inventory (this is the key step)
            inventory[item_index].quantity -= quantity;
            
            total_amount += bill_items[bill_item_count - 1].price * quantity;
            printf("Item added to bill.\n");
        }

        printf("Add another item? (y/n): ");
        scanf(" %c", &add_more);
        while (getchar() != '\n'); // Clear buffer

    } while (add_more == 'y' || add_more == 'Y');

    // --- Print Final Bill ---
    if (bill_item_count > 0) {
        printf("\n\n--- FINAL BILL ---\n");
        printf("------------------------------------------------------------\n");
        printf("%-30s %-10s %-10s %-10s\n", "Name", "Price", "Quantity", "Total");
        printf("------------------------------------------------------------\n");
        for (int i = 0; i < bill_item_count; i++) {
            float item_total = bill_items[i].price * bill_items[i].quantity;
            printf("%-30s %-10.2f %-10d %-10.2f\n", bill_items[i].name, bill_items[i].price, bill_items[i].quantity, item_total);
        }
        printf("------------------------------------------------------------\n");
        printf("%-52s %-10.2f\n", "GRAND TOTAL:", total_amount);
        printf("------------------------------------------------------------\n");
        printf("Thank you for shopping!\n");
    } else {
        printf("\nNo items were billed.\n");
    }
}

/**
 * @brief Searches for an item by its ID and displays its details.
 */
void searchItem() {
    if (item_count == 0) {
        printf("\nInventory is empty.\n");
        return;
    }
    int item_id;
    printf("\nEnter Item ID to search for: ");
    scanf("%d", &item_id);
    while (getchar() != '\n'); // Clear buffer

    if (item_id <= 0 || item_id > item_count) {
        printf("Invalid Item ID.\n");
        return;
    }

    int item_index = item_id - 1;
    printf("\n--- Item Found ---\n");
    printf("ID:       %d\n", inventory[item_index].id);
    printf("Name:     %s\n", inventory[item_index].name);
    printf("Price:    %.2f\n", inventory[item_index].price);
    printf("Quantity: %d\n", inventory[item_index].quantity);
}

/**
 * @brief Saves the current inventory data to a binary file.
 */
void saveData() {
    FILE *fp = fopen(FILENAME, "wb");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    fwrite(inventory, sizeof(struct Item), item_count, fp);
    fclose(fp);
}

/**
 * @brief Loads inventory data from a binary file.
 */
void loadData() {
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        // File might not exist on first run, which is okay.
        return;
    }
    item_count = fread(inventory, sizeof(struct Item), MAX_ITEMS, fp);
    fclose(fp);
    if (item_count > 0) {
        printf("Loaded %d item(s) from inventory.\n", item_count);
    }
}
