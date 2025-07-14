/*
 * -----------------------------------------------------------------------------
 *
 * Project: 18 - Currency Converter
 *
 * -----------------------------------------------------------------------------
 *
 * Question:
 * Write a C program that acts as a simple currency converter. The program
 * should be able to convert an amount from one currency to another based on
 * a fixed set of exchange rates.
 *
 * The program should provide the following functionality:
 * 1.  Store a list of currencies (e.g., USD, EUR, GBP, JPY, INR) and their
 * conversion rates relative to a base currency (e.g., USD).
 * 2.  Display a list of available currencies for the user to choose from.
 * 3.  Prompt the user to enter the amount they want to convert.
 * 4.  Prompt the user to select the "from" currency and the "to" currency.
 * 5.  Calculate and display the converted amount.
 * 6.  The program should loop to allow for multiple conversions until the
 * user chooses to exit.
 *
 * Example Rates (relative to 1 USD):
 * - USD: 1.0
 * - EUR: 0.92
 * - GBP: 0.79
 * - JPY: 157.45
 * - INR: 83.54
 *
 * Concepts Covered:
 * - Working with arrays of structs to store data.
 * - Menu-driven input for selecting options.
 * - Performing calculations based on user choices.
 * - Creating a reusable conversion logic.
 *
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Data Structures ---
struct Currency {
    char code[4]; // e.g., "USD"
    char name[50];
    double rate_vs_usd; // Rate relative to 1 USD
};

// --- Global Data ---
// A fixed list of currencies and their rates
struct Currency currencies[] = {
    {"USD", "US Dollar", 1.0},
    {"EUR", "Euro", 0.92},
    {"GBP", "British Pound", 0.79},
    {"JPY", "Japanese Yen", 157.45},
    {"INR", "Indian Rupee", 83.54},
    {"CAD", "Canadian Dollar", 1.37}
};
const int currency_count = sizeof(currencies) / sizeof(currencies[0]);

// --- Function Prototypes ---
void displayCurrencies();
int findCurrencyByCode(const char* code);

int main() {
    char from_code[4], to_code[4];
    double amount;
    int from_index, to_index;
    int choice;

    while (1) {
        printf("\n\n--- Currency Converter ---\n");
        printf("1. Perform a Conversion\n");
        printf("2. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n');

        if (choice != 1) {
            break; // Exit loop if user doesn't choose 1
        }

        // --- Start Conversion Process ---
        displayCurrencies();

        printf("\nEnter the amount to convert: ");
        scanf("%lf", &amount);
        while (getchar() != '\n');

        printf("Enter the 3-letter code of the currency to convert FROM: ");
        scanf("%3s", from_code);
        while (getchar() != '\n');

        printf("Enter the 3-letter code of the currency to convert TO: ");
        scanf("%3s", to_code);
        while (getchar() != '\n');

        // Convert codes to uppercase for consistent matching
        for(int i = 0; from_code[i]; i++) from_code[i] = toupper(from_code[i]);
        for(int i = 0; to_code[i]; i++) to_code[i] = toupper(to_code[i]);

        // Find the indices of the selected currencies
        from_index = findCurrencyByCode(from_code);
        to_index = findCurrencyByCode(to_code);

        // --- Validation ---
        if (from_index == -1 || to_index == -1) {
            printf("Error: One or both currency codes are invalid.\n");
            continue; // Go back to the main menu
        }

        // --- Calculation ---
        // 1. Convert the initial amount to the base currency (USD).
        double amount_in_usd = amount / currencies[from_index].rate_vs_usd;
        // 2. Convert the USD amount to the target currency.
        double converted_amount = amount_in_usd * currencies[to_index].rate_vs_usd;

        // --- Display Result ---
        printf("\n--- Conversion Result ---\n");
        printf("%.2f %s = %.2f %s\n",
               amount, currencies[from_index].code,
               converted_amount, currencies[to_index].code);
        printf("-------------------------\n");
    }

    printf("Thank you for using the Currency Converter!\n");
    return 0;
}

/**
 * @brief Displays the list of available currencies and their codes.
 */
void displayCurrencies() {
    printf("\n--- Available Currencies ---\n");
    printf("%-10s %-s\n", "Code", "Currency Name");
    printf("------------------------------\n");
    for (int i = 0; i < currency_count; i++) {
        printf("%-10s %-s\n", currencies[i].code, currencies[i].name);
    }
    printf("------------------------------\n");
}

/**
 * @brief Finds a currency in the global array by its 3-letter code.
 * @param code The currency code to search for (e.g., "USD").
 * @return The index of the currency in the array, or -1 if not found.
 */
int findCurrencyByCode(const char* code) {
    for (int i = 0; i < currency_count; i++) {
        if (strcmp(currencies[i].code, code) == 0) {
            return i;
        }
    }
    return -1; // Not found
}
