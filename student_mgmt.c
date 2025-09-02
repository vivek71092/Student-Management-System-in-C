/*
 * Student Management System - Complete Source Code
 * A comprehensive CRUD application in C with file handling
 * Features: Add, Display, Search, Update, Delete student records
 * Author: Professional C Programming Tutorial
 * Version: 1.0
 * Date: 2025
 * 
 * Compile: gcc -o student_mgmt student_mgmt.c
 * Run: ./student_mgmt (Linux/Mac) or student_mgmt.exe (Windows)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Structure definition for Student
typedef struct {
    int roll_no;
    char name[50];
    char department[50];
    char course[30];
    int year_joined;
    float gpa;
} Student;

// Function prototypes
void displayMenu();
void addStudent();
void displayAll();
void searchStudent();
void updateStudent();
void deleteStudent();
int isDuplicate(int roll_no);
void clearInputBuffer();
void printHeader();
void printStudent(Student s);
void pressEnterToContinue();
void displayWelcome();
void exportToCSV();
void displayStatistics();

// Global constants
const char *DB_FILE = "students.dat";
const char *TEMP_FILE = "temp.dat";
const char *CSV_FILE = "students_export.csv";

// Main function
int main() {
    int choice;
    
    displayWelcome();
    
    while(1) {
        displayMenu();
        
        printf("Enter your choice: ");
        if(scanf("%d", &choice) != 1) {
            printf("\n⚠ Invalid input! Please enter a number.\n");
            clearInputBuffer();
            pressEnterToContinue();
            continue;
        }
        clearInputBuffer();
        
        switch(choice) {
            case 1: 
                addStudent(); 
                break;
            case 2: 
                displayAll(); 
                break;
            case 3: 
                searchStudent(); 
                break;
            case 4: 
                updateStudent(); 
                break;
            case 5: 
                deleteStudent(); 
                break;
            case 6:
                displayStatistics();
                break;
            case 7:
                exportToCSV();
                break;
            case 8: 
                printf("\n╔════════════════════════════════════════╗\n");
                printf("║  Thank you for using our system!      ║\n");
                printf("║  Have a great day! 👋                 ║\n");
                printf("╚════════════════════════════════════════╝\n\n");
                exit(0);
            default:
                printf("\n⚠ Invalid choice! Please select 1-8.\n");
                pressEnterToContinue();
        }
    }
    
    return 0;
}

// Display welcome screen
void displayWelcome() {
    printf("\n╔════════════════════════════════════════════════╗\n");
    printf("║                                                ║\n");
    printf("║     STUDENT MANAGEMENT SYSTEM v1.0            ║\n");
    printf("║     Professional Edition - 2025               ║\n");
    printf("║                                                ║\n");
    printf("║     Welcome to the Future of Education        ║\n");
    printf("║                                                ║\n");
    printf("╚════════════════════════════════════════════════╝\n");
    printf("\nPress Enter to continue...");
    getchar();
}

// Display main menu
void displayMenu() {
    printf("\n┌────────────────────────────────────────────────┐\n");
    printf("│                 MAIN MENU                      │\n");
    printf("├────────────────────────────────────────────────┤\n");
    printf("│  1. 📝 Add New Student                         │\n");
    printf("│  2. 📋 Display All Students                    │\n");
    printf("│  3. 🔍 Search Student                          │\n");
    printf("│  4. ✏️  Update Student Details                  │\n");
    printf("│  5. 🗑️  Delete Student                         │\n");
    printf("│  6. 📊 View Statistics                         │\n");
    printf("│  7. 💾 Export to CSV                           │\n");
    printf("│  8. 🚪 Exit                                    │\n");
    printf("└────────────────────────────────────────────────┘\n");
}

// Clear input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Press enter to continue
void pressEnterToContinue() {
    printf("\nPress Enter to continue...");
    getchar();
}

// Check for duplicate roll number
int isDuplicate(int roll_no) {
    FILE *fp = fopen(DB_FILE, "rb");
    if(fp == NULL) return 0;
    
    Student temp;
    while(fread(&temp, sizeof(Student), 1, fp) == 1) {
        if(temp.roll_no == roll_no) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

// Add new student
void addStudent() {
    FILE *fp;
    Student newStudent;
    
    fp = fopen(DB_FILE, "ab");
    if(fp == NULL) {
        printf("\n⚠ Error: Could not open database file!\n");
        printf("Check if you have write permissions.\n");
        pressEnterToContinue();
        return;
    }
    
    printf("\n╔════════════════════════════════════════════════╗\n");
    printf("║              ADD NEW STUDENT                   ║\n");
    printf("╚════════════════════════════════════════════════╝\n");
    
    // Get roll number
    printf("\nEnter Roll Number: ");
    while(scanf("%d", &newStudent.roll_no) != 1 || newStudent.roll_no <= 0) {
        printf("⚠ Please enter a valid positive number: ");
        clearInputBuffer();
    }
    clearInputBuffer();
    
    // Check for duplicate
    if(isDuplicate(newStudent.roll_no)) {
        printf("\n⚠ Error: Roll number %d already exists!\n", newStudent.roll_no);
        fclose(fp);
        pressEnterToContinue();
        return;
    }
    
    // Get name
    printf("Enter Name: ");
    fgets(newStudent.name, sizeof(newStudent.name), stdin);
    newStudent.name[strcspn(newStudent.name, "\n")] = '\0';
    
    // Validate name is not empty
    if(strlen(newStudent.name) == 0) {
        printf("\n⚠ Error: Name cannot be empty!\n");
        fclose(fp);
        pressEnterToContinue();
        return;
    }
    
    // Get department
    printf("Enter Department (CS/EE/ME/CE/Other): ");
    fgets(newStudent.department, sizeof(newStudent.department), stdin);
    newStudent.department[strcspn(newStudent.department, "\n")] = '\0';
    
    // Get course
    printf("Enter Course: ");
    fgets(newStudent.course, sizeof(newStudent.course), stdin);
    newStudent.course[strcspn(newStudent.course, "\n")] = '\0';
    
    // Get year
    printf("Enter Year of Joining (2000-2025): ");
    while(scanf("%d", &newStudent.year_joined) != 1 || 
          newStudent.year_joined < 2000 || 
          newStudent.year_joined > 2025) {
        printf("⚠ Please enter a valid year (2000-2025): ");
        clearInputBuffer();
    }
    clearInputBuffer();
    
    // Get GPA
    printf("Enter GPA (0.0-4.0): ");
    while(scanf("%f", &newStudent.gpa) != 1 || 
          newStudent.gpa < 0.0 || 
          newStudent.gpa > 4.0) {
        printf("⚠ Please enter a valid GPA (0.0-4.0): ");
        clearInputBuffer();
    }
    clearInputBuffer();
    
    // Write to file
    if(fwrite(&newStudent, sizeof(Student), 1, fp) != 1) {
        printf("\n⚠ Error: Failed to save student data!\n");
    } else {
        printf("\n╔════════════════════════════════════════════════╗\n");
        printf("║     ✓ Student added successfully!              ║\n");
        printf("║     Roll Number %d has been registered.        ║\n", newStudent.roll_no);
        printf("╚════════════════════════════════════════════════╝\n");
    }
    
    fclose(fp);
    pressEnterToContinue();
}

// Print table header
void printHeader() {
    printf("\n╔════════╦══════════════════════════════╦═══════════════╦════════════════════╦══════╦═════╗\n");
    printf("║ Roll # ║ Name                         ║ Department    ║ Course             ║ Year ║ GPA ║\n");
    printf("╠════════╬══════════════════════════════╬═══════════════╬════════════════════╬══════╬═════╣\n");
}

// Print single student in table format
void printStudent(Student s) {
    printf("║ %-6d ║ %-28s ║ %-13s ║ %-18s ║ %4d ║ %.1f ║\n",
           s.roll_no, s.name, s.department, s.course, s.year_joined, s.gpa);
}

// Display all students
void displayAll() {
    FILE *fp;
    Student student;
    int count = 0;
    
    fp = fopen(DB_FILE, "rb");
    if(fp == NULL) {
        printf("\n╔════════════════════════════════════════════════╗\n");
        printf("║     ⚠ No records found!                        ║\n");
        printf("║     Database is empty.                         ║\n");
        printf("║     Add some students first.                   ║\n");
        printf("╚════════════════════════════════════════════════╝\n");
        pressEnterToContinue();
        return;
    }
    
    printHeader();
    
    while(fread(&student, sizeof(Student), 1, fp) == 1) {
        printStudent(student);
        count++;
    }
    
    printf("╚════════╩══════════════════════════════╩═══════════════╩════════════════════╩══════╩═════╝\n");
    printf("\nTotal Students: %d\n", count);
    
    fclose(fp);
    pressEnterToContinue();
}

// Search for a student
void searchStudent() {
    FILE *fp;
    Student student;
    int searchRoll, found = 0;
    
    printf("\n╔════════════════════════════════════════════════╗\n");
    printf("║              SEARCH STUDENT                    ║\n");
    printf("╚════════════════════════════════════════════════╝\n");
    
    printf("\nEnter Roll Number to search: ");
    if(scanf("%d", &searchRoll) != 1) {
        printf("\n⚠ Invalid input!\n");
        clearInputBuffer();
        pressEnterToContinue();
        return;
    }
    clearInputBuffer();
    
    fp = fopen(DB_FILE, "rb");
    if(fp == NULL) {
        printf("\n⚠ Database is empty!\n");
        pressEnterToContinue();
        return;
    }
    
    while(fread(&student, sizeof(Student), 1, fp) == 1) {
        if(student.roll_no == searchRoll) {
            printf("\n╔════════════════════════════════════════════════╗\n");
            printf("║           ✓ STUDENT FOUND!                     ║\n");
            printf("╠════════════════════════════════════════════════╣\n");
            printf("║  Roll Number : %-32d ║\n", student.roll_no);
            printf("║  Name        : %-32s ║\n", student.name);
            printf("║  Department  : %-32s ║\n", student.department);
            printf("║  Course      : %-32s ║\n", student.course);
            printf("║  Year Joined : %-32d ║\n", student.year_joined);
            printf("║  GPA         : %-32.2f ║\n", student.gpa);
            printf("╚════════════════════════════════════════════════╝\n");
            found = 1;
            break;
        }
    }
    
    if(!found) {
        printf("\n╔════════════════════════════════════════════════╗\n");
        printf("║     ⚠ Student with Roll Number %d not found!   ║\n", searchRoll);
        printf("╚════════════════════════════════════════════════╝\n");
    }
    
    fclose(fp);
    pressEnterToContinue();
}

// Update student details
void updateStudent() {
    FILE *fp;
    Student student;
    int searchRoll, found = 0;
    long pos;
    char buffer[100];
    
    printf("\n╔════════════════════════════════════════════════╗\n");
    printf("║            UPDATE STUDENT DETAILS              ║\n");
    printf("╚════════════════════════════════════════════════╝\n");
    
    printf("\nEnter Roll Number to update: ");
    if(scanf("%d", &searchRoll) != 1) {
        printf("\n⚠ Invalid input!\n");
        clearInputBuffer();
        pressEnterToContinue();
        return;
    }
    clearInputBuffer();
    
    fp = fopen(DB_FILE, "rb+");
    if(fp == NULL) {
        printf("\n⚠ Database error! Cannot open file.\n");
        pressEnterToContinue();
        return;
    }
    
    while(fread(&student, sizeof(Student), 1, fp) == 1) {
        if(student.roll_no == searchRoll) {
            found = 1;
            pos = ftell(fp) - sizeof(Student);
            
            printf("\n┌─── Current Details ───┐\n");
            printf("│ Name       : %s\n", student.name);
            printf("│ Department : %s\n", student.department);
            printf("│ Course     : %s\n", student.course);
            printf("│ Year       : %d\n", student.year_joined);
            printf("│ GPA        : %.2f\n", student.gpa);
            printf("└───────────────────────┘\n");
            
            printf("\n📝 Enter new details (press Enter to keep current):\n\n");
            
            // Update name
            printf("New Name [%s]: ", student.name);
            fgets(buffer, sizeof(buffer), stdin);
            if(buffer[0] != '\n') {
                buffer[strcspn(buffer, "\n")] = '\0';
                strcpy(student.name, buffer);
            }
            
            // Update department
            printf("New Department [%s]: ", student.department);
            fgets(buffer, sizeof(buffer), stdin);
            if(buffer[0] != '\n') {
                buffer[strcspn(buffer, "\n")] = '\0';
                strcpy(student.department, buffer);
            }
            
            // Update course
            printf("New Course [%s]: ", student.course);
            fgets(buffer, sizeof(buffer), stdin);
            if(buffer[0] != '\n') {
                buffer[strcspn(buffer, "\n")] = '\0';
                strcpy(student.course, buffer);
            }
            
            // Update year
            printf("New Year [%d]: ", student.year_joined);
            fgets(buffer, sizeof(buffer), stdin);
            if(buffer[0] != '\n') {
                int newYear;
                if(sscanf(buffer, "%d", &newYear) == 1 && 
                   newYear >= 2000 && newYear <= 2025) {
                    student.year_joined = newYear;
                }
            }
            
            // Update GPA
            printf("New GPA [%.2f]: ", student.gpa);
            fgets(buffer, sizeof(buffer), stdin);
            if(buffer[0] != '\n') {
                float newGpa;
                if(sscanf(buffer, "%f", &newGpa) == 1 && 
                   newGpa >= 0.0 && newGpa <= 4.0) {
                    student.gpa = newGpa;
                }
            }
            
            // Seek back and write updated record
            fseek(fp, pos, SEEK_SET);
            
            if(fwrite(&student, sizeof(Student), 1, fp) != 1) {
                printf("\n⚠ Error: Update failed!\n");
            } else {
                printf("\n╔════════════════════════════════════════════════╗\n");
                printf("║     ✓ Student record updated successfully!     ║\n");
                printf("╚════════════════════════════════════════════════╝\n");
            }
            break;
        }
    }
    
    if(!found) {
        printf("\n⚠ Student with Roll Number %d not found!\n", searchRoll);
    }
    
    fclose(fp);
    pressEnterToContinue();
}

// Delete a student
void deleteStudent() {
    FILE *fp, *temp;
    Student student;
    int searchRoll, found = 0;
    char confirm;
    
    printf("\n╔════════════════════════════════════════════════╗\n");
    printf("║              DELETE STUDENT                    ║\n");
    printf("╚════════════════════════════════════════════════╝\n");
    
    printf("\nEnter Roll Number to delete: ");
    if(scanf("%d", &searchRoll) != 1) {
        printf("\n⚠ Invalid input!\n");
        clearInputBuffer();
        pressEnterToContinue();
        return;
    }
    clearInputBuffer();
    
    fp = fopen(DB_FILE, "rb");
    if(fp == NULL) {
        printf("\n⚠ Database is empty!\n");
        pressEnterToContinue();
        return;
    }
    
    // First, find and display the student
    Student toDelete;
    while(fread(&student, sizeof(Student), 1, fp) == 1) {
        if(student.roll_no == searchRoll) {
            toDelete = student;
            found = 1;
            break;
        }
    }
    
    if(!found) {
        printf("\n⚠ Student with Roll Number %d not found!\n", searchRoll);
        fclose(fp);
        pressEnterToContinue();
        return;
    }
    
    // Show student details and confirm deletion
    printf("\n┌─── Student to Delete ───┐\n");
    printf("│ Roll Number : %d\n", toDelete.roll_no);
    printf("│ Name        : %s\n", toDelete.name);
    printf("│ Department  : %s\n", toDelete.department);
    printf("│ GPA         : %.2f\n", toDelete.gpa);
    printf("└─────────────────────────┘\n");
    
    printf("\n⚠ Are you sure you want to delete this student? (y/n): ");
    scanf(" %c", &confirm);
    clearInputBuffer();
    
    if(confirm != 'y' && confirm != 'Y') {
        printf("\n✓ Deletion cancelled.\n");
        fclose(fp);
        pressEnterToContinue();
        return;
    }
    
    // Reset file pointer and create temp file
    rewind(fp);
    temp = fopen(TEMP_FILE, "wb");
    if(temp == NULL) {
        printf("\n⚠ System error! Cannot create temporary file.\n");
        fclose(fp);
        pressEnterToContinue();
        return;
    }
    
    // Copy all records except the one to delete
    while(fread(&student, sizeof(Student), 1, fp) == 1) {
        if(student.roll_no != searchRoll) {
            fwrite(&student, sizeof(Student), 1, temp);
        }
    }
    
    fclose(fp);
    fclose(temp);
    
    // Replace original file with temp file
    if(remove(DB_FILE) != 0) {
        printf("\n⚠ Error: Cannot delete original file!\n");
        pressEnterToContinue();
        return;
    }
    
    if(rename(TEMP_FILE, DB_FILE) != 0) {
        printf("\n⚠ Error: Cannot rename temporary file!\n");
        pressEnterToContinue();
        return;
    }
    
    printf("\n╔════════════════════════════════════════════════╗\n");
    printf("║     ✓ Student deleted successfully!            ║\n");
    printf("╚════════════════════════════════════════════════╝\n");
    
    pressEnterToContinue();
}

// Display statistics
void displayStatistics() {
    FILE *fp;
    Student student;
    int count = 0;
    float totalGPA = 0.0;
    float highestGPA = 0.0;
    float lowestGPA = 4.0;
    char topStudent[50] = "";
    char weakStudent[50] = "";
    
    fp = fopen(DB_FILE, "rb");
    if(fp == NULL) {
        printf("\n⚠ No data available for statistics!\n");
        pressEnterToContinue();
        return;
    }
    
    while(fread(&student, sizeof(Student), 1, fp) == 1) {
        count++;
        totalGPA += student.gpa;
        
        if(student.gpa > highestGPA) {
            highestGPA = student.gpa;
            strcpy(topStudent, student.name);
        }
        
        if(student.gpa < lowestGPA) {
            lowestGPA = student.gpa;
            strcpy(weakStudent, student.name);
        }
    }
    
    fclose(fp);
    
    if(count == 0) {
        printf("\n⚠ No students in database!\n");
        pressEnterToContinue();
        return;
    }
    
    float averageGPA = totalGPA / count;
    
    printf("\n╔════════════════════════════════════════════════╗\n");
    printf("║            📊 DATABASE STATISTICS              ║\n");
    printf("╠════════════════════════════════════════════════╣\n");
    printf("║  Total Students    : %-25d ║\n", count);
    printf("║  Average GPA       : %-25.2f ║\n", averageGPA);
    printf("║  Highest GPA       : %-25.2f ║\n", highestGPA);
    printf("║  Top Performer     : %-25s ║\n", topStudent);
    printf("║  Lowest GPA        : %-25.2f ║\n", lowestGPA);
    printf("║  Needs Improvement : %-25s ║\n", weakStudent);
    printf("╚════════════════════════════════════════════════╝\n");
    
    // GPA Distribution
    printf("\n📈 GPA Distribution:\n");
    printf("   Excellent (3.5-4.0): ");
    
    fp = fopen(DB_FILE, "rb");
    int excellent = 0, good = 0, average = 0, poor = 0;
    
    while(fread(&student, sizeof(Student), 1, fp) == 1) {
        if(student.gpa >= 3.5) excellent++;
        else if(student.gpa >= 3.0) good++;
        else if(student.gpa >= 2.0) average++;
        else poor++;
    }
    
    fclose(fp);
    
    printf("%d students\n", excellent);
    printf("   Good (3.0-3.49)    : %d students\n", good);
    printf("   Average (2.0-2.99) : %d students\n", average);
    printf("   Poor (Below 2.0)   : %d students\n", poor);
    
    pressEnterToContinue();
}

// Export data to CSV
void exportToCSV() {
    FILE *fp, *csv;
    Student student;
    int count = 0;
    
    fp = fopen(DB_FILE, "rb");
    if(fp == NULL) {
        printf("\n⚠ No data to export!\n");
        pressEnterToContinue();
        return;
    }
    
    csv = fopen(CSV_FILE, "w");
    if(csv == NULL) {
        printf("\n⚠ Cannot create CSV file!\n");
        fclose(fp);
        pressEnterToContinue();
        return;
    }
    
    // Write CSV header
    fprintf(csv, "Roll Number,Name,Department,Course,Year Joined,GPA\n");
    
    // Write student data
    while(fread(&student, sizeof(Student), 1, fp) == 1) {
        fprintf(csv, "%d,%s,%s,%s,%d,%.2f\n",
                student.roll_no,
                student.name,
                student.department,
                student.course,
                student.year_joined,
                student.gpa);
        count++;
    }
    
    fclose(fp);
    fclose(csv);
    
    printf("\n╔════════════════════════════════════════════════╗\n");
    printf("║     ✓ Export Successful!                       ║\n");
    printf("║     %d records exported to %s     ║\n", count, CSV_FILE);
    printf("║     You can open this file in Excel.           ║\n");
    printf("╚════════════════════════════════════════════════╝\n");
    
    pressEnterToContinue();
}
