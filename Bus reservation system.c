/*
 * -----------------------------------------------------------------------------
 *
 * Project: 15 - Bus Reservation System
 *
 * -----------------------------------------------------------------------------
 *
 * Question:
 * Create a C program that simulates a bus ticket reservation system. The
 * system should manage the booking of seats on a single bus.
 *
 * The system must support the following operations:
 * 1.  Display a seat map of the bus, showing which seats are available and
 * which are booked.
 * 2.  Book a seat: The user selects an available seat number and provides
 * their name. The seat is then marked as booked.
 * 3.  Cancel a booking: The user provides a seat number, and if it's booked,
 * the reservation is canceled, making the seat available again.
 * 4.  Display the list of all booked seats along with the passenger names.
 * 5.  Save the current booking status to a file ("bus_reservation.dat") and
 * load it when the program starts.
 *
 * Concepts Covered:
 * - Managing an array of structs to represent a physical layout (bus seats).
 * - Visual representation of data (the seat map).
 * - Status-based operations (booking and canceling).
 * - Input validation (checking for valid seat numbers and availability).
 *
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Constants ---
#define TOTAL_SEATS 32
#define FILENAME "bus_reservation.dat"

// --- Data Structures ---
struct Seat {
    int seat_number;
    int is_booked; // 0 for available, 1 for booked
    char passenger_name[100];
};

// --- Global Data ---
struct Seat bus_seats[TOTAL_SEATS];

// --- Function Prototypes ---
void initializeSeats();
void displaySeatMap();
void bookSeat();
void cancelBooking();
void displayBookedSeats();
void saveData();
void loadData();

int main() {
    loadData(); // Tries to load existing data, otherwise initializes
    int choice;

    while (1) {
        printf("\n\n--- Bus Reservation System ---\n");
        printf("1. Display Seat Map\n");
        printf("2. Book a Seat\n");
        printf("3. Cancel a Booking\n");
        printf("4. Display Booked Seats List\n");
        printf("5. Save and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // Clear input buffer

        switch (choice) {
            case 1: displaySeatMap(); break;
            case 2: bookSeat(); break;
            case 3: cancelBooking(); break;
            case 4: displayBookedSeats(); break;
            case 5:
                saveData();
                printf("Booking data saved. Have a safe journey!\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

/**
 * @brief Initializes all seats to be available if no data is loaded.
 */
void initializeSeats() {
    for (int i = 0; i < TOTAL_SEATS; i++) {
        bus_seats[i].seat_number = i + 1;
        bus_seats[i].is_booked = 0;
        strcpy(bus_seats[i].passenger_name, "N/A");
    }
}

/**
 * @brief Displays a visual map of the bus seats.
 */
void displaySeatMap() {
    printf("\n\n--- Bus Seat Map ---\n");
    printf("[XX] = Booked, [##] = Available\n");
    printf("-------------------------------------\n");
    for (int i = 0; i < TOTAL_SEATS; i++) {
        if (bus_seats[i].is_booked) {
            printf("[XX] "); // Booked seat
        } else {
            printf("[%02d] ", bus_seats[i].seat_number); // Available seat
        }
        // Newline after every 4 seats to create rows
        if ((i + 1) % 4 == 0) {
            printf("\n");
        }
    }
    printf("-------------------------------------\n");
}

/**
 * @brief Handles the process of booking a seat.
 */
void bookSeat() {
    displaySeatMap();
    int seat_num;
    printf("Enter the seat number you want to book: ");
    scanf("%d", &seat_num);
    while (getchar() != '\n');

    if (seat_num < 1 || seat_num > TOTAL_SEATS) {
        printf("Error: Invalid seat number.\n");
        return;
    }

    // Array index is seat_num - 1
    int index = seat_num - 1;

    if (bus_seats[index].is_booked) {
        printf("Error: Seat %d is already booked by %s.\n", seat_num, bus_seats[index].passenger_name);
        return;
    }

    printf("Enter passenger name for seat %d: ", seat_num);
    fgets(bus_seats[index].passenger_name, sizeof(bus_seats[index].passenger_name), stdin);
    bus_seats[index].passenger_name[strcspn(bus_seats[index].passenger_name, "\n")] = 0;

    bus_seats[index].is_booked = 1;
    printf("Seat %d booked successfully for %s!\n", seat_num, bus_seats[index].passenger_name);
}

/**
 * @brief Cancels an existing booking.
 */
void cancelBooking() {
    int seat_num;
    printf("Enter the seat number to cancel booking: ");
    scanf("%d", &seat_num);
    while (getchar() != '\n');

    if (seat_num < 1 || seat_num > TOTAL_SEATS) {
        printf("Error: Invalid seat number.\n");
        return;
    }

    int index = seat_num - 1;

    if (!bus_seats[index].is_booked) {
        printf("Error: Seat %d is not booked.\n", seat_num);
        return;
    }

    printf("Booking for seat %d by %s has been canceled.\n", seat_num, bus_seats[index].passenger_name);
    bus_seats[index].is_booked = 0;
    strcpy(bus_seats[index].passenger_name, "N/A");
}

/**
 * @brief Displays a list of all currently booked seats.
 */
void displayBookedSeats() {
    printf("\n--- List of Booked Seats ---\n");
    printf("%-15s %-s\n", "Seat Number", "Passenger Name");
    printf("----------------------------------\n");
    int booked_count = 0;
    for (int i = 0; i < TOTAL_SEATS; i++) {
        if (bus_seats[i].is_booked) {
            printf("%-15d %-s\n", bus_seats[i].seat_number, bus_seats[i].passenger_name);
            booked_count++;
        }
    }
    if (booked_count == 0) {
        printf("No seats are currently booked.\n");
    }
    printf("----------------------------------\n");
}

/**
 * @brief Saves the current booking status to a file.
 */
void saveData() {
    FILE *fp = fopen(FILENAME, "wb");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    fwrite(bus_seats, sizeof(struct Seat), TOTAL_SEATS, fp);
    fclose(fp);
}

/**
 * @brief Loads booking status from a file, or initializes if file doesn't exist.
 */
void loadData() {
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        // If file doesn't exist, it's the first run. Initialize seats.
        initializeSeats();
        return;
    }
    fread(bus_seats, sizeof(struct Seat), TOTAL_SEATS, fp);
    fclose(fp);
    printf("Loaded previous booking data.\n");
}
