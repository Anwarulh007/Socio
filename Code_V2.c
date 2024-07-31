#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

#define MIN_LENGTH 8
#define MAX_LENGTH 30
#define MIN_PASSWORD_LENGTH 8
#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define NUM_ROWS 100
#define NUM_COLUMNS 6

typedef struct
{
    char username[100];
    int likes;
    int comments;
    int chats;
    int tagged_photos;
    int comments_mentioned;
    int points;
} Data;

void create_account();
int isValidUsername(char *user);
int isUsernameTaken(char *user);
int isValidPassword(char *password);

void login();

int operations(char *user);
void printMenu();
void generate_data(char *user);
int readFile(Data data[], int maxRecords, char *user);
void printData(Data data[], int records);
void calculatePoints(Data data[], int records);
void sortData(Data data[], int records);
void printTop5(Data data[], int records);

int main()
{
    int option;
    while (1)
    {
        printf("--------------------------------------------------------------------------------------\n");
        printf("MENU\n");
        printf("1. Create account\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("--------------------------------------------------------------------------------------\n");
        printf("Enter an option: ");
        fflush(stdin);
        scanf("%d", &option);
        printf("--------------------------------------------------------------------------------------\n");
        switch (option)
        {
        case 1:
            create_account();
            break;
        case 2:
            login();
            break;
        case 3:
            printf("Exiting...\n");
            exit(0);
            break;
        default:
            printf("Invalid option, try again.\n");
            break;
        }
    }

    return 0;
}

void create_account()
{
    char user[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    printf("Enter a username: ");
    fflush(stdin);
    scanf("%[^\n]", user);
    while (!isValidUsername(user))
    {
        printf("Invalid username\n");
        printf("--------------------------------------------------------------------------------------\n");
        printf("re-enter a username: ");

        fflush(stdin);
        scanf("%[^\n]", user);
    }

    while (isUsernameTaken(user))
    {
        printf("Sorry, '%s' is already taken.\n", user);
        printf("--------------------------------------------------------------------------------------\n");
        printf("Please try a new one: ");
        fflush(stdin);
        scanf("%[^\n]", user);
    }

    printf("--------------------------------------------------------------------------------------\n");
    printf("Enter a password: ");
    fflush(stdin);
    scanf("%[^\n]", password);
    while (!isValidPassword(password))
    {
        printf("Invalid password\n");
        printf("--------------------------------------------------------------------------------------\n");
        printf("re-enter a password: ");
        fflush(stdin);
        scanf("%[^\n]", password);
    }
    FILE *fp;
    fp = fopen("accounts.csv", "a");
    fprintf(fp, "%s,%s\n", user, password);
    printf("\nAccount created successfully\n");
    fclose(fp);
}

int isValidUsername(char *user)
{
    int len = strlen(user);
    int a, b, c = 1;
    if (len < MIN_LENGTH || len > MAX_LENGTH)
    {
        if (len < MIN_LENGTH)
        {
            printf("Error: Username is too short. MIN_LENGTH = %d and MAX_LENGTH = %d\n", MIN_LENGTH, MAX_LENGTH);
            a = 0;
        }
        else
        {
            printf("Error: Username is too long. MIN_LENGTH = %d and MAX_LENGTH = %d\n", MIN_LENGTH, MAX_LENGTH);
        }
    }
    if (!isalpha(user[0]))
    {
        printf("Error: First character must be a letter.\n");
        b = 0;
    }
    for (int i = 0; i < len; i++)
    {
        if (!isalnum(user[i]) && user[i] != '_')
        {
            if (isspace(user[i]))
            {
                printf("Error: Space not allowed in the username.\n");
                c = 0;
            }
            else
            {
                printf("Error: Invalid character '%c' in username. Only letters, numbers and _ are allowed.\n", user[i]);
                c = 0;
            }
        }
    }
    if (!(a && b && c))
    {
        return 0;
    }

    return 1;
}

int isUsernameTaken(char *user)
{
    if (access("accounts.csv", F_OK) != -1)
    {
        FILE *file = fopen("accounts.csv", "r");
        if (file == NULL)
        {
            printf("Error opening file.\n");
            return -1;
        }
        char currentUsername[100], currentPassword[100];
        while (fscanf(file, "%30[^,],%s\n", currentUsername, currentPassword) == 2)
        {
            if (strcmp(currentUsername, user) == 0)
            {
                fclose(file);
                return 1;
            }
        }
        fclose(file);
        return 0;
    }
    else
    {
        return 0;
    }
}

const char special_characters[] = "!@#$%^&*()-+";

int isValidPassword(char *password)
{
    int len = strlen(password);
    if (len < MIN_PASSWORD_LENGTH)
    {
        printf("Error: Password is too short. It must be at least %d characters long.\n", MIN_PASSWORD_LENGTH);
    }

    int hasLower = 0, hasUpper = 0, hasDigit = 0, hasSpecial = 0;

    for (int i = 0; i < len; i++)
    {
        if (islower(password[i]))
            hasLower = 1;
        else if (isupper(password[i]))
            hasUpper = 1;
        else if (isdigit(password[i]))
            hasDigit = 1;
        else if (strchr(special_characters, password[i]) != NULL)
            hasSpecial = 1;
    }
    if (!hasLower)
        printf("Error: Password must contain at least one lowercase letter.\n");
    if (!hasUpper)
        printf("Error: Password must contain at least one uppercase letter.\n");
    if (!hasDigit)
        printf("Error: Password must contain at least one digit.\n");
    if (!hasSpecial)
        printf("Error: Password must contain at least one special character.\n");

    if (hasLower && hasUpper && hasDigit && hasSpecial)
    {
        return 1;
    }
    return 0;
}

void login()
{
    char user[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char file_user[MAX_USERNAME_LENGTH];
    char file_password[MAX_PASSWORD_LENGTH];
    int valid = 0;
    printf("Enter your username: ");
    fflush(stdin);
    scanf("%[^\n]", user);
    printf("Enter your password: ");
    fflush(stdin);
    scanf("%[^\n]", password);

    FILE *fp;
    fp = fopen("accounts.csv", "r");
    if (fp == NULL)
    {
        printf("\nError opening file.\n");
        return;
    }
    while (fscanf(fp, "%30[^,],%s\n", file_user, file_password) != EOF)
    {
        if (strcmp(file_user, user) == 0)
        {
            if (strcmp(file_password, password) == 0)
            {
                valid = 1;
                break;
            }
        }
    }
    fclose(fp);

    if (valid)
    {
        printf("--------------------------------------------------------------------------------------\n");
        printf("Welcome, %s!\n", user);
        operations(user);
    }
    else
    {
        printf("Invalid username or password.\n");
    }
}

int operations(char *user)
{
    Data data[100];
    int records = 0;
    int choice;
    while (1)
    {
        printMenu();
        fflush(stdin);
        scanf("%d", &choice);
        printf("--------------------------------------------------------------------------------------\n");
        switch (choice)
        {
        case 1:
            generate_data(user);
            break;
        case 2:
            records = readFile(data, 100, user);
            if (records > 0)
            {
                printf("%d line of data has been read.\n", records);
            }
            break;
        case 3:
            if (records > 0)
            {
                printData(data, records);
            }
            else
            {
                printf("1st Perform these 2 steps:\n\t\tGenerate Data\n\t\tRead Data\n");
            }
            break;
        case 4:
            printf("Point Calculation Matrix:\n");
            printf("1 Like = 1 point\n");
            printf("1 Comment = 1.5 points\n");
            printf("1 Chat = 0.2 points\n");
            printf("1 Tagged photo = 2 points\n");
            printf("1 Comment mentioned = 3 points\n");
            break;
        case 5:
            if (records > 0)
            {
                calculatePoints(data, records);
                sortData(data, records);
                printf("\nCalculated Points:\n");
                printf("-------------------------------\n");
                printf("|      %-14s| %-7s|\n", "Username", "Points");
                printf("-------------------------------\n");
                for (int i = 0; i < records; i++)
                    printf("|  %-18s|   %-5d|\n", data[i].username, data[i].points);
                printf("-------------------------------\n");
            }
            else
            {
                printf("1st Perform these 2 steps:\n\t\tGenerate Data\n\t\tRead Data\n");
            }
            break;
        case 6:
            if (records > 0)
            {
                calculatePoints(data, records);
                sortData(data, records);
                printTop5(data, records);
            }
            else
            {
                printf("1st Perform these 2 steps:\n\t\tGenerate Data\n\t\tRead Data\n");
            }
            break;
        case 7:
            return 0;
            break;
        default:
            printf("\nInvalid option selected. Please try again.\n");
            break;
        }
    }
    return 0;
}

void printMenu()
{
    printf("--------------------------------------------------------------------------------------\n");
    printf("MENU\n");
    printf("1. Generate Data\n");
    printf("2. Read Data\n");
    printf("3. View Data\n");
    printf("4. View Point Calculation Criteria\n");
    printf("5. View Calculated Points\n");
    printf("6. View Result\n");
    printf("7. Logout\n");
    printf("--------------------------------------------------------------------------------------\n");
    printf("Select an option:");
}

const char *first_names[] = {
    "Amit", "Anil", "Anjali", "Ankita", "Asha", "Aarti",
    "Dinesh", "Jasmine", "Jyoti", "Kamal", "Manish", "Megha",
    "Monika", "Mukesh", "Nandini", "Neha", "Nidhi", "Praveen",
    "Preeti", "Priyanka", "Prashant", "Rajat", "Rajesh", "Rakesh",
    "Rashmi", "Reena", "Ravi", "Sanjay", "Sanjeev", "Shilpa",
    "Shobha", "Shruti", "Sneha", "Suresh", "Vikas", "Vinod"};

const char *surnames[] = {
    "Sharma", "Devi", "Singh", "Kumar", "Yadav", "Gupta",
    "Verma", "Chaudhary", "Jain", "Aggarwal", "Rao", "Patel",
    "Bhatia", "Choudhary", "Bhardwaj", "Tripathi", "Kapoor", "Mehta",
    "Dixit", "Jha", "Prasad", "Anand", "Malhotra", "Arora",
    "Chauhan", "Rajput", "Sinha", "Mathur", "Varma", "Chawla",
    "Khanna", "Meena", "Khatri", "Nath", "Soni", "Bajaj",
    "Pandey", "Rastogi", "Goel", "Desai", "Chopra", "Dutta",
    "Sethi", "Bose", "Jadhav", "Shah", "Shukla", "Thakur", "Bedi"};

void generate_data(char *user)
{
    srand(time(NULL));

    if (access(user, F_OK) != -1)
    {
        printf("Data already exists.\n");
        return;
    }
    else
    {

        char command[200];
        sprintf(command, "mkdir %s", user);
        if (system(command) != 0)
        {
            printf("Error: Failed to create folder.\n");
            return;
        }

        char fileName[200];
        sprintf(fileName, "%s/recent-activities.csv", user);

        FILE *file = fopen(fileName, "w");
        if (file == NULL)
        {
            perror("Error opening file");
            return;
        }

        for (int i = 0; i < NUM_ROWS; i++)
        {
            char name[32];
            sprintf(name, "%s %s", first_names[rand() % (sizeof(first_names) / sizeof(char *))],
                    surnames[rand() % (sizeof(surnames) / sizeof(char *))]);
            int likes = rand() % 50;
            int comments = rand() % 30;
            int chats = (i < 20) ? (70 + rand() % 29) : (rand() % 50);
            int photos = rand() % 8;
            int comments_mentioned = rand() % 8;
            fprintf(file, "%s,%d,%d,%d,%d,%d\n", name, likes, comments, chats, photos, comments_mentioned);
        }

        fclose(file);

        printf("Data has been generated successfully!\n");
    }
}

int readFile(Data data[], int maxRecords, char *user)
{
    char fileName[200];
    sprintf(fileName, "%s/recent-activities.csv", user);
    FILE *file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("\n1st Generate data then read it.\n");
        return -1;
    }

    int read = 0;
    int records = 0;

    do
    {
        read = fscanf(file, "%30[^,],%d,%d,%d,%d,%d\n", data[records].username,
                                                        &data[records].likes,
                                                        &data[records].comments,
                                                        &data[records].chats,
                                                        &data[records].tagged_photos,
                                                        &data[records].comments_mentioned);
        if (read == 6)
            records++;
        if (read != 6 && !feof(file))
        {
            printf("\nFile format incorrect.\n");
            return -1;
        }
        if (ferror(file))
        {
            printf("\nError reading file.\n");
            return -1;
        }
    } while (!feof(file));

    fclose(file);

    return records;
}

void printData(Data data[], int records)
{
    printf("\nData : \n");
    printf("--------------------------------------------------------------------------------------\n");
    printf("|      %-14s| %-6s| %-9s| %-6s| %-14s| %-19s|\n", "Username",
                                                              "Likes",
                                                              "Comments",
                                                              "Chats",
                                                              "Tagged photos",
                                                              "Comments mentioned");
    printf("--------------------------------------------------------------------------------------\n");
    for (int i = 0; i < records; i++)
        printf("|  %-18s|  %-5d|    %-6d|  %-5d|       %-8d|         %-11d|\n", data[i].username,
                                                                                data[i].likes,
                                                                                data[i].comments,
                                                                                data[i].chats,
                                                                                data[i].tagged_photos,
                                                                                data[i].comments_mentioned);
    printf("--------------------------------------------------------------------------------------\n");
}

void calculatePoints(Data data[], int records)
{
    for (int i = 0; i < records; i++)
    {
        data[i].points = data[i].likes +
                         data[i].comments * 1.5 +
                         data[i].chats / 5 +
                         data[i].tagged_photos * 2 +
                         data[i].comments_mentioned * 3;
    }
}

void sortData(Data data[], int records)
{
    for (int i = 0; i < records - 1; i++)
    {
        for (int j = i + 1; j < records; j++)
        {
            if (data[i].points < data[j].points)
            {
                Data temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }
    }
}

void printTop5(Data data[], int records)
{
    int n = 5;
    if (records < 5)
    {
        n = records;
    }
    printf("\nThese are your %d closest friends:\n", n);
    for (int i = 0; i < n; i++)
    {
        printf("%d. %s - %d points\n", i + 1, data[i].username, data[i].points);
    }
}