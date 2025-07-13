/*
 * -----------------------------------------------------------------------------
 *
 * Project: 19 - Student Gradebook System
 *
 * -----------------------------------------------------------------------------
 *
 * Question:
 * Create a C program to manage a gradebook for a class. The system should
 * store student information and their scores on various assignments.
 *
 * The system must support the following operations:
 * 1.  Add a new student to the gradebook (by ID and Name).
 * 2.  Enter or update scores for a specific student for a fixed number of
 * assignments (e.g., 5 assignments).
 * 3.  Display the full class gradebook, showing each student's name, their
 * scores on all assignments, and their calculated final grade (average).
 * 4.  Calculate and display class-wide statistics, such as the class
 * average, the highest grade, and the lowest grade.
 * 5.  Save the entire gradebook to a file ("gradebook.dat") and load it
 * upon startup.
 *
 * Concepts Covered:
 * - Managing nested data and arrays within structs.
 * - Performing data analysis and aggregation (calculating averages, stats).
 * - Creating detailed, formatted reports.
 * - Reinforcing CRUD principles and file persistence.
 *
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Constants ---
#define MAX_STUDENTS 50
#define NUM_ASSIGNMENTS 5
#define FILENAME "gradebook.dat"

// --- Data Structures ---
struct Student {
    int id;
    char name[100];
    double scores[NUM_ASSIGNMENTS];
    int scores_entered; // To track how many scores are set
};

// --- Global Data ---
struct Student gradebook[MAX_STUDENTS];
int student_count = 0;

// --- Function Prototypes ---
void addStudent();
void enterScores();
void displayGradebook();
void displayClassStats();
double calculateFinalGrade(int student_index);
int findStudentById(int id);
void saveData();
void loadData();

int main() {
    loadData();
    int choice;

    while (1) {
        printf("\n\n--- Student Gradebook System ---\n");
        printf("1. Add New Student\n");
        printf("2. Enter/Update Student Scores\n");
        printf("3. Display Class Gradebook\n");
        printf("4. Display Class Statistics\n");
        printf("5. Save and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // Clear input buffer

        switch (choice) {
            case 1: addStudent(); break;
            case 2: enterScores(); break;
            case 3: displayGradebook(); break;
            case 4: displayClassStats(); break;
            case 5:
                saveData();
                printf("Gradebook data saved. Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

/**
 * @brief Adds a new student to the gradebook.
 */
void addStudent() {
    if (student_count >= MAX_STUDENTS) {
        printf("Gradebook is full.\n");
        return;
    }

    struct Student *s = &gradebook[student_count];
    printf("\n--- Add New Student ---\n");
    printf("Enter Student ID: ");
    scanf("%d", &s->id);
    while (getchar() != '\n');

    if (findStudentById(s->id) != -1) {
        printf("Error: A student with ID %d already exists.\n", s->id);
        return;
    }

    printf("Enter Student Name: ");
    fgets(s->name, sizeof(s->name), stdin);
    s->name[strcspn(s->name, "\n")] = 0;

    // Initialize scores to 0
    for (int i = 0; i < NUM_ASSIGNMENTS; i++) {
        s->scores[i] = 0.0;
    }
    s->scores_entered = 0;

    student_count++;
    printf("Student added successfully!\n");
}

/**
 * @brief Enters or updates scores for a specific student.
 */
void enterScores() {
    if (student_count == 0) {
        printf("\nNo students in the gradebook. Please add a student first.\n");
        return;
    }
    int id;
    printf("Enter Student ID to enter/update scores: ");
    scanf("%d", &id);
    while (getchar() != '\n');

    int index = findStudentById(id);
    if (index == -1) {
        printf("Student with ID %d not found.\n", id);
        return;
    }

    struct Student *s = &gradebook[index];
    printf("--- Entering Scores for %s (ID: %d) ---\n", s->name, s->id);
    for (int i = 0; i < NUM_ASSIGNMENTS; i++) {
        printf("Enter score for Assignment %d (current: %.2f): ", i + 1, s->scores[i]);
        scanf("%lf", &s->scores[i]);
    }
    s->scores_entered = NUM_ASSIGNMENTS;
    while (getchar() != '\n');
    printf("Scores updated successfully.\n");
}

/**
 * @brief Displays the entire class gradebook.
 */
void displayGradebook() {
    if (student_count == 0) {
        printf("\nGradebook is empty.\n");
        return;
    }
    printf("\n--- Class Gradebook ---\n");
    printf("%-10s %-25s", "ID", "Name");
    for (int i = 0; i < NUM_ASSIGNMENTS; i++) {
        printf("  Asgmt %d", i + 1);
    }
    printf("  Final Grade\n");
    printf("--------------------------------------------------------------------------------------\n");

    for (int i = 0; i < student_count; i++) {
        printf("%-10d %-25s", gradebook[i].id, gradebook[i].name);
        for (int j = 0; j < NUM_ASSIGNMENTS; j++) {
            printf("  %-7.2f", gradebook[i].scores[j]);
        }
        double final_grade = calculateFinalGrade(i);
        printf("  %-11.2f\n", final_grade);
    }
    printf("--------------------------------------------------------------------------------------\n");
}

/**
 * @brief Calculates and displays statistics for the entire class.
 */
void displayClassStats() {
    if (student_count == 0) {
        printf("\nGradebook is empty. No stats to display.\n");
        return;
    }

    double total_grade_sum = 0;
    double highest_grade = -1.0;
    double lowest_grade = 101.0;
    char highest_scorer[100] = "N/A";
    char lowest_scorer[100] = "N/A";

    for (int i = 0; i < student_count; i++) {
        double final_grade = calculateFinalGrade(i);
        if (final_grade < 0) continue; // Skip students with no grades

        total_grade_sum += final_grade;
        if (final_grade > highest_grade) {
            highest_grade = final_grade;
            strcpy(highest_scorer, gradebook[i].name);
        }
        if (final_grade < lowest_grade) {
            lowest_grade = final_grade;
            strcpy(lowest_scorer, gradebook[i].name);
        }
    }
    
    double class_average = total_grade_sum / student_count;

    printf("\n--- Class Statistics ---\n");
    printf("Total Students: %d\n", student_count);
    printf("Class Average Grade: %.2f\n", class_average);
    printf("Highest Grade: %.2f (by %s)\n", highest_grade, highest_scorer);
    printf("Lowest Grade: %.2f (by %s)\n", lowest_grade, lowest_scorer);
    printf("--------------------------\n");
}

/**
 * @brief Calculates the final grade (average) for a student.
 * @return The final grade, or -1.0 if no scores are entered.
 */
double calculateFinalGrade(int student_index) {
    double sum = 0;
    struct Student s = gradebook[student_index];
    if (s.scores_entered == 0) return -1.0;

    for (int i = 0; i < NUM_ASSIGNMENTS; i++) {
        sum += s.scores[i];
    }
    return sum / NUM_ASSIGNMENTS;
}

/**
 * @brief Finds a student by their ID.
 * @return Index of the student, or -1 if not found.
 */
int findStudentById(int id) {
    for (int i = 0; i < student_count; i++) {
        if (gradebook[i].id == id) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Saves gradebook data to a file.
 */
void saveData() {
    FILE *fp = fopen(FILENAME, "wb");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    fwrite(gradebook, sizeof(struct Student), student_count, fp);
    fclose(fp);
}

/**
 * @brief Loads gradebook data from a file.
 */
void loadData() {
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        return;
    }
    student_count = fread(gradebook, sizeof(struct Student), MAX_STUDENTS, fp);
    fclose(fp);
    if (student_count > 0) {
        printf("Loaded %d student(s) from gradebook.\n", student_count);
    }
}
