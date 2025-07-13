/*
 * -----------------------------------------------------------------------------
 *
 * Project: 13 - Car Rental System
 *
 * -----------------------------------------------------------------------------
 *
 * Question:
 * Create a C program to manage a small car rental business. The system
 * should keep track of the cars in the fleet and manage rentals.
 *
 * The system must support the following operations:
 * 1.  Add a new car to the fleet (Car ID, Model Name, Rent per Day).
 * 2.  Display a list of all available cars for rent.
 * 3.  Display a list of all cars in the fleet (both available and rented).
 * 4.  Rent a car: The user provides their name and the ID of the car they
 * wish to rent. The car's status is changed to "Rented".
 * 5.  Return a car: The user provides the car ID. The system calculates the
 * total rent based on the number of days it was rented and changes the
 * car's status back to "Available".
 * 6.  Save the fleet's data to a file ("cars.dat") and load it on startup.
 *
 * Concepts Covered:
 * - Inventory status management (tracking availability).
 * - Transactional logic for renting and returning items.
 * - User interaction for cost calculation.
 * - Reinforcing CRUD principles and file persistence.
 *
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Constants ---
#define MAX_CARS 50
#define FILENAME "cars.dat"

// --- Data Structures ---
struct Car {
    int id;
    char model[100];
    double rent_per_day;
    int is_available; // 1 for available, 0 for rented
    char customer_name[100]; // To store who rented the car
};

// --- Global Data ---
struct Car fleet[MAX_CARS];
int car_count = 0;

// --- Function Prototypes ---
void addCar();
void displayAvailableCars();
void displayAllCars();
void rentCar();
void returnCar();
int findCarById(int id);
void saveData();
void loadData();

int main() {
    loadData();
    int choice;

    while (1) {
        printf("\n\n--- Car Rental System ---\n");
        printf("1. Add New Car to Fleet\n");
        printf("2. Display Available Cars\n");
        printf("3. Display All Cars\n");
        printf("4. Rent a Car\n");
        printf("5. Return a Car\n");
        printf("6. Save and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // Clear input buffer

        switch (choice) {
            case 1: addCar(); break;
            case 2: displayAvailableCars(); break;
            case 3: displayAllCars(); break;
            case 4: rentCar(); break;
            case 5: returnCar(); break;
            case 6:
                saveData();
                printf("Fleet data saved. Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

/**
 * @brief Adds a new car to the fleet.
 */
void addCar() {
    if (car_count >= MAX_CARS) {
        printf("Fleet is at maximum capacity.\n");
        return;
    }

    struct Car *car = &fleet[car_count];
    printf("\n--- Add New Car ---\n");
    printf("Enter Car ID: ");
    scanf("%d", &car->id);
    while (getchar() != '\n');

    if (findCarById(car->id) != -1) {
        printf("Error: A car with ID %d already exists.\n", car->id);
        return;
    }

    printf("Enter Car Model: ");
    fgets(car->model, sizeof(car->model), stdin);
    car->model[strcspn(car->model, "\n")] = 0;

    printf("Enter Rent per Day: ");
    scanf("%lf", &car->rent_per_day);
    while (getchar() != '\n');

    car->is_available = 1; // New cars are available by default
    strcpy(car->customer_name, "N/A");

    car_count++;
    printf("Car added to the fleet successfully!\n");
}

/**
 * @brief Displays only the cars that are available for rent.
 */
void displayAvailableCars() {
    printf("\n--- Available Cars for Rent ---\n");
    printf("%-10s %-30s %-15s\n", "Car ID", "Model", "Rent per Day");
    printf("------------------------------------------------------\n");
    int available_count = 0;
    for (int i = 0; i < car_count; i++) {
        if (fleet[i].is_available) {
            printf("%-10d %-30s %-15.2f\n", fleet[i].id, fleet[i].model, fleet[i].rent_per_day);
            available_count++;
        }
    }
    if (available_count == 0) {
        printf("No cars are currently available for rent.\n");
    }
    printf("------------------------------------------------------\n");
}

/**
 * @brief Displays all cars in the fleet, along with their status.
 */
void displayAllCars() {
    if (car_count == 0) {
        printf("\nThe fleet is empty.\n");
        return;
    }
    printf("\n--- Full Fleet Status ---\n");
    printf("%-10s %-30s %-15s %-15s %-s\n", "Car ID", "Model", "Rent per Day", "Status", "Rented By");
    printf("----------------------------------------------------------------------------------------\n");
    for (int i = 0; i < car_count; i++) {
        const char* status = fleet[i].is_available ? "Available" : "Rented";
        printf("%-10d %-30s %-15.2f %-15s %-s\n",
               fleet[i].id, fleet[i].model, fleet[i].rent_per_day, status, fleet[i].customer_name);
    }
    printf("----------------------------------------------------------------------------------------\n");
}

/**
 * @brief Rents a car to a customer.
 */
void rentCar() {
    displayAvailableCars();
    int id;
    printf("\nEnter the ID of the car you want to rent: ");
    scanf("%d", &id);
    while (getchar() != '\n');

    int index = findCarById(id);
    if (index == -1) {
        printf("Error: Car with ID %d not found.\n", id);
        return;
    }

    if (!fleet[index].is_available) {
        printf("Error: Car is already rented by %s.\n", fleet[index].customer_name);
        return;
    }

    printf("Enter your name: ");
    fgets(fleet[index].customer_name, sizeof(fleet[index].customer_name), stdin);
    fleet[index].customer_name[strcspn(fleet[index].customer_name, "\n")] = 0;

    fleet[index].is_available = 0; // Mark as rented
    printf("Car ID %d has been successfully rented to %s.\n", id, fleet[index].customer_name);
}

/**
 * @brief Returns a rented car and calculates the cost.
 */
void returnCar() {
    int id, days;
    printf("Enter the ID of the car you are returning: ");
    scanf("%d", &id);
    while (getchar() != '\n');

    int index = findCarById(id);
    if (index == -1) {
        printf("Error: Car with ID %d not found.\n", id);
        return;
    }

    if (fleet[index].is_available) {
        printf("Error: This car is already marked as available.\n");
        return;
    }

    printf("Enter the number of days the car was rented: ");
    scanf("%d", &days);
    while (getchar() != '\n');

    if (days <= 0) {
        printf("Invalid number of days.\n");
        return;
    }

    double total_cost = days * fleet[index].rent_per_day;
    printf("\n--- Return Summary ---\n");
    printf("Car Model: %s\n", fleet[index].model);
    printf("Rented by: %s\n", fleet[index].customer_name);
    printf("Total Rent for %d days: $%.2f\n", days, total_cost);
    printf("------------------------\n");

    fleet[index].is_available = 1; // Mark as available again
    strcpy(fleet[index].customer_name, "N/A");
    printf("Car ID %d has been successfully returned.\n", id);
}

/**
 * @brief Finds a car by its ID.
 * @return Index of the car, or -1 if not found.
 */
int findCarById(int id) {
    for (int i = 0; i < car_count; i++) {
        if (fleet[i].id == id) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Saves fleet data to a file.
 */
void saveData() {
    FILE *fp = fopen(FILENAME, "wb");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    fwrite(fleet, sizeof(struct Car), car_count, fp);
    fclose(fp);
}

/**
 * @brief Loads fleet data from a file.
 */
void loadData() {
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        return;
    }
    car_count = fread(fleet, sizeof(struct Car), MAX_CARS, fp);
    fclose(fp);
    if (car_count > 0) {
        printf("Loaded %d car(s) from the fleet data.\n", car_count);
    }
}
