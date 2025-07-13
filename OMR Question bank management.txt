/*
 * -----------------------------------------------------------------------------
 *
 * Project: 6 - OMR Question Bank Management System
 *
 * -----------------------------------------------------------------------------
 *
 * Question:
 * Create a C program to manage a question bank for a multiple-choice quiz,
 * similar to what would be used for an OMR (Optical Mark Recognition) test.
 *
 * The system should provide the following functionalities:
 * 1.  Add a new question to the bank. Each question must have:
 * - A unique ID.
 * - The question text.
 * - Four options (A, B, C, D).
 * - The correct answer (e.g., 'A').
 * 2.  Display all questions and their correct answers (for the administrator).
 * 3.  Start a quiz: The system will present each question to the user,
 * prompt for an answer, and then calculate and display the final score.
 * 4.  Save the entire question bank to a file ("questions.dat").
 * 5.  Load the question bank from the file when the program starts.
 *
 * Concepts Covered:
 * - Managing more complex structs (containing arrays of strings).
 * - Creating an interactive quiz loop.
 * - Input validation for user answers.
 * - Score tracking and reporting.
 * - File I/O for persistence.
 *
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // For toupper()

// --- Constants ---
#define MAX_QUESTIONS 100
#define FILENAME "questions.dat"

// --- Data Structures ---
struct Question {
    int id;
    char text[256];
    char options[4][100]; // Array of 4 strings for the options
    char correct_option;  // 'A', 'B', 'C', or 'D'
};

// --- Global Data ---
struct Question question_bank[MAX_QUESTIONS];
int question_count = 0;

// --- Function Prototypes ---
void addQuestion();
void displayAllQuestions();
void startQuiz();
void saveData();
void loadData();

int main() {
    loadData();
    int choice;

    while (1) {
        printf("\n\n--- OMR Question Bank Management ---\n");
        printf("1. Add New Question\n");
        printf("2. Display All Questions (Admin View)\n");
        printf("3. Start Quiz\n");
        printf("4. Save and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // Clear input buffer

        switch (choice) {
            case 1: addQuestion(); break;
            case 2: displayAllQuestions(); break;
            case 3: startQuiz(); break;
            case 4:
                saveData();
                printf("Question bank saved. Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

/**
 * @brief Adds a new question to the question bank.
 */
void addQuestion() {
    if (question_count >= MAX_QUESTIONS) {
        printf("Question bank is full.\n");
        return;
    }

    struct Question *q = &question_bank[question_count];
    q->id = question_count + 1;

    printf("\n--- Add New Question (ID: %d) ---\n", q->id);
    printf("Enter the question text: ");
    fgets(q->text, sizeof(q->text), stdin);
    q->text[strcspn(q->text, "\n")] = 0;

    printf("Enter Option A: ");
    fgets(q->options[0], sizeof(q->options[0]), stdin);
    q->options[0][strcspn(q->options[0], "\n")] = 0;

    printf("Enter Option B: ");
    fgets(q->options[1], sizeof(q->options[1]), stdin);
    q->options[1][strcspn(q->options[1], "\n")] = 0;

    printf("Enter Option C: ");
    fgets(q->options[2], sizeof(q->options[2]), stdin);
    q->options[2][strcspn(q->options[2], "\n")] = 0;

    printf("Enter Option D: ");
    fgets(q->options[3], sizeof(q->options[3]), stdin);
    q->options[3][strcspn(q->options[3], "\n")] = 0;

    // Loop to ensure a valid correct answer is entered
    while (1) {
        printf("Enter the correct option (A, B, C, or D): ");
        scanf(" %c", &q->correct_option);
        while (getchar() != '\n'); // Clear buffer
        q->correct_option = toupper(q->correct_option); // Convert to uppercase
        if (q->correct_option >= 'A' && q->correct_option <= 'D') {
            break;
        }
        printf("Invalid input. Please enter A, B, C, or D.\n");
    }

    question_count++;
    printf("Question added successfully!\n");
}

/**
 * @brief Displays all questions with their options and correct answer.
 */
void displayAllQuestions() {
    if (question_count == 0) {
        printf("\nThe question bank is empty.\n");
        return;
    }
    printf("\n--- All Questions in Bank ---\n");
    for (int i = 0; i < question_count; i++) {
        struct Question q = question_bank[i];
        printf("\n-------------------------------------\n");
        printf("ID: %d\n", q.id);
        printf("Q: %s\n", q.text);
        printf("  A) %s\n", q.options[0]);
        printf("  B) %s\n", q.options[1]);
        printf("  C) %s\n", q.options[2]);
        printf("  D) %s\n", q.options[3]);
        printf("Correct Answer: %c\n", q.correct_option);
        printf("-------------------------------------\n");
    }
}

/**
 * @brief Starts an interactive quiz for the user.
 */
void startQuiz() {
    if (question_count == 0) {
        printf("\nCannot start quiz. The question bank is empty.\n");
        return;
    }

    int score = 0;
    char user_answer;

    printf("\n\n--- QUIZ STARTED! --- \n");
    printf("Answer each question with A, B, C, or D.\n");

    for (int i = 0; i < question_count; i++) {
        struct Question q = question_bank[i];
        printf("\n-------------------------------------\n");
        printf("Question %d: %s\n", q.id, q.text);
        printf("  A) %s\n", q.options[0]);
        printf("  B) %s\n", q.options[1]);
        printf("  C) %s\n", q.options[2]);
        printf("  D) %s\n", q.options[3]);

        while (1) {
            printf("Your Answer: ");
            scanf(" %c", &user_answer);
            while (getchar() != '\n'); // Clear buffer
            user_answer = toupper(user_answer);
            if (user_answer >= 'A' && user_answer <= 'D') {
                break;
            }
            printf("Invalid input. Please enter A, B, C, or D.\n");
        }

        if (user_answer == q.correct_option) {
            printf("Correct!\n");
            score++;
        } else {
            printf("Wrong! The correct answer was %c.\n", q.correct_option);
        }
    }

    printf("\n\n--- QUIZ FINISHED ---\n");
    printf("Your final score is: %d out of %d\n", score, question_count);
    printf("Percentage: %.2f%%\n", (float)score / question_count * 100);
    printf("-----------------------\n");
}

/**
 * @brief Saves the current question bank to a binary file.
 */
void saveData() {
    FILE *fp = fopen(FILENAME, "wb");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    fwrite(question_bank, sizeof(struct Question), question_count, fp);
    fclose(fp);
}

/**
 * @brief Loads the question bank from a binary file.
 */
void loadData() {
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        return; // File might not exist on the first run.
    }
    question_count = fread(question_bank, sizeof(struct Question), MAX_QUESTIONS, fp);
    fclose(fp);
    if (question_count > 0) {
        printf("Loaded %d question(s) from the bank.\n", question_count);
    }
}
