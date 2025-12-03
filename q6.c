#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_NAME_LENGTH 100
#define MAX_BATCH_LENGTH 20
#define MAX_MEMBERSHIP_LENGTH 10
#define INITIAL_CAPACITY 10

typedef struct {
    int studentID;
    char fullName[MAX_NAME_LENGTH];
    char batch[MAX_BATCH_LENGTH];
    char membershipType[MAX_MEMBERSHIP_LENGTH];
    char registrationDate[11]; // YYYY-MM-DD format
    char dateOfBirth[11];      // YYYY-MM-DD format
    char interestIn[MAX_MEMBERSHIP_LENGTH];
} Student;

typedef struct {
    Student *students;
    size_t size;
    size_t capacity;
} StudentDatabase;

void loadDatabase(StudentDatabase *db, const char *filename);
void saveDatabase(const StudentDatabase *db, const char *filename);
void addStudent(StudentDatabase *db, Student s);
void updateStudent(StudentDatabase *db, int studentID, const char *batch, const char *membershipType);
void deleteStudent(StudentDatabase *db, int studentID);
void displayAllStudents(const StudentDatabase *db);
void generateBatchWiseReport(const StudentDatabase *db, const char *batch, const char *interest);
void freeDatabase(StudentDatabase *db);

int main() {
    StudentDatabase db = {NULL, 0, 0};
    loadDatabase(&db, "members.dat");
    
    int choice;
    do {
        printf("\nMenu:\n");
        printf("1. Register a new student\n");
        printf("2. Update student's information\n");
        printf("3. Delete a student registration\n");
        printf("4. View all registrations\n");
        printf("5. Generate batch-wise report\n");
        printf("6. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar();  // Consume newline character

        if (choice == 1) {
            Student newStudent;
            printf("Enter Student ID: ");
            scanf("%d", &newStudent.studentID);
            getchar();  // Consume newline character
            printf("Enter Full Name: ");
            fgets(newStudent.fullName, MAX_NAME_LENGTH, stdin);
            newStudent.fullName[strcspn(newStudent.fullName, "\n")] = 0; // Remove newline
            printf("Enter Batch (CS/SE/Cyber Security/AI): ");
            fgets(newStudent.batch, MAX_BATCH_LENGTH, stdin);
            newStudent.batch[strcspn(newStudent.batch, "\n")] = 0;
            printf("Enter Membership Type (IEEE/ACM): ");
            fgets(newStudent.membershipType, MAX_MEMBERSHIP_LENGTH, stdin);
            newStudent.membershipType[strcspn(newStudent.membershipType, "\n")] = 0;
            printf("Enter Registration Date (YYYY-MM-DD): ");
            scanf("%s", newStudent.registrationDate);
            getchar(); // Consume newline character
            printf("Enter Date of Birth (YYYY-MM-DD): ");
            scanf("%s", newStudent.dateOfBirth);
            getchar(); // Consume newline character
            printf("Enter Interest In (IEEE/ACM/Both): ");
            fgets(newStudent.interestIn, MAX_MEMBERSHIP_LENGTH, stdin);
            newStudent.interestIn[strcspn(newStudent.interestIn, "\n")] = 0;

            addStudent(&db, newStudent);
        } else if (choice == 2) {
            int studentID;
            char newBatch[MAX_BATCH_LENGTH], newMembership[MAX_MEMBERSHIP_LENGTH];
            printf("Enter Student ID to update: ");
            scanf("%d", &studentID);
            getchar();  // Consume newline character
            printf("Enter new Batch: ");
            fgets(newBatch, MAX_BATCH_LENGTH, stdin);
            newBatch[strcspn(newBatch, "\n")] = 0;
            printf("Enter new Membership Type: ");
            fgets(newMembership, MAX_MEMBERSHIP_LENGTH, stdin);
            newMembership[strcspn(newMembership, "\n")] = 0;

            updateStudent(&db, studentID, newBatch, newMembership);
        } else if (choice == 3) {
            int studentID;
            printf("Enter Student ID to delete: ");
            scanf("%d", &studentID);
            deleteStudent(&db, studentID);
        } else if (choice == 4) {
            displayAllStudents(&db);
        } else if (choice == 5) {
            char batch[MAX_BATCH_LENGTH], interest[MAX_MEMBERSHIP_LENGTH];
            printf("Enter Batch for report: ");
            fgets(batch, MAX_BATCH_LENGTH, stdin);
            batch[strcspn(batch, "\n")] = 0;
            printf("Enter Interest (IEEE/ACM/Both): ");
            fgets(interest, MAX_MEMBERSHIP_LENGTH, stdin);
            interest[strcspn(interest, "\n")] = 0;
            generateBatchWiseReport(&db, batch, interest);
        }
    } while (choice != 6);

    saveDatabase(&db, "members.dat");
    freeDatabase(&db);
    return 0;
}

void loadDatabase(StudentDatabase *db, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) return;

    db->students = malloc(INITIAL_CAPACITY * sizeof(Student));
    db->capacity = INITIAL_CAPACITY;
    db->size = 0;

    while (fread(&db->students[db->size], sizeof(Student), 1, file)) {
        db->size++;
        if (db->size >= db->capacity) {
            db->capacity *= 2;
            db->students = realloc(db->students, db->capacity * sizeof(Student));
        }
    }

    fclose(file);
}

void saveDatabase(const StudentDatabase *db, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) return;

    fwrite(db->students, sizeof(Student), db->size, file);
    fclose(file);
}

void addStudent(StudentDatabase *db, Student s) {
    for (size_t i = 0; i < db->size; i++) {
        if (db->students[i].studentID == s.studentID) {
            printf("Student ID already exists!\n");
            return;
        }
    }

    if (db->size >= db->capacity) {
        db->capacity *= 2;
        db->students = realloc(db->students, db->capacity * sizeof(Student));
    }
    db->students[db->size++] = s;
}

void updateStudent(StudentDatabase *db, int studentID, const char *batch, const char *membershipType) {
    for (size_t i = 0; i < db->size; i++) {
        if (db->students[i].studentID == studentID) {
            strcpy(db->students[i].batch, batch);
            strcpy(db->students[i].membershipType, membershipType);
            return;
        }
    }
    printf("Student ID not found!\n");
}

void deleteStudent(StudentDatabase *db, int studentID) {
    for (size_t i = 0; i < db->size; i++) {
        if (db->students[i].studentID == studentID) {
            for (size_t j = i; j < db->size - 1; j++) {
                db->students[j] = db->students[j + 1];
            }
            db->size--;
            return;
        }
    }
    printf("Student ID not found!\n");
}

void displayAllStudents(const StudentDatabase *db) {
    for (size_t i = 0; i < db->size; i++) {
        Student s = db->students[i];
        printf("ID: %d, Name: %s, Batch: %s, Membership: %s, Reg. Date: %s, DOB: %s, Interest: %s\n",
               s.studentID, s.fullName, s.batch, s.membershipType, s.registrationDate, s.dateOfBirth, s.interestIn);
    }
}

void generateBatchWiseReport(const StudentDatabase *db, const char *batch, const char *interest) {
    for (size_t i = 0; i < db->size; i++) {
        Student s = db->students[i];
        if (strcmp(s.batch, batch) == 0 && 
            (strcmp(s.interestIn, interest) == 0 || strcmp(interest, "Both") == 0)) {
            printf("ID: %d, Name: %s, Membership: %s\n", s.studentID, s.fullName, s.membershipType);
        }
    }
}

void freeDatabase(StudentDatabase *db) {
    free(db->students);
}