/*
 * -----------------------------------------------------------------------------
 *
 * Project: 12 - Employee Management System
 *
 * -----------------------------------------------------------------------------
 *
 * Question:
 * Develop a C program to manage employee records for a small company.
 * The system should handle employee information and be able to calculate
 * their monthly salary.
 *
 * The system must support the following operations:
 * 1.  Add a new employee record (Employee ID, Name, Department, Basic Salary).
 * 2.  Display a list of all employees.
 * 3.  Update an employee's record.
 * 4.  Delete an employee's record.
 * 5.  Generate and display a salary slip for a specific employee. The slip
 * should calculate Gross Salary and Net Salary based on predefined rules:
 * - HRA (House Rent Allowance): 10% of Basic Salary
 * - DA (Dearness Allowance): 7.5% of Basic Salary
 * - PF (Provident Fund Deduction): 12% of Basic Salary
 * - Gross Salary = Basic Salary + HRA + DA
 * - Net Salary = Gross Salary - PF
 * 6.  Save all records to a file ("employees.dat") and load them on startup.
 *
 * Concepts Covered:
 * - Full CRUD (Create, Read, Update, Delete) operations.
 * - Performing calculations based on stored data.
 * - Generating formatted output (salary slip).
 * - Data validation (e.g., unique employee ID).
 *
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Constants ---
#define MAX_EMPLOYEES 100
#define FILENAME "employees.dat"

// --- Data Structures ---
struct Employee {
    int id;
    char name[100];
    char department[50];
    double basic_salary;
};

// --- Global Data ---
struct Employee employees[MAX_EMPLOYEES];
int employee_count = 0;

// --- Function Prototypes ---
void addEmployee();
void displayAllEmployees();
void updateEmployee();
void deleteEmployee();
void generateSalarySlip();
int findEmployeeById(int id);
void saveData();
void loadData();

int main() {
    loadData();
    int choice;

    while (1) {
        printf("\n\n--- Employee Management System ---\n");
        printf("1. Add New Employee\n");
        printf("2. Display All Employees\n");
        printf("3. Update Employee Record\n");
        printf("4. Delete Employee Record\n");
        printf("5. Generate Salary Slip\n");
        printf("6. Save and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // Clear input buffer

        switch (choice) {
            case 1: addEmployee(); break;
            case 2: displayAllEmployees(); break;
            case 3: updateEmployee(); break;
            case 4: deleteEmployee(); break;
            case 5: generateSalarySlip(); break;
            case 6:
                saveData();
                printf("Employee records saved. Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

/**
 * @brief Adds a new employee to the system.
 */
void addEmployee() {
    if (employee_count >= MAX_EMPLOYEES) {
        printf("Employee database is full.\n");
        return;
    }

    struct Employee *emp = &employees[employee_count];
    printf("\n--- Add New Employee ---\n");
    printf("Enter Employee ID: ");
    scanf("%d", &emp->id);
    while (getchar() != '\n');

    if (findEmployeeById(emp->id) != -1) {
        printf("Error: An employee with ID %d already exists.\n", emp->id);
        return;
    }

    printf("Enter Name: ");
    fgets(emp->name, sizeof(emp->name), stdin);
    emp->name[strcspn(emp->name, "\n")] = 0;

    printf("Enter Department: ");
    fgets(emp->department, sizeof(emp->department), stdin);
    emp->department[strcspn(emp->department, "\n")] = 0;

    printf("Enter Basic Salary: ");
    scanf("%lf", &emp->basic_salary);
    while (getchar() != '\n');

    employee_count++;
    printf("Employee added successfully!\n");
}

/**
 * @brief Displays all employee records.
 */
void displayAllEmployees() {
    if (employee_count == 0) {
        printf("\nNo employee records found.\n");
        return;
    }
    printf("\n--- All Employee Records ---\n");
    printf("%-10s %-30s %-20s %-15s\n", "ID", "Name", "Department", "Basic Salary");
    printf("--------------------------------------------------------------------------\n");
    for (int i = 0; i < employee_count; i++) {
        printf("%-10d %-30s %-20s %-15.2f\n",
               employees[i].id, employees[i].name,
               employees[i].department, employees[i].basic_salary);
    }
    printf("--------------------------------------------------------------------------\n");
}

/**
 * @brief Updates an existing employee's record.
 */
void updateEmployee() {
    if (employee_count == 0) {
        printf("\nNo records to update.\n");
        return;
    }
    int id;
    printf("Enter Employee ID to update: ");
    scanf("%d", &id);
    while (getchar() != '\n');

    int index = findEmployeeById(id);
    if (index == -1) {
        printf("Employee with ID %d not found.\n", id);
        return;
    }

    struct Employee *emp = &employees[index];
    printf("--- Updating Record for %s (ID: %d) ---\n", emp->name, emp->id);
    printf("Enter new Department (current: %s): ", emp->department);
    fgets(emp->department, sizeof(emp->department), stdin);
    emp->department[strcspn(emp->department, "\n")] = 0;

    printf("Enter new Basic Salary (current: %.2f): ", emp->basic_salary);
    scanf("%lf", &emp->basic_salary);
    while (getchar() != '\n');

    printf("Record updated successfully!\n");
}

/**
 * @brief Deletes an employee record.
 */
void deleteEmployee() {
    if (employee_count == 0) {
        printf("\nNo records to delete.\n");
        return;
    }
    int id;
    printf("Enter Employee ID to delete: ");
    scanf("%d", &id);
    while (getchar() != '\n');

    int index = findEmployeeById(id);
    if (index == -1) {
        printf("Employee with ID %d not found.\n", id);
        return;
    }

    for (int i = index; i < employee_count - 1; i++) {
        employees[i] = employees[i + 1];
    }
    employee_count--;
    printf("Employee record deleted successfully.\n");
}

/**
 * @brief Calculates and displays a salary slip for an employee.
 */
void generateSalarySlip() {
    if (employee_count == 0) {
        printf("\nNo records found.\n");
        return;
    }
    int id;
    printf("Enter Employee ID to generate salary slip: ");
    scanf("%d", &id);
    while (getchar() != '\n');

    int index = findEmployeeById(id);
    if (index == -1) {
        printf("Employee with ID %d not found.\n", id);
        return;
    }

    struct Employee emp = employees[index];
    double hra = emp.basic_salary * 0.10;
    double da = emp.basic_salary * 0.075;
    double pf = emp.basic_salary * 0.12;
    double gross_salary = emp.basic_salary + hra + da;
    double net_salary = gross_salary - pf;

    printf("\n\n--- Salary Slip for %s ---\n", emp.name);
    printf("========================================\n");
    printf("Employee ID: %d\n", emp.id);
    printf("Department:  %s\n", emp.department);
    printf("----------------------------------------\n");
    printf("  Earnings\n");
    printf("----------------------------------------\n");
    printf("  Basic Salary:    %10.2f\n", emp.basic_salary);
    printf("  HRA (10%%):       %10.2f\n", hra);
    printf("  DA (7.5%%):       %10.2f\n", da);
    printf("----------------------------------------\n");
    printf("  Gross Salary:    %10.2f\n", gross_salary);
    printf("----------------------------------------\n");
    printf("  Deductions\n");
    printf("----------------------------------------\n");
    printf("  Provident Fund:  %10.2f\n", pf);
    printf("----------------------------------------\n");
    printf("  NET SALARY:      %10.2f\n", net_salary);
    printf("========================================\n");
}

/**
 * @brief Finds an employee by their ID.
 * @return Index of the employee, or -1 if not found.
 */
int findEmployeeById(int id) {
    for (int i = 0; i < employee_count; i++) {
        if (employees[i].id == id) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Saves employee data to a file.
 */
void saveData() {
    FILE *fp = fopen(FILENAME, "wb");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    fwrite(employees, sizeof(struct Employee), employee_count, fp);
    fclose(fp);
}

/**
 * @brief Loads employee data from a file.
 */
void loadData() {
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        return;
    }
    employee_count = fread(employees, sizeof(struct Employee), MAX_EMPLOYEES, fp);
    fclose(fp);
    if (employee_count > 0) {
        printf("Loaded %d employee record(s).\n", employee_count);
    }
}
