/*
 * -----------------------------------------------------------------------------
 *
 * Project: 5 - Student Record Management System
 *
 * -----------------------------------------------------------------------------
 *
 * Question:
 * Design and implement a C program for managing student records. The program
 * should handle student information including roll number, name, and marks
 * in three subjects.
 *
 * The system must support the following "CRUD" operations:
 * 1.  Create: Add a new student record.
 * 2.  Read: Display all student records, including their calculated grade.
 * 3.  Search: Find and display the record of a specific student by roll number.
 * 4.  Update: Modify the details of an existing student record.
 * 5.  Delete: Remove a student record from the system.
 * 6.  Persistence: Save all records to a file and load them on startup.
 *
 * A grade should be calculated based on the average of the three marks:
 * - A: 90-100
 * - B: 80-89
 * - C: 70-79
 * - D: 60-69
 * - F: Below 60
 *
 * Concepts Covered:
 * - Full CRUD (Create, Read, Update, Delete) operations.
 * - Dynamic data manipulation (modifying/deleting array elements).
 * - Logic for calculations (grade calculation).
 * - Enhanced file and data management.
 *
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Constants ---
#define MAX_STUDENTS 100
#define FILENAME "students.dat"

// --- Data Structures ---
struct Student {
    int roll_no;
    char name[50];
    int marks[3]; // Marks for 3 subjects
};

// --- Global Data ---
struct Student students[MAX_STUDENTS];
int student_count = 0;

// --- Function Prototypes ---
void addStudent();
void displayAllStudents();
void searchStudent();
void updateStudent();
void deleteStudent();
char calculateGrade(struct Student s);
void saveData();
void loadData();

int main() {
    loadData();
    int choice;

    while (1) {
        printf("\n\n--- Student Record Management System ---\n");
        printf("1. Add Student Record\n");
        printf("2. Display All Student Records\n");
        printf("3. Search for a Student\n");
        printf("4. Update a Student Record\n");
        printf("5. Delete a Student Record\n");
        printf("6. Save and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // Clear input buffer

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayAllStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6:
                saveData();
                printf("Records saved. Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

/**
 * @brief Adds a new student record to the system.
 */
void addStudent() {
    if (student_count >= MAX_STUDENTS) {
        printf("Student database is full.\n");
        return;
    }

    struct Student *s = &students[student_count];
    printf("\n--- Add New Student ---\n");
    printf("Enter Roll Number: ");
    scanf("%d", &s->roll_no);
    while (getchar() != '\n');

    // Check for duplicate roll number
    for (int i = 0; i < student_count; i++) {
        if (students[i].roll_no == s->roll_no) {
            printf("Error: A student with this roll number already exists.\n");
            return;
        }
    }

    printf("Enter Name: ");
    fgets(s->name, sizeof(s->name), stdin);
    s->name[strcspn(s->name, "\n")] = 0;

    printf("Enter Marks for Subject 1: ");
    scanf("%d", &s->marks[0]);
    printf("Enter Marks for Subject 2: ");
    scanf("%d", &s->marks[1]);
    printf("Enter Marks for Subject 3: ");
    scanf("%d", &s->marks[2]);
    while (getchar() != '\n');

    student_count++;
    printf("Student record added successfully!\n");
}

/**
 * @brief Displays all student records.
 */
void displayAllStudents() {
    if (student_count == 0) {
        printf("\nNo student records found.\n");
        return;
    }
    printf("\n--- All Student Records ---\n");
    printf("%-10s %-30s %-8s %-8s %-8s %-8s\n", "Roll No", "Name", "Sub1", "Sub2", "Sub3", "Grade");
    printf("--------------------------------------------------------------------------\n");
    for (int i = 0; i < student_count; i++) {
        char grade = calculateGrade(students[i]);
        printf("%-10d %-30s %-8d %-8d %-8d %-8c\n",
               students[i].roll_no, students[i].name,
               students[i].marks[0], students[i].marks[1], students[i].marks[2],
               grade);
    }
    printf("--------------------------------------------------------------------------\n");
}

/**
 * @brief Finds a student by roll number and returns their index.
 * @return Index of the student, or -1 if not found.
 */
int findStudentByRollNo(int roll_no) {
    for (int i = 0; i < student_count; i++) {
        if (students[i].roll_no == roll_no) {
            return i; // Return the array index
        }
    }
    return -1; // Not found
}

/**
 * @brief Searches for and displays a single student's record.
 */
void searchStudent() {
    if (student_count == 0) {
        printf("\nNo records to search.\n");
        return;
    }
    int roll_no;
    printf("Enter Roll Number to search: ");
    scanf("%d", &roll_no);
    while (getchar() != '\n');

    int index = findStudentByRollNo(roll_no);

    if (index != -1) {
        struct Student s = students[index];
        char grade = calculateGrade(s);
        printf("\n--- Student Record Found ---\n");
        printf("Roll Number: %d\n", s.roll_no);
        printf("Name:        %s\n", s.name);
        printf("Marks:       %d, %d, %d\n", s.marks[0], s.marks[1], s.marks[2]);
        printf("Grade:       %c\n", grade);
    } else {
        printf("Student with Roll Number %d not found.\n", roll_no);
    }
}

/**
 * @brief Updates an existing student's record.
 */
void updateStudent() {
    if (student_count == 0) {
        printf("\nNo records to update.\n");
        return;
    }
    int roll_no;
    printf("Enter Roll Number of the student to update: ");
    scanf("%d", &roll_no);
    while (getchar() != '\n');

    int index = findStudentByRollNo(roll_no);

    if (index != -1) {
        struct Student *s = &students[index];
        printf("--- Updating Record for %s (Roll No: %d) ---\n", s->name, s->roll_no);
        printf("Enter new Name (or press Enter to keep '%s'): ", s->name);
        char newName[50];
        fgets(newName, sizeof(newName), stdin);
        if (strcmp(newName, "\n") != 0) {
            newName[strcspn(newName, "\n")] = 0;
            strcpy(s->name, newName);
        }

        printf("Enter new Marks for Subject 1 (current: %d): ", s->marks[0]);
        scanf("%d", &s->marks[0]);
        printf("Enter new Marks for Subject 2 (current: %d): ", s->marks[1]);
        scanf("%d", &s->marks[1]);
        printf("Enter new Marks for Subject 3 (current: %d): ", s->marks[2]);
        scanf("%d", &s->marks[2]);
        while (getchar() != '\n');

        printf("Record updated successfully!\n");
    } else {
        printf("Student with Roll Number %d not found.\n", roll_no);
    }
}

/**
 * @brief Deletes a student record from the system.
 */
void deleteStudent() {
    if (student_count == 0) {
        printf("\nNo records to delete.\n");
        return;
    }
    int roll_no;
    printf("Enter Roll Number of the student to delete: ");
    scanf("%d", &roll_no);
    while (getchar() != '\n');

    int index = findStudentByRollNo(roll_no);

    if (index != -1) {
        // To delete, we shift all subsequent elements one position to the left
        for (int i = index; i < student_count - 1; i++) {
            students[i] = students[i + 1];
        }
        student_count--; // Decrement the total count
        printf("Student record deleted successfully.\n");
    } else {
        printf("Student with Roll Number %d not found.\n", roll_no);
    }
}

/**
 * @brief Calculates the grade based on average marks.
 * @param s The student struct.
 * @return The character grade.
 */
char calculateGrade(struct Student s) {
    float average = (s.marks[0] + s.marks[1] + s.marks[2]) / 3.0;
    if (average >= 90) return 'A';
    if (average >= 80) return 'B';
    if (average >= 70) return 'C';
    if (average >= 60) return 'D';
    return 'F';
}

/**
 * @brief Saves the current student data to a binary file.
 */
void saveData() {
    FILE *fp = fopen(FILENAME, "wb");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    fwrite(students, sizeof(struct Student), student_count, fp);
    fclose(fp);
}

/**
 * @brief Loads student data from a binary file.
 */
void loadData() {
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        return; // File doesn't exist yet
    }
    student_count = fread(students, sizeof(struct Student), MAX_STUDENTS, fp);
    fclose(fp);
    if (student_count > 0) {
        printf("Loaded %d student record(s).\n", student_count);
    }
}
