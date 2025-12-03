#include <stdio.h>
#include <string.h>

#define MAX_EMPLOYEES 100
#define NAME_LENGTH 50
#define DESIGNATION_LENGTH 50

typedef struct {
    int id;
    char name[NAME_LENGTH];
    char designation[DESIGNATION_LENGTH];
    float salary;
} Employee;

void displayEmployees(Employee employees[], int count) {
    printf("\n%-10s %-20s %-15s %-10s\n", "ID", "Name", "Designation", "Salary");
    printf("-------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-10d %-20s %-15s $%-10.2f\n", employees[i].id, employees[i].name, employees[i].designation, employees[i].salary);
    }
}

void findHighestSalary(Employee employees[], int count) {
    if (count == 0) {
        printf("No employee records available.\n");
        return;
    }
    Employee *highest = &employees[0];
    for (int i = 1; i < count; i++) {
        if (employees[i].salary > highest->salary) {
            highest = &employees[i];
        }
    }
    printf("\nEmployee with Highest Salary:\n");
    printf("ID: %d\nName: %s\nDesignation: %s\nSalary: $%.2f\n", highest->id, highest->name, highest->designation, highest->salary);
}

void searchEmployee(Employee employees[], int count) {
    char searchTerm[NAME_LENGTH];
    printf("Enter Employee ID or Name to search: ");
    fgets(searchTerm, sizeof(searchTerm), stdin);
    searchTerm[strcspn(searchTerm, "\n")] = 0;

    int isID = 1;
    for (int i = 0; searchTerm[i] != '\0'; i++) {
        if (searchTerm[i] < '0' || searchTerm[i] > '9') {
            isID = 0;
            break;
        }
    }

    if (isID) {
        int id = atoi(searchTerm);
        for (int i = 0; i < count; i++) {
            if (employees[i].id == id) {
                printf("\nEmployee Found:\n");
                printf("ID: %d\nName: %s\nDesignation: %s\nSalary: $%.2f\n", employees[i].id, employees[i].name, employees[i].designation, employees[i].salary);
                return;
            }
        }
    } else {
        for (int i = 0; i < count; i++) {
            if (strcmp(employees[i].name, searchTerm) == 0) {
                printf("\nEmployee Found:\n");
                printf("ID: %d\nName: %s\nDesignation: %s\nSalary: $%.2f\n", employees[i].id, employees[i].name, employees[i].designation, employees[i].salary);
                return;
            }
        }
    }

    printf("Employee not found.\n");
}

void giveBonus(Employee employees[], int count, int employeeId) {
    for (int i = 0; i < count; i++) {
        if (employees[i].id == employeeId) {
            if (employees[i].salary < 50000) {
                employees[i].salary *= 1.10; // Apply 10% bonus
                printf("Bonus given to %s. New Salary: $%.2f\n", employees[i].name, employees[i].salary);
            } else {
                printf("No bonus applied to %s (Salary: $%.2f)\n", employees[i].name, employees[i].salary);
            }
            return;
        }
    }
    printf("Employee with ID %d not found, bonus not applied.\n", employeeId);
}

int main() {
    Employee employees[MAX_EMPLOYEES];
    int count = 0;

    printf("Enter the number of employees (max %d): ", MAX_EMPLOYEES);
    scanf("%d", &count);
    getchar(); // Clear newline character

    for (int i = 0; i < count; i++) {
        printf("\nEnter details for employee %d:\n", i + 1);
        printf("ID: ");
        scanf("%d", &employees[i].id);
        getchar(); // Clear newline character
        printf("Name: ");
        fgets(employees[i].name, NAME_LENGTH, stdin);
        employees[i].name[strcspn(employees[i].name, "\n")] = 0;
        
        printf("Designation: ");
        fgets(employees[i].designation, DESIGNATION_LENGTH, stdin);
        employees[i].designation[strcspn(employees[i].designation, "\n")] = 0;

        printf("Salary: ");
        scanf("%f", &employees[i].salary);
    }

    displayEmployees(employees, count);
    findHighestSalary(employees, count);
    searchEmployee(employees, count);
    
    int employeeId;
    printf("\nEnter the Employee ID to give a bonus: ");
    scanf("%d", &employeeId);
    
    giveBonus(employees, count, employeeId);

    return 0;
}