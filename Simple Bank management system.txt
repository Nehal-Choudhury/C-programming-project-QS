/*
 * -----------------------------------------------------------------------------
 *
 * Project: 10 - Simple Bank Management System
 *
 * -----------------------------------------------------------------------------
 *
 * Question:
 * Create a C program for a basic command-line banking system. The program
 * should manage customer accounts and allow for simple transactions.
 *
 * The system must support the following operations:
 * 1.  Create a new account (with a unique Account Number, Name, and initial balance).
 * 2.  Display a list of all customer accounts.
 * 3.  Search for a specific account by its account number.
 * 4.  Deposit an amount into an account.
 * 5.  Withdraw an amount from an account, ensuring the balance does not go
 * below zero.
 * 6.  Save all account data to a file ("accounts.dat") and load it on startup.
 *
 * Concepts Covered:
 * - Financial transaction logic (deposits, withdrawals).
 * - Data validation (checking for sufficient funds, unique account numbers).
 * - Reinforcement of CRUD operations (Create, Read, Update).
 * - File persistence for sensitive data.
 *
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Constants ---
#define MAX_ACCOUNTS 100
#define FILENAME "accounts.dat"

// --- Data Structures ---
struct Account {
    int acc_number;
    char name[100];
    double balance;
};

// --- Global Data ---
struct Account accounts[MAX_ACCOUNTS];
int account_count = 0;

// --- Function Prototypes ---
void createAccount();
void displayAllAccounts();
void transaction(int type); // 1 for deposit, 2 for withdrawal
int findAccount(int acc_no);
void searchAccount();
void saveData();
void loadData();

int main() {
    loadData();
    int choice;

    while (1) {
        printf("\n\n--- Simple Bank Management System ---\n");
        printf("1. Create New Account\n");
        printf("2. Display All Accounts\n");
        printf("3. Search for an Account\n");
        printf("4. Deposit Money\n");
        printf("5. Withdraw Money\n");
        printf("6. Save and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // Clear input buffer

        switch (choice) {
            case 1: createAccount(); break;
            case 2: displayAllAccounts(); break;
            case 3: searchAccount(); break;
            case 4: transaction(1); break; // Deposit
            case 5: transaction(2); break; // Withdraw
            case 6:
                saveData();
                printf("Account data saved. Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

/**
 * @brief Creates a new bank account.
 */
void createAccount() {
    if (account_count >= MAX_ACCOUNTS) {
        printf("Bank is at full capacity. Cannot create new accounts.\n");
        return;
    }

    struct Account *new_acc = &accounts[account_count];
    printf("\n--- Create New Account ---\n");
    printf("Enter Account Number: ");
    scanf("%d", &new_acc->acc_number);
    while (getchar() != '\n');

    // Check for duplicate account number
    if (findAccount(new_acc->acc_number) != -1) {
        printf("Error: An account with this number already exists.\n");
        return;
    }

    printf("Enter Account Holder's Name: ");
    fgets(new_acc->name, sizeof(new_acc->name), stdin);
    new_acc->name[strcspn(new_acc->name, "\n")] = 0;

    printf("Enter Initial Balance: ");
    scanf("%lf", &new_acc->balance);
    while (getchar() != '\n');

    account_count++;
    printf("Account created successfully!\n");
}

/**
 * @brief Displays all existing accounts.
 */
void displayAllAccounts() {
    if (account_count == 0) {
        printf("\nNo accounts found in the system.\n");
        return;
    }
    printf("\n--- All Customer Accounts ---\n");
    printf("%-15s %-30s %-15s\n", "Account No.", "Name", "Balance");
    printf("------------------------------------------------------------\n");
    for (int i = 0; i < account_count; i++) {
        printf("%-15d %-30s %-15.2f\n", accounts[i].acc_number, accounts[i].name, accounts[i].balance);
    }
    printf("------------------------------------------------------------\n");
}

/**
 * @brief Handles both deposit and withdrawal transactions.
 * @param type 1 for deposit, 2 for withdrawal.
 */
void transaction(int type) {
    if (account_count == 0) {
        printf("\nNo accounts exist to perform a transaction.\n");
        return;
    }
    int acc_no;
    double amount;
    printf("Enter Account Number: ");
    scanf("%d", &acc_no);
    while (getchar() != '\n');

    int index = findAccount(acc_no);

    if (index == -1) {
        printf("Error: Account not found.\n");
        return;
    }

    if (type == 1) { // Deposit
        printf("Enter amount to deposit: ");
        scanf("%lf", &amount);
        while (getchar() != '\n');
        if (amount <= 0) {
            printf("Invalid deposit amount.\n");
            return;
        }
        accounts[index].balance += amount;
        printf("Deposit successful. New balance: %.2f\n", accounts[index].balance);
    } else { // Withdrawal
        printf("Enter amount to withdraw: ");
        scanf("%lf", &amount);
        while (getchar() != '\n');
        if (amount <= 0) {
            printf("Invalid withdrawal amount.\n");
            return;
        }
        if (amount > accounts[index].balance) {
            printf("Error: Insufficient funds. Available balance: %.2f\n", accounts[index].balance);
        } else {
            accounts[index].balance -= amount;
            printf("Withdrawal successful. New balance: %.2f\n", accounts[index].balance);
        }
    }
}

/**
 * @brief Searches for an account by its account number.
 */
void searchAccount() {
    if (account_count == 0) {
        printf("\nNo accounts to search.\n");
        return;
    }
    int acc_no;
    printf("Enter account number to search: ");
    scanf("%d", &acc_no);
    while (getchar() != '\n');

    int index = findAccount(acc_no);

    if (index != -1) {
        struct Account acc = accounts[index];
        printf("\n--- Account Details ---\n");
        printf("Account Number: %d\n", acc.acc_number);
        printf("Holder Name:    %s\n", acc.name);
        printf("Balance:        %.2f\n", acc.balance);
    } else {
        printf("Account with number %d not found.\n", acc_no);
    }
}

/**
 * @brief Finds an account by account number.
 * @return The index of the account, or -1 if not found.
 */
int findAccount(int acc_no) {
    for (int i = 0; i < account_count; i++) {
        if (accounts[i].acc_number == acc_no) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Saves the current account data to a binary file.
 */
void saveData() {
    FILE *fp = fopen(FILENAME, "wb");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    fwrite(accounts, sizeof(struct Account), account_count, fp);
    fclose(fp);
}

/**
 * @brief Loads account data from a binary file.
 */
void loadData() {
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        return; // File doesn't exist yet.
    }
    account_count = fread(accounts, sizeof(struct Account), MAX_ACCOUNTS, fp);
    fclose(fp);
    if (account_count > 0) {
        printf("Loaded %d account(s).\n", account_count);
    }
}
