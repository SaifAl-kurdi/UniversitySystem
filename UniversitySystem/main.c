#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<time.h>

struct Department {
    int departmentID;
    char departmentName[100];
    int phoneNumber;
    char email[100];
    struct Department* next;
};
struct Department* d_root = NULL;

struct Student {
    int studentID;
    char studentName[100];
    int phoneNumber;
    char city[100];
    int departmentID;
    struct Student* next;
};
struct Student* s_root = NULL;

struct Course {
    int courseID;
    char courseTitle[100];
    int courseNumber;
    int departmentID;
    struct Course* next;
};
struct Course* c_root = NULL;

struct Enrollment {
    int studentID;
    int courseID;
    int year;
    char semester[20];
    struct enrollment *next;
};
struct enrollment* e_root = NULL;


#define HASH_TABLE_SIZE 100

struct StudentHashTable {
    int phoneNumber;
    struct Student* student;
    struct StudentHashTable* next;
};
struct StudentHashTable* studentHashTable[HASH_TABLE_SIZE];

int hashFunction(int id) {
    return id % HASH_TABLE_SIZE;
}

void insertIntoHashTable(struct Student* student) {
    int hashIndex = hashFunction(student->phoneNumber);

    struct StudentHashTable* newEntry = (struct StudentHashTable*) malloc(sizeof(struct StudentHashTable));
    newEntry->phoneNumber = student->phoneNumber;
    newEntry->student = student;
    newEntry->next = NULL;

    if (studentHashTable[hashIndex] == NULL) {
        studentHashTable[hashIndex] = newEntry;
    } else {
        struct StudentHashTable* current = studentHashTable[hashIndex];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newEntry;
    }
}

struct Student* searchInHashTable(int id) {
    int hashIndex = hashFunction(id);

    struct StudentHashTable* current = studentHashTable[hashIndex];
    while (current != NULL) {
        if (current->phoneNumber == id) {
            return current->student;
        }
        current = current->next;
    }
    return NULL;
}

/*-------------------------------------------------------------------*/

int years[] = {2019, 2020, 2021, 2022, 2023};
char *semesters[] = {"fall", "spring", "summer"};

void delete_enrollment(int studentID, int courseID) {
    struct Enrollment *current = e_root, *previous = NULL;

    if (current != NULL && current->studentID == studentID && current->courseID == courseID) {
        e_root = current->next;
        printf("The enrollment delete successfully\n\n");
        free(current);
        return;
    }
    while (current != NULL && (current->studentID != studentID || current->courseID != courseID)) {
        previous = current;
        current = current->next;
    }
    if (current == NULL) {
        printf("Enrollment not found in the system.\n");
        return;
    }

    printf("The enrollment delete successfully");
    previous->next = current->next;
}


int is_student_saved(int studentID) {
    struct Student* current = s_root;
    while(current != NULL) {
        if(current->studentID == studentID){
            return 1;
        }
        current = current->next;
    }
    return 0;
}

int is_course_saved(int courseID) {
    struct Course* current = c_root;
    while(current != NULL) {
        if(current->courseID == courseID){
            return 1;
        }
        current = current->next;
    }
    return 0;
}

char* get_student_name(int studentID) {
    struct Student *current = s_root;

    while (current != NULL) {
        if (current->studentID == studentID) {
            char *name = malloc(strlen(current->studentName) + 1);
            if (name) {
                strcpy(name, current->studentName);
            }
            return name;
        }
        current = current->next;
    }
    return NULL;
}

char* get_course_title(int courseID) {
    struct Course *current = c_root;

    while (current != NULL) {
        if (current->courseID == courseID) {
            char *title = malloc(strlen(current->courseTitle) + 1);
            if (title) {
                strcpy(title, current->courseTitle);
            }
            return title;
        }
        current = current->next;
    }
    return NULL;
}

void insert_enrollment(int studentID, int courseID) {
    if (!is_student_saved(studentID) || !is_course_saved(courseID)) {
        printf("Student or Course not saved in the system.\n");
        return;
    }

    struct Enrollment *new_node = malloc(sizeof(struct Enrollment));
    new_node->studentID = studentID;
    new_node->courseID = courseID;

    srand(time(0));
    new_node->year = years[rand() % 5];
    strcpy(new_node->semester, semesters[rand() % 3]);

    new_node->next = e_root;
    e_root = new_node;
    printf("The course ID add successfully for the student ID %d.\n", courseID, studentID);
}

void display_student_courses(int studentID) {
    struct Enrollment *current = e_root;
    int found = 0;

    printf("Courses enrolled by student with ID %s:\n",get_student_name(studentID));

    while (current != NULL) {
        if (current->studentID == studentID) {
            printf("Course ID: %s, Year: %d, Semester: %s\n", get_course_title(current->courseID), current->year, current->semester);
            found = 1;
        }
        current = current->next;
    }

    if (!found) {
        printf("No courses found for the given student ID.\n");
    }
}

void* createDepartmentThread(void* arg) {
    struct Department* dept = (struct Department*)malloc(sizeof(struct Department));
    int departmentID = *((int*)arg);
    switch (departmentID) {
        case 1:
            dept->departmentID = 1;
            strcpy(dept->departmentName, "Computer science");
            dept->phoneNumber = 796055236;
            strcpy(dept->email, "ComputerScience@htu.edu.jo");
            dept->next = NULL;
            d_root = dept;
        break;
        case 2:
            dept->departmentID = 2;
            strcpy(dept->departmentName, "Data science and AI");
            dept->phoneNumber = 788877661;
            strcpy(dept->email, "DataScience@htu.edu.jo");
            dept->next = NULL;
            d_root->next = dept;
        break;
        case 3:
            dept->departmentID = 3;
            strcpy(dept->departmentName, "Cybersecurity");
            dept->phoneNumber = 777766551;
            strcpy(dept->email, "Cybersecurity@htu.edu.jo");
            dept->next = NULL;
            d_root->next->next = dept;
        break;
    }
    pthread_exit(NULL);
}

void GenerateDepartmentsInformation() {
    pthread_t threads[3];
    int departmentIDs[3] = {1, 2, 3};

    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, createDepartmentThread, (void*)&departmentIDs[i]);
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }
}



void viewDepartmentByID(int id) {
    struct Department* dept = d_root;
    while (dept != NULL) {
        if (dept->departmentID == id) {
            printf("Department ID: %d\n", dept->departmentID);
            printf("Department Name: %s\n", dept->departmentName);
            printf("Phone Number: %d\n", dept->phoneNumber);
            printf("Email: %s\n", dept->email);
            return;
        }
        dept = dept->next;
    }
    printf("No department found with ID %d\n", id);
}

void viewDepartmentByName(const char* name) {
    struct Department* dept = d_root;
    while (dept != NULL) {
        if (strcmp(dept->departmentName, name) == 0) {
            printf("Department ID: %d\n", dept->departmentID);
            printf("Department Name: %s\n", dept->departmentName);
            printf("Phone Number: %d\n", dept->phoneNumber);
            printf("Email: %s\n", dept->email);
            return;
        }
        dept = dept->next;
    }
    printf("No department found with Name %s\n", name);
}

/*-------------------------------------------------------------------*/

int checkDepartment(int id) {
    struct Department* dept = d_root;
    while (dept != NULL) {
        if (dept->departmentID == id) {
            return 1;
        }
        dept = dept->next;
    }
    return 0;
}

void addStudent() {
    struct Student* newStudent = (struct Student*) malloc(sizeof(struct Student));

    printf("Add a student\n");
    printf("Student ID: ");
    scanf("%d", &(newStudent->studentID));

    printf("Student Name: ");
    scanf("%s", newStudent->studentName);

    printf("Student Phone Number: ");
    scanf("%d", &(newStudent->phoneNumber));

    printf("Student City: ");
    scanf("%s", newStudent->city);

    do {
        printf("Department ID: ");
        scanf("%d", &(newStudent->departmentID));
        if (checkDepartment(newStudent->departmentID) == 0) {
            printf("Invalid department. Please try again.\n");
        }
    } while (checkDepartment(newStudent->departmentID) == 0);

    newStudent->next = NULL;

    if (s_root == NULL) {
        s_root = newStudent;
    } else {
        struct Student* current = s_root;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newStudent;
    }
    printf("Student inserted successfully\n");
    printf("Student with the following information has been added ** %d %s %d %s %d **\n\n", newStudent->studentID, newStudent->studentName, newStudent->phoneNumber, newStudent->city, newStudent->departmentID);

}

void saveStudentsToFile() {
    FILE *file = fopen("student.dat", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    struct Student* current = s_root;
    while (current != NULL) {
        fprintf(file, "%d %s %d %s %d\n", current->studentID, current->studentName, current->phoneNumber, current->city, current->departmentID);
        current = current->next;
    }

    fclose(file);
    printf("Students data saved to file successfully.\n");
}

void readStudentsFromFile() {
    FILE* file = fopen("student.dat", "r");
    if (file == NULL) {
        return;
    }

    while (1) {
        struct Student* newStudent = (struct Student*) malloc(sizeof(struct Student));
        if (fscanf(file, "%d %s %d %s %d", &(newStudent->studentID), newStudent->studentName, &(newStudent->phoneNumber), newStudent->city, &(newStudent->departmentID)) != 5) {
            free(newStudent);
            break;
        }

        newStudent->next = NULL;

        if (s_root == NULL) {
            s_root = newStudent;
        } else {
            struct Student* current = s_root;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newStudent;
        }
    }
    fclose(file);
}

void deleteStudentByID(int id) {
    struct Student* current = s_root;

    struct Student* previous = NULL;

    if(current == NULL) {
        printf("No student found with ID %d\n", id);
        return;
    }

    while(current != NULL) {
        if(current->studentID == id) {
            if(previous == NULL) {
                s_root = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            printf("Student with ID %d deleted successfully\n", id);
            return;
        }

        previous = current;
        current = current->next;
    }

    printf("No student found with ID %d\n", id);
}

void deleteStudentByName(char* name) {
    struct Student* current = s_root;
    struct Student* previous = NULL;

    if(current == NULL) {
        printf("No student found with name %s\n", name);
        return;
    }

    while(current != NULL) {
        if(strcmp(current->studentName, name) == 0) {
            if(previous == NULL) {
                s_root = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            printf("Student with name %s deleted successfully\n", name);
            return;
        }
        previous = current;
        current = current->next;
    }

    printf("No student found with name %s\n", name);
}

void updateStudent(int id) {
    struct Student* current = s_root;

    int choice, newPhoneNumber;
    char newName[100], newCity[100];


    while(current != NULL) {
        if(current->studentID == id) {
            printf("1. Update name\n");
            printf("2. Update phone number\n");
            printf("3. Update city\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch(choice) {
                case 1:
                    printf("Enter new name: ");
                    scanf("%s", newName);
                    strcpy(current->studentName, newName);
                    printf("Name updated successfully\n");
                    break;
                case 2:
                    printf("Enter new phone number: ");
                    scanf("%d", &newPhoneNumber);
                    current->phoneNumber = newPhoneNumber;
                    printf("Phone number updated successfully\n");
                    break;
                case 3:
                    printf("Enter new city: ");
                    scanf("%s", newCity);
                    strcpy(current->city, newCity);
                    printf("City updated successfully\n");
                    break;
                default:
                    printf("Invalid choice\n");
            }
            return;
        }
        current = current->next;
    }
    printf("No student found with ID %d\n", id);
}

void displayStudentByID(int id) {
    struct Student* current = s_root;

    while(current != NULL) {
        if(current->studentID == id) {
            printf("Student ID: %d\n", current->studentID);
            printf("Student Name: %s\n", current->studentName);
            printf("Phone Number: %d\n", current->phoneNumber);
            printf("City: %s\n", current->city);
            printf("Department ID: %d\n", current->departmentID);
            return;
        }
        current = current->next;
    }
    printf("No student found with ID %d\n", id);
}


void displayStudentByName(char* name) {
    struct Student* current = s_root;

    while(current != NULL) {
        if(strcmp(current->studentName, name) == 0) {
            printf("Student ID: %d\n", current->studentID);
            printf("Student Name: %s\n", current->studentName);
            printf("Phone Number: %d\n", current->phoneNumber);
            printf("City: %s\n", current->city);
            printf("Department ID: %d\n\n", current->departmentID);
            return;
        }
        current = current->next;
    }
    printf("No student found with the name %s\n\n", name);
}


void displayStudentByPhoneNumber(int phoneNumber) {
    struct Student* current = s_root;

    while (current != NULL) {
        if (current->phoneNumber == phoneNumber) {
            printf("Student with Phone Number %d found\n", phoneNumber);
            printf("Student ID: %d\n", current->studentID);
            printf("Student Name: %s\n", current->studentName);
            printf("Phone Number: %d\n", current->phoneNumber);
            printf("City: %s\n", current->city);
            printf("Department ID: %d\n", current->departmentID);
            return;
        }
        current = current->next;
    }
    printf("No student found with Phone Number %d\n", phoneNumber);
}

/*-------------------------------------------------------------------*/

void addCourse() {
    struct Course* newCourse = (struct Course*) malloc(sizeof(struct Course));

    printf("Add a course\n");
    printf("Course ID: ");
    scanf("%d", &(newCourse->courseID));

    printf("Course Title: ");
    scanf("%s", newCourse->courseTitle);

    printf("Course Number: ");
    scanf("%d", &(newCourse->courseNumber));

    do {
        printf("Department ID: ");
        scanf("%d", &(newCourse->departmentID));
        if (checkDepartment(newCourse->departmentID) == 0) {
            printf("Invalid department. Please try again.\n");
        }
    } while (checkDepartment(newCourse->departmentID) == 0);

    newCourse->next = NULL;

    if (c_root == NULL) {
        c_root = newCourse;
    } else {
        struct Course* current = c_root;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newCourse;
    }
    printf("Course inserted successfully\n");
    printf("Course with the following information has been added ** %d %s %d %d **\n\n", newCourse->courseID, newCourse->courseTitle, newCourse->courseNumber, newCourse->departmentID);
}

void deleteCourseByID(int courseID) {
    struct Course* current = c_root;
    struct Course* previous = NULL;

    while (current != NULL) {
        if (current->courseID == courseID) {
            if (previous == NULL) {
                c_root = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            printf("Course with ID %d deleted successfully\n", courseID);
            return;
        }
        previous = current;
        current = current->next;
    }
    printf("No course found with ID %d\n", courseID);
}

void deleteCourseByTitle(char* courseTitle) {
    struct Course* current = c_root;
    struct Course* previous = NULL;

    while (current != NULL) {
        if (strcmp(current->courseTitle, courseTitle) == 0) {
            if (previous == NULL) {
                c_root = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            printf("Course with title %s deleted successfully\n", courseTitle);
            return;
        }
        previous = current;
        current = current->next;
    }
    printf("No course found with title %s\n", courseTitle);
}

void updateCourse(int courseID) {
    struct Course* current = c_root;
    int update;

    while (current != NULL) {
        if (current->courseID == courseID) {
            printf("Found course with ID %d. what do you want to update:\n", courseID);
                printf("1- Do You want to change course title.\n");
                printf("2- Do You want to change course number.\n");
                printf("3- Do You want to change department ID.\n");
                printf("4- Exit. \n");
                scanf("%d", &update);
                        switch(update){
                            case 1:
                            printf("Enter new course title: ");
                            scanf("%s", current->courseTitle);
                            printf("\nCourse updated successfully\n");
                            break;

                            case 2:
                            printf("Enter new course number: ");
                            scanf("%d", &(current->courseNumber));
                            printf("\nCourse updated successfully\n");
                            break;

                            case 3:
                            printf("Enter new department ID: ");
                            scanf("%d", &(current->departmentID));
                            printf("\nCourse updated successfully\n");
                            break;

                            default:
                                printf("Invalid choice\n");
                        }
                return;
            }
            current = current->next;
        }
    printf("No course found with ID %d\n", courseID);
}

void displayCourseByID(int courseID) {
    struct Course* current = c_root;

    while (current != NULL) {
        if (current->courseID == courseID) {
            printf("Course ID: %d\n", current->courseID);
            printf("Course Title: %s\n", current->courseTitle);
            printf("Course Number: %d\n", current->courseNumber);
            printf("Department ID: %d\n\n", current->departmentID);
            return;
        }
        current = current->next;
    }
    printf("No course found with ID %d\n", courseID);
}

void displayCourseByTitle(char* courseTitle) {
    struct Course* current = c_root;

    while (current != NULL) {
        if (strcmp(current->courseTitle, courseTitle) == 0) {
            printf("Course ID: %d\n", current->courseID);
            printf("Course Title: %s\n", current->courseTitle);
            printf("Course Number: %d\n", current->courseNumber);
            printf("Department ID: %d\n\n", current->departmentID);
            return;
        }
        current = current->next;
    }

    printf("No course found with title %s\n", courseTitle);
}

/*-------------------------------------------------------------------*/

void freeMemory() {
    struct Department* dept = d_root;
    while (dept != NULL) {
        struct Department* current = dept;
        dept = dept->next;
        free(current);
    }
    d_root = NULL;

    struct Student* student = s_root;
    while (student != NULL) {
        struct Student* current = student;
        student = student->next;
        free(current);
    }
    s_root = NULL;

    struct Course* course = c_root;
    while (course != NULL) {
        struct Course* current = course;
        course = course->next;
        free(current);
    }
    c_root = NULL;

    struct Enrollment* enroll = e_root;
    while (enroll != NULL) {
        struct enrollment* current = enroll;
        enroll = enroll->next;
        free(current);
    }
    e_root = NULL;
}

/*-------------------------------------------------------------------*/

int main()
{
    int getFromTheUser, studentID, studentPhoneNumber, courseID,departmentId, courseNumber, gradeID, departmentID, departmentPhoneNumber, phoneNumber,view, update, deletee;
    char studentName[100], courseTitle[100], studentCity[100], year[100], departmentName[20];

    GenerateDepartmentsInformation();
    readStudentsFromFile();

    do{
        printf("Please select one of these option: \n");

        printf("1- Add the student.\n");
        printf("2- Delete the student.\n");
        printf("3- Update the information for the student.\n");
        printf("4- View the student information.\n");

        printf("--------------------------------------------------\n");

        printf("5- Add the course.\n");
        printf("6- Delete the course.\n");
        printf("7- Update the course information.\n");
        printf("8- View the course information.\n");

        printf("--------------------------------------------------\n");

        printf("9- Add the courses for the students.\n");
        printf("10- Delete the courses for the students.\n");
        printf("11- View the courses for the students.\n");

        printf("--------------------------------------------------\n");

        printf("12- View the department.\n");

        printf("--------------------------------------------------\n");

        printf("13- Exit.\n");


        scanf("%d", &getFromTheUser);
        switch(getFromTheUser){
            case 1:
                addStudent();
            break;

            case 2:
                do{
                    printf("Please enter the type of deletion: \n");
                    printf("1- Delete student by ID. \n");
                    printf("2- Delete student by name. \n");
                    printf("3- Exit. \n");
                    scanf("%d", &deletee);
                    switch(deletee){
                        case 1:
                            printf("Please enter the student ID: ");
                            scanf("%d", &studentID);
                            deleteStudentByID(studentID);
                            printf("\n\n");
                        break;

                        case 2:
                            printf("Please enter the student name: ");
                            scanf("%s", studentName);
                            deleteStudentByName(studentName);
                            printf("\n\n");
                        break;
                    }
                } while (deletee != 3);
            break;

            case 3:
                printf("Enter student ID to update: ");
                int idToUpdate;
                scanf("%d", &idToUpdate);
                updateStudent(idToUpdate);
            break;

            case 4:
                do{
                    printf("What type do you want to view.\n");
                    printf("1- Do you want to view by ID.\n");
                    printf("2- Do you want to view by name.\n");
                    printf("3- Do you want to view by phone number.\n");
                    printf("4- Exit.\n");

                    scanf("%d", &view);
                    switch(view){
                        case 1:
                            printf("Please enter the student ID: ");
                            scanf("%d", &studentID);
                            displayStudentByID(studentID);

                        break;

                        case 2:
                            printf("Please enter the student name: ");
                            scanf("%s", studentName);
                            displayStudentByName(studentName);

                        break;

                        case 3:
                            printf("Please enter the student phone number: ");
                            scanf("%d", &studentPhoneNumber);
                            displayStudentByPhoneNumber(studentPhoneNumber);
                        break;
                    }
                } while(view != 4);
            break;

            case 5:
                addCourse();
            break;

            case 6:
                do{
                    printf("Please enter the type of deletion: \n");
                    printf("1- Delete course by ID. \n");
                    printf("2- Delete course by title. \n");
                    printf("3- Exit. \n");
                    scanf("%d", &deletee);
                    switch(deletee){
                        case 1:
                            printf("Please enter the course ID: ");
                            scanf("%d", &courseID);
                            deleteCourseByID(courseID);
                        break;
                        case 2:
                            printf("Please enter the course title: ");
                            scanf("%s", courseTitle);
                            deleteCourseByTitle(courseTitle);
                        break;
                    }
                } while (deletee != 3);
            break;

            case 7:
                printf("Enter course ID to update: ");
                scanf("%d", &courseID);
                updateCourse(courseID);
            break;

            case 8:
                do{
                    printf("Please enter the type to view the courses: \n");
                    printf("1- View course by ID. \n");
                    printf("2- View course by title. \n");
                    printf("3- Exit. \n");
                    scanf("%d", &deletee);
                    switch(deletee){
                        case 1:
                            printf("Please enter the course ID: ");
                            scanf("%d", &courseID);
                            displayCourseByID(courseID);
                        break;
                        case 2:
                            printf("Please enter the course title: ");
                            scanf("%s", courseTitle);
                            displayCourseByTitle(courseTitle);
                        break;
                    }
                } while (deletee != 3);
            break;

            case 9:
                printf("Please enter the student ID: ");
                scanf("%d", &studentID);

                printf("Please enter the course ID: ");
                scanf("%d", &courseID);
                insert_enrollment(studentID, courseID);

                printf("\n\n");
            break;

            case 10:
                do{
                    printf("Please enter what do you want: \n");
                    printf("1- Delete enrollment by student ID. \n");
                    printf("2- Exit. \n");
                    scanf("%d", &deletee);
                    switch(deletee){
                        case 1:
                            printf("Please enter the student ID: ");
                            scanf("%d", &studentID);
                            printf("Please enter the course ID: ");
                            scanf("%d", &courseID);
                            delete_enrollment(studentID, courseID);
                        break;
                    }
                }while(deletee != 2);
            break;

            case 11:
                do{
                    printf("Please choose the one: \n");
                    printf("1- To view the courses the student chooses (By the student ID). \n");
                    printf("2- Exit. \n");
                    scanf("%d", &deletee);
                    switch(deletee){
                        case 1:
                            printf("Please enter the student ID: ");
                            scanf("%d", &studentID);
                            display_student_courses(studentID);
                        break;
                    }
                } while (deletee != 2);
            break;

            case 12:
                do{
                    printf("Please enter the type to view the Departments: \n");
                    printf("1- View department by ID. \n");
                    printf("2- View department by name. \n");
                    printf("3- Exit. \n");
                    scanf("%d", &view);
                    switch(view){
                        case 1:
                            printf("Please enter the department ID: ");
                            scanf("%d", &departmentID);
                            viewDepartmentByID(departmentID);
                        break;

                        case 2:
                            printf("Enter a department name: ");
                            scanf("%d", studentPhoneNumber);
                            viewDepartmentByName(studentPhoneNumber);
                        break;
                    }
                } while (view != 3);
            break;
        }
    } while(getFromTheUser != 13);
    saveStudentsToFile();
    freeMemory();
    return 0;
}
