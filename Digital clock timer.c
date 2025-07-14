/*
 * -----------------------------------------------------------------------------
 *
 * Project: 16 - Digital Clock and Countdown Timer
 *
 * -----------------------------------------------------------------------------
 *
 * Question:
 * Write a C program that functions as a simple digital clock and a countdown
 * timer.
 *
 * The system should have two main modes:
 * 1.  Digital Clock Mode: When selected, the program should continuously
 * display the current system time, updating every second until the user
 * manually stops the program (e.g., with Ctrl+C).
 * 2.  Countdown Timer Mode: When selected, the program should prompt the user
 * to enter a duration in minutes and seconds. It will then start a
 * countdown, updating the display every second. When the timer reaches
 * zero, it should print a "Time's up!" message.
 *
 * The program should present a menu to switch between these modes or exit.
 *
 * Concepts Covered:
 * - Using <time.h> for fetching and formatting the current time.
 * - Using sleep() to pause execution for regular intervals.
 * - Basic console manipulation (clearing the screen).
 * - Implementing time-based loops for a clock and a countdown.
 * - Handling user input for setting a timer duration.
 *
 * Note on Compilation:
 * - On Linux/macOS, this code should compile directly.
 * - On Windows, you might need to include <windows.h> and use Sleep()
 * (with a capital 'S') instead of sleep().
 *
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Use unistd.h for sleep() on Linux/macOS
#ifdef __unix__
#include <unistd.h>
#elif defined(_WIN32)
#include <windows.h> // Use Windows.h for Sleep() on Windows
#define sleep(sec) Sleep((sec) * 1000)
#endif


// --- Function Prototypes ---
void displayDigitalClock();
void startCountdownTimer();

int main() {
    int choice;

    while (1) {
        printf("\n\n--- Digital Clock & Timer ---\n");
        printf("1. Display Digital Clock\n");
        printf("2. Start Countdown Timer\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // Clear input buffer

        switch (choice) {
            case 1:
                displayDigitalClock();
                break;
            case 2:
                startCountdownTimer();
                break;
            case 3:
                printf("Exiting program.\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

/**
 * @brief Displays the current time, updating every second.
 */
void displayDigitalClock() {
    printf("\n--- Digital Clock ---\n");
    printf("Press Ctrl+C to return to the main menu.\n\n");
    sleep(1); // Give user time to read the message

    // This loop will run indefinitely until the user interrupts it.
    while (1) {
        // Get current time
        time_t now = time(NULL);
        struct tm *local_time = localtime(&now);

        // Format the time into HH:MM:SS
        char time_str[100];
        strftime(time_str, sizeof(time_str), "%H:%M:%S", local_time);

        // Clear the console screen
        // "clear" for Linux/macOS, "cls" for Windows
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        printf("\n\n\n");
        printf("\t\t\tCurrent Time\n");
        printf("\t\t\t============\n");
        printf("\t\t\t  %s\n", time_str);
        printf("\t\t\t============\n");
        printf("\n\n(Press Ctrl+C to stop the clock and return to menu)\n");


        // Wait for one second before updating
        sleep(1);
    }
}

/**
 * @brief Prompts for a duration and starts a countdown timer.
 */
void startCountdownTimer() {
    int minutes, seconds;
    printf("\n--- Countdown Timer ---\n");
    printf("Enter minutes: ");
    scanf("%d", &minutes);
    printf("Enter seconds: ");
    scanf("%d", &seconds);
    while (getchar() != '\n'); // Clear buffer

    if (minutes < 0 || seconds < 0) {
        printf("Invalid time entered.\n");
        return;
    }

    int total_seconds = (minutes * 60) + seconds;

    printf("Timer starting for %02d:%02d. Press Ctrl+C to cancel.\n", minutes, seconds);
    sleep(1);

    while (total_seconds >= 0) {
        // Calculate remaining minutes and seconds
        int remaining_min = total_seconds / 60;
        int remaining_sec = total_seconds % 60;

        // Clear the console screen
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        printf("\n\n\n");
        printf("\t\t\tTime Remaining\n");
        printf("\t\t\t==============\n");
        printf("\t\t\t   %02d:%02d\n", remaining_min, remaining_sec);
        printf("\t\t\t==============\n");

        if (total_seconds == 0) {
            break; // Exit loop when timer hits zero
        }
        
        sleep(1);
        total_seconds--;
    }

    printf("\n\t\t\t!!! TIME'S UP !!!\n");
    printf("\a"); // Produce a beep sound
}
