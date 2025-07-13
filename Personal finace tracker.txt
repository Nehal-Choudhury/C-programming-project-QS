/*
 * -----------------------------------------------------------------------------
 *
 * Project: 20 - Personal Finance Tracker
 *
 * -----------------------------------------------------------------------------
 *
 * Question:
 * Create a C program to help a user track their personal finances. The system
 * should allow users to log their income and expenses and view a summary.
 *
 * The system must support the following operations:
 * 1.  Add a new transaction, which can be either 'income' or 'expense'.
 * Each transaction should have a date, a category (e.g., "Salary", "Groceries"),
 * and an amount.
 * 2.  Display a list of all transactions in chronological order.
 * 3.  Display a financial summary, showing:
 * - Total Income.
 * - Total Expenses.
 * - Net Balance (Income - Expenses).
 * 4.  Delete a transaction (optional, but good practice).
 * 5.  View transactions by category.
 * 6.  Save all financial records to a file ("finance_log.dat") and load
 * them upon startup.
 *
 * Concepts Covered:
 * - Differentiating data using enums or types within a struct.
 * - Date handling for logging transactions.
 * - Financial calculations and report generation.
 * - Combining CRUD operations into a practical, useful application.
 * - Filtering and searching data based on user criteria.
 *
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <strings.h> // For strcasecmp on POSIX systems

// --- Constants ---
#define MAX_TRANSACTIONS 200
#define FILENAME "finance_log.dat"

// --- Data Structures ---
typedef enum {
    INCOME,
    EXPENSE
} TransactionType;

struct Transaction {
    int id;
    TransactionType type;
    double amount;
    char category[100];
    char date[11]; // "YYYY-MM-DD"
};

// --- Global Data ---
struct Transaction logbook[MAX_TRANSACTIONS];
int transaction_count = 0;

// --- Function Prototypes ---
void addTransaction();
void viewAllTransactions();
void displaySummary();
void deleteTransaction();
void viewByCategory();
int findTransactionById(int id);
void saveData();
void loadData();

int main() {
    loadData();
    int choice;

    while (1) {
        printf("\n\n--- Personal Finance Tracker ---\n");
        printf("1. Add Transaction (Income/Expense)\n");
        printf("2. View All Transactions\n");
        printf("3. Display Financial Summary\n");
        printf("4. Delete a Transaction\n");
        printf("5. View Transactions by Category\n");
        printf("6. Save and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // Clear input buffer

        switch (choice) {
            case 1: addTransaction(); break;
            case 2: viewAllTransactions(); break;
            case 3: displaySummary(); break;
            case 4: deleteTransaction(); break;
            case 5: viewByCategory(); break;
            case 6:
                saveData();
                printf("Financial records saved. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

/**
 * @brief Adds a new income or expense transaction.
 */
void addTransaction() {
    if (transaction_count >= MAX_TRANSACTIONS) {
        printf("Finance log is full.\n");
        return;
    }

    struct Transaction *t = &logbook[transaction_count];
    int type_choice;

    printf("\n--- Add New Transaction ---\n");
    printf("1. Income\n");
    printf("2. Expense\n");
    printf("Select transaction type: ");
    scanf("%d", &type_choice);
    while (getchar() != '\n');

    if (type_choice == 1) {
        t->type = INCOME;
    } else if (type_choice == 2) {
        t->type = EXPENSE;
    } else {
        printf("Invalid type.\n");
        return;
    }

    t->id = (transaction_count > 0) ? logbook[transaction_count - 1].id + 1 : 1;

    printf("Enter Date (YYYY-MM-DD): ");
    scanf("%10s", t->date);
    while (getchar() != '\n');

    printf("Enter Category (e.g., Salary, Groceries): ");
    fgets(t->category, sizeof(t->category), stdin);
    t->category[strcspn(t->category, "\n")] = 0;

    printf("Enter Amount: ");
    scanf("%lf", &t->amount);
    while (getchar() != '\n');

    transaction_count++;
    printf("Transaction added successfully!\n");
}

/**
 * @brief Displays all recorded transactions.
 */
void viewAllTransactions() {
    if (transaction_count == 0) {
        printf("\nNo transactions recorded.\n");
        return;
    }
    printf("\n--- All Transactions ---\n");
    printf("%-5s %-12s %-15s %-25s %-s\n", "ID", "Type", "Date", "Category", "Amount");
    printf("--------------------------------------------------------------------------\n");

    for (int i = 0; i < transaction_count; i++) {
        struct Transaction t = logbook[i];
        const char *type_str = (t.type == INCOME) ? "Income" : "Expense";
        printf("%-5d %-12s %-15s %-25s %.2f\n",
               t.id, type_str, t.date, t.category, t.amount);
    }
    printf("--------------------------------------------------------------------------\n");
}

/**
 * @brief Calculates and displays a summary of finances.
 */
void displaySummary() {
    if (transaction_count == 0) {
        printf("\nNo transactions to summarize.\n");
        return;
    }

    double total_income = 0.0;
    double total_expenses = 0.0;

    for (int i = 0; i < transaction_count; i++) {
        if (logbook[i].type == INCOME) {
            total_income += logbook[i].amount;
        } else {
            total_expenses += logbook[i].amount;
        }
    }

    double net_balance = total_income - total_expenses;

    printf("\n--- Financial Summary ---\n");
    printf("Total Income:   +%.2f\n", total_income);
    printf("Total Expenses: -%.2f\n", total_expenses);
    printf("-------------------------\n");
    printf("Net Balance:     %.2f\n", net_balance);
    printf("-------------------------\n");
}

/**
 * @brief Deletes a transaction by its ID.
 */
void deleteTransaction() {
    if (transaction_count == 0) {
        printf("\nNo transactions to delete.\n");
        return;
    }
    int id;
    printf("Enter the ID of the transaction to delete: ");
    scanf("%d", &id);
    while (getchar() != '\n');

    int index = findTransactionById(id);
    if (index == -1) {
        printf("Transaction with ID %d not found.\n", id);
        return;
    }

    for (int i = index; i < transaction_count - 1; i++) {
        logbook[i] = logbook[i + 1];
    }
    transaction_count--;
    printf("Transaction ID %d deleted successfully.\n", id);
}

/**
 * @brief Displays transactions filtered by a specific category.
 */
void viewByCategory() {
    if (transaction_count == 0) {
        printf("\nNo transactions to view.\n");
        return;
    }
    char category_filter[100];
    printf("Enter category to filter by: ");
    fgets(category_filter, sizeof(category_filter), stdin);
    category_filter[strcspn(category_filter, "\n")] = 0;

    printf("\n--- Transactions in Category: %s ---\n", category_filter);
    printf("%-5s %-12s %-15s %-25s %-s\n", "ID", "Type", "Date", "Category", "Amount");
    printf("--------------------------------------------------------------------------\n");

    int found_count = 0;
    double category_total = 0.0;
    for (int i = 0; i < transaction_count; i++) {
        // Use strcasecmp for case-insensitive comparison
        if (strcasecmp(logbook[i].category, category_filter) == 0) {
            struct Transaction t = logbook[i];
            const char *type_str = (t.type == INCOME) ? "Income" : "Expense";
            printf("%-5d %-12s %-15s %-25s %.2f\n",
                   t.id, type_str, t.date, t.category, t.amount);
            
            if (t.type == INCOME) {
                category_total += t.amount;
            } else {
                category_total -= t.amount;
            }
            found_count++;
        }
    }

    if (found_count == 0) {
        printf("No transactions found for this category.\n");
    } else {
        printf("--------------------------------------------------------------------------\n");
        printf("Total for category '%s': %.2f\n", category_filter, category_total);
    }
     printf("--------------------------------------------------------------------------\n");
}


/**
 * @brief Finds a transaction by its ID.
 * @return Index of the transaction, or -1 if not found.
 */
int findTransactionById(int id) {
    for (int i = 0; i < transaction_count; i++) {
        if (logbook[i].id == id) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Saves transaction data to a file.
 */
void saveData() {
    FILE *fp = fopen(FILENAME, "wb");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    fwrite(logbook, sizeof(struct Transaction), transaction_count, fp);
    fclose(fp);
}

/**
 * @brief Loads transaction data from a file.
 */
void loadData() {
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        return;
    }
    transaction_count = fread(logbook, sizeof(struct Transaction), MAX_TRANSACTIONS, fp);
    fclose(fp);
    if (transaction_count > 0) {
        printf("Loaded %d transaction(s).\n", transaction_count);
    }
}
