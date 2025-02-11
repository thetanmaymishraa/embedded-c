#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void displayStudents(int reg[], char names[][20], float cgpa[], int size);
void sortByName(int reg[], char names[][20], float cgpa[], int size);
void sortByCGPA(int reg[], char names[][20], float cgpa[], int size);
void findHighestCGPA(int reg[], char names[][20], float cgpa[], int size);
void findLowestCGPA(int reg[], char names[][20], float cgpa[], int size);
void findCGPAByReg(int reg[], char names[][20], float cgpa[], int size, int searchReg);

int main() {
    int size;
    printf("Enter the number of students: ");
    scanf("%d", &size);
    
    int *reg = (int *)malloc(size * sizeof(int));
    char (*names)[20] = (char (*)[20])malloc(size * 20 * sizeof(char));
    float *cgpa = (float *)malloc(size * sizeof(float));
    
    printf("Enter details for %d students:\n", size);
    for (int i = 0; i < size; i++) {
        printf("Enter Registration Number for student %d: ", i + 1);
        scanf("%d", &reg[i]);
        printf("Enter Name for student %d: ", i + 1);
        scanf("%s", names[i]);
        printf("Enter CGPA for student %d: ", i + 1);
        scanf("%f", &cgpa[i]);
    }
    
    printf("\nStudents sorted by Name:\n");
    sortByName(reg, names, cgpa, size);
    displayStudents(reg, names, cgpa, size);
    
    printf("\nStudents sorted by CGPA (Descending):\n");
    sortByCGPA(reg, names, cgpa, size);
    displayStudents(reg, names, cgpa, size);
    
    printf("\nStudent with the highest CGPA:\n");
    findHighestCGPA(reg, names, cgpa, size);
    
    printf("\nStudent with the lowest CGPA:\n");
    findLowestCGPA(reg, names, cgpa, size);
    
    int searchReg;
    printf("\nEnter Registration Number to search CGPA: ");
    scanf("%d", &searchReg);
    findCGPAByReg(reg, names, cgpa, size, searchReg);
    
    free(reg);
    free(names);
    free(cgpa);
    
    return 0;
}

void displayStudents(int reg[], char names[][20], float cgpa[], int size) {
    printf("%-10s %-10s %-5s\n", "Reg No", "Name", "CGPA");
    printf("----------------------------\n");
    for (int i = 0; i < size; i++) {
        printf("%-10d %-10s %.2f\n", reg[i], names[i], cgpa[i]);
    }
}

void sortByName(int reg[], char names[][20], float cgpa[], int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (strcmp(names[i], names[j]) > 0) {
                int tempReg = reg[i];
                reg[i] = reg[j];
                reg[j] = tempReg;
                
                char tempName[20];
                strcpy(tempName, names[i]);
                strcpy(names[i], names[j]);
                strcpy(names[j], tempName);
                
                float tempCGPA = cgpa[i];
                cgpa[i] = cgpa[j];
                cgpa[j] = tempCGPA;
            }
        }
    }
}

void sortByCGPA(int reg[], char names[][20], float cgpa[], int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (cgpa[i] < cgpa[j]) {
                int tempReg = reg[i];
                reg[i] = reg[j];
                reg[j] = tempReg;
                
                char tempName[20];
                strcpy(tempName, names[i]);
                strcpy(names[i], names[j]);
                strcpy(names[j], tempName);
                
                float tempCGPA = cgpa[i];
                cgpa[i] = cgpa[j];
                cgpa[j] = tempCGPA;
            }
        }
    }
}

void findHighestCGPA(int reg[], char names[][20], float cgpa[], int size) {
    int maxIndex = 0;
    for (int i = 1; i < size; i++) {
        if (cgpa[i] > cgpa[maxIndex]) {
            maxIndex = i;
        }
    }
    printf("%-10s %-10s %-5s\n", "Reg No", "Name", "CGPA");
    printf("----------------------------\n");
    printf("%-10d %-10s %.2f\n", reg[maxIndex], names[maxIndex], cgpa[maxIndex]);
}

void findLowestCGPA(int reg[], char names[][20], float cgpa[], int size) {
    int minIndex = 0;
    for (int i = 1; i < size; i++) {
        if (cgpa[i] < cgpa[minIndex]) {
            minIndex = i;
        }
    }
    printf("%-10s %-10s %-5s\n", "Reg No", "Name", "CGPA");
    printf("----------------------------\n");
    printf("%-10d %-10s %.2f\n", reg[minIndex], names[minIndex], cgpa[minIndex]);
}

void findCGPAByReg(int reg[], char names[][20], float cgpa[], int size, int searchReg) {
    for (int i = 0; i < size; i++) {
        if (reg[i] == searchReg) {
            printf("Student Found:\n");
            printf("%-10s %-10s %-5s\n", "Reg No", "Name", "CGPA");
            printf("----------------------------\n");
            printf("%-10d %-10s %.2f\n", reg[i], names[i], cgpa[i]);
            return;
        }
    }
    printf("Student with Reg No %d not found.\n", searchReg);
}
