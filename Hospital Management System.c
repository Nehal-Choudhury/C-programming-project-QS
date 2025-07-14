/*
 * -----------------------------------------------------------------------------
 *
 * Project: 3 - Simple Hospital Management System
 *
 * -----------------------------------------------------------------------------
 *
 * Question:
 * Develop a C program for a simple hospital management system. The system
 * should handle patient records, doctor records, and appointments.
 *
 * Core functionalities should include:
 * 1.  Registering a new patient (ID, Name, Age, Gender).
 * 2.  Registering a new doctor (ID, Name, Specialization).
 * 3.  Scheduling a new appointment (Appointment ID, Patient ID, Doctor ID, Date).
 * 4.  Displaying a list of all patients.
 * 5.  Displaying a list of all doctors.
 * 6.  Displaying a list of all scheduled appointments.
 * 7.  Saving all data (patients, doctors, appointments) to separate files.
 * 8.  Loading all data from files upon startup.
 *
 * The program must be menu-driven, allowing the user to select an operation
 * and continue until they choose to exit.
 *
 * Concepts Covered:
 * - Managing multiple, related data structures (structs for Patient, Doctor, Appointment).
 * - Enhanced file I/O for handling multiple data files.
 * - Data relationships (linking a patient and doctor in an appointment).
 * - More complex menu navigation.
 *
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Constants ---
#define MAX_PATIENTS 100
#define MAX_DOCTORS 50
#define MAX_APPOINTMENTS 200

#define PATIENTS_FILE "patients.dat"
#define DOCTORS_FILE "doctors.dat"
#define APPOINTMENTS_FILE "appointments.dat"

// --- Data Structures ---
struct Patient {
    int id;
    char name[100];
    int age;
    char gender[10];
};

struct Doctor {
    int id;
    char name[100];
    char specialization[100];
};

struct Appointment {
    int id;
    int patient_id;
    int doctor_id;
    char date[20]; // e.g., "YYYY-MM-DD"
};

// --- Global Data Arrays ---
struct Patient patients[MAX_PATIENTS];
struct Doctor doctors[MAX_DOCTORS];
struct Appointment appointments[MAX_APPOINTMENTS];

// --- Global Counters ---
int patient_count = 0;
int doctor_count = 0;
int appointment_count = 0;

// --- Function Prototypes ---
void addPatient();
void addDoctor();
void scheduleAppointment();
void displayPatients();
void displayDoctors();
void displayAppointments();
void saveData();
void loadData();

int main() {
    // Load all data from files when the program starts.
    loadData();

    int choice;
    while (1) {
        printf("\n\n--- Hospital Management System ---\n");
        printf("1. Add New Patient\n");
        printf("2. Add New Doctor\n");
        printf("3. Schedule Appointment\n");
        printf("4. Display All Patients\n");
        printf("5. Display All Doctors\n");
        printf("6. Display All Appointments\n");
        printf("7. Save and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // Clear input buffer

        switch (choice) {
            case 1: addPatient(); break;
            case 2: addDoctor(); break;
            case 3: scheduleAppointment(); break;
            case 4: displayPatients(); break;
            case 5: displayDoctors(); break;
            case 6: displayAppointments(); break;
            case 7:
                saveData();
                printf("All data saved. Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

// --- Function Implementations ---

/**
 * @brief Adds a new patient to the system.
 */
void addPatient() {
    if (patient_count >= MAX_PATIENTS) {
        printf("Patient database is full.\n");
        return;
    }
    struct Patient *p = &patients[patient_count];
    p->id = patient_count + 1;
    printf("\n--- Add New Patient (ID: %d) ---\n", p->id);
    printf("Enter Name: ");
    fgets(p->name, sizeof(p->name), stdin);
    p->name[strcspn(p->name, "\n")] = 0;
    printf("Enter Age: ");
    scanf("%d", &p->age);
    while (getchar() != '\n'); // Clear buffer
    printf("Enter Gender: ");
    fgets(p->gender, sizeof(p->gender), stdin);
    p->gender[strcspn(p->gender, "\n")] = 0;

    patient_count++;
    printf("Patient added successfully!\n");
}

/**
 * @brief Adds a new doctor to the system.
 */
void addDoctor() {
    if (doctor_count >= MAX_DOCTORS) {
        printf("Doctor database is full.\n");
        return;
    }
    struct Doctor *d = &doctors[doctor_count];
    d->id = doctor_count + 1;
    printf("\n--- Add New Doctor (ID: %d) ---\n", d->id);
    printf("Enter Name: ");
    fgets(d->name, sizeof(d->name), stdin);
    d->name[strcspn(d->name, "\n")] = 0;
    printf("Enter Specialization: ");
    fgets(d->specialization, sizeof(d->specialization), stdin);
    d->specialization[strcspn(d->specialization, "\n")] = 0;

    doctor_count++;
    printf("Doctor added successfully!\n");
}

/**
 * @brief Schedules a new appointment.
 */
void scheduleAppointment() {
    if (appointment_count >= MAX_APPOINTMENTS) {
        printf("Appointment schedule is full.\n");
        return;
    }
    if (patient_count == 0 || doctor_count == 0) {
        printf("Cannot schedule appointment. Please add patients and doctors first.\n");
        return;
    }

    struct Appointment *a = &appointments[appointment_count];
    a->id = appointment_count + 1;
    printf("\n--- Schedule New Appointment (ID: %d) ---\n", a->id);

    // Display available patients and doctors to make scheduling easier
    displayPatients();
    displayDoctors();

    printf("Enter Patient ID: ");
    scanf("%d", &a->patient_id);
    printf("Enter Doctor ID: ");
    scanf("%d", &a->doctor_id);
    while (getchar() != '\n'); // Clear buffer
    printf("Enter Date (YYYY-MM-DD): ");
    fgets(a->date, sizeof(a->date), stdin);
    a->date[strcspn(a->date, "\n")] = 0;

    // Basic validation (can be improved)
    if (a->patient_id <= 0 || a->patient_id > patient_count || a->doctor_id <= 0 || a->doctor_id > doctor_count) {
        printf("Invalid Patient or Doctor ID. Appointment not scheduled.\n");
        return;
    }

    appointment_count++;
    printf("Appointment scheduled successfully!\n");
}

/**
 * @brief Displays all registered patients.
 */
void displayPatients() {
    if (patient_count == 0) {
        printf("\nNo patients registered.\n");
        return;
    }
    printf("\n--- List of All Patients ---\n");
    printf("%-5s %-30s %-10s %-10s\n", "ID", "Name", "Age", "Gender");
    printf("----------------------------------------------------------\n");
    for (int i = 0; i < patient_count; i++) {
        printf("%-5d %-30s %-10d %-10s\n", patients[i].id, patients[i].name, patients[i].age, patients[i].gender);
    }
    printf("----------------------------------------------------------\n");
}

/**
 * @brief Displays all registered doctors.
 */
void displayDoctors() {
    if (doctor_count == 0) {
        printf("\nNo doctors registered.\n");
        return;
    }
    printf("\n--- List of All Doctors ---\n");
    printf("%-5s %-30s %-30s\n", "ID", "Name", "Specialization");
    printf("------------------------------------------------------------------\n");
    for (int i = 0; i < doctor_count; i++) {
        printf("%-5d %-30s %-30s\n", doctors[i].id, doctors[i].name, doctors[i].specialization);
    }
    printf("------------------------------------------------------------------\n");
}

/**
 * @brief Displays all scheduled appointments.
 */
void displayAppointments() {
    if (appointment_count == 0) {
        printf("\nNo appointments scheduled.\n");
        return;
    }
    printf("\n--- List of All Appointments ---\n");
    printf("%-10s %-15s %-15s %-15s\n", "Appt. ID", "Patient ID", "Doctor ID", "Date");
    printf("------------------------------------------------------------\n");
    for (int i = 0; i < appointment_count; i++) {
        printf("%-10d %-15d %-15d %-15s\n", appointments[i].id, appointments[i].patient_id, appointments[i].doctor_id, appointments[i].date);
    }
    printf("------------------------------------------------------------\n");
}

/**
 * @brief Saves all data (patients, doctors, appointments) to binary files.
 */
void saveData() {
    FILE *fp;

    // Save patients
    if ((fp = fopen(PATIENTS_FILE, "wb")) != NULL) {
        fwrite(patients, sizeof(struct Patient), patient_count, fp);
        fclose(fp);
    } else {
        printf("Error: Could not open %s for writing.\n", PATIENTS_FILE);
    }

    // Save doctors
    if ((fp = fopen(DOCTORS_FILE, "wb")) != NULL) {
        fwrite(doctors, sizeof(struct Doctor), doctor_count, fp);
        fclose(fp);
    } else {
        printf("Error: Could not open %s for writing.\n", DOCTORS_FILE);
    }

    // Save appointments
    if ((fp = fopen(APPOINTMENTS_FILE, "wb")) != NULL) {
        fwrite(appointments, sizeof(struct Appointment), appointment_count, fp);
        fclose(fp);
    } else {
        printf("Error: Could not open %s for writing.\n", APPOINTMENTS_FILE);
    }
}

/**
 * @brief Loads all data from their respective binary files.
 */
void loadData() {
    FILE *fp;
    int loaded_count = 0;

    // Load patients
    if ((fp = fopen(PATIENTS_FILE, "rb")) != NULL) {
        patient_count = fread(patients, sizeof(struct Patient), MAX_PATIENTS, fp);
        fclose(fp);
        loaded_count += patient_count;
    }

    // Load doctors
    if ((fp = fopen(DOCTORS_FILE, "rb")) != NULL) {
        doctor_count = fread(doctors, sizeof(struct Doctor), MAX_DOCTORS, fp);
        fclose(fp);
        loaded_count += doctor_count;
    }

    // Load appointments
    if ((fp = fopen(APPOINTMENTS_FILE, "rb")) != NULL) {
        appointment_count = fread(appointments, sizeof(struct Appointment), MAX_APPOINTMENTS, fp);
        fclose(fp);
        loaded_count += appointment_count;
    }

    if (loaded_count > 0) {
        printf("Loaded data from files: %d Patients, %d Doctors, %d Appointments.\n", patient_count, doctor_count, appointment_count);
    }
}
