#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define MAX_MEMBERS 10
#define MAX_TRAUMA_LEN 200
#define MAX_RESPONSE_LEN 101
#define MAX_QUESTIONS 5
#define MAX_SESSIONS 50

typedef struct {
    char nickname[30];
    int age;
    char trauma[MAX_TRAUMA_LEN];
} Trauma;

typedef struct {
    int id;
    char content[150];
} Question;

typedef struct {
    char nickname[30];
    char question[150];
    char response[MAX_RESPONSE_LEN];
} Counseling;

// Example member data
char memberNicknames[MAX_MEMBERS][30] = {
    "Luna", "Kai", "Mira", "Jin", "Yuki"
};
int memberAges[MAX_MEMBERS] = {20, 22, 19, 21, 23};

Trauma traumas[MAX_MEMBERS];
int traumaCount = 0;

const Question questions[MAX_QUESTIONS] = {
    {1, "In what situations have you experienced this trauma?"},
    {2, "How has this situation affected your daily life and emotions?"},
    {3, "How have you tried to overcome this trauma?"},
    {4, "What emotions do you associate with this trauma?"},
    {5, "What kind of support do you think is necessary to overcome this trauma?"}
};

Counseling sessions[MAX_SESSIONS];
int sessionCount = 0;

void inputTrauma() {
    char nickname[30], trauma[MAX_TRAUMA_LEN];
    int index = -1;
    printf("Enter nickname: ");
    scanf("%s", nickname);

    for (int i = 0; i < MAX_MEMBERS; i++) {
        index = strcmp(nickname, memberNicknames[i]) == 0 ? i : index;
    }

    index != -1 ?
        (printf("Enter trauma description: "), getchar(), fgets(trauma, MAX_TRAUMA_LEN, stdin),
        trauma[strcspn(trauma, "\n")] = 0,
        strcpy(traumas[index].nickname, nickname),
        traumas[index].age = memberAges[index],
        strcpy(traumas[index].trauma, trauma),
        printf("Trauma saved for %s.\n", nickname)) :
        printf("Nickname not found. Please try again.\n");
}

void startCounseling() {
    printf("Members with trauma entries:\n");
    for (int i = 0; i < MAX_MEMBERS; i++) {
        if (strlen(traumas[i].trauma) > 0) {
            printf("- %s\n", traumas[i].nickname);
        }
    }

    char nickname[30];
    printf("Enter nickname for counseling: ");
    scanf("%s", nickname);

    int found = 0;
    for (int i = 0; i < MAX_MEMBERS; i++) {
        if (strcmp(nickname, traumas[i].nickname) == 0 && strlen(traumas[i].trauma) > 0) {
            found = 1;
            srand((unsigned int)time(NULL));
            int selected[3] = {-1, -1, -1};
            int count = 0;

            while (count < 3) {
                int randIdx = rand() % MAX_QUESTIONS;
                int duplicate = 0;
                for (int j = 0; j < count; j++) {
                    if (selected[j] == randIdx) {
                        duplicate = 1;
                        break;
                    }
                }
                if (!duplicate) {
                    selected[count++] = randIdx;
                }
            }

            for (int k = 0; k < 3; k++) {
                char response[MAX_RESPONSE_LEN];
                do {
                    printf("\nQ: %s\n", questions[selected[k]].content);
                    printf("Your response (max 100 characters): ");
                    getchar(); // clear input buffer
                    fgets(response, MAX_RESPONSE_LEN, stdin);
                    response[strcspn(response, "\n")] = 0;
                    if (strlen(response) == 0 || strlen(response) >= MAX_RESPONSE_LEN) {
                        printf("Invalid response. Try again.\n");
                    }
                } while (strlen(response) == 0 || strlen(response) >= MAX_RESPONSE_LEN);

                strcpy(sessions[sessionCount].nickname, nickname);
                strcpy(sessions[sessionCount].question, questions[selected[k]].content);
                strcpy(sessions[sessionCount].response, response);
                sessionCount++;
            }

            printf("\nCounseling session completed for %s.\n", nickname);
            break;
        }
    }
    if (!found) {
        printf("No trauma entry found for this nickname.\n");
    }
}

void viewCounselingResults() {
    char nickname[30];
    printf("Enter nickname to view session results: ");
    scanf("%s", nickname);

    int found = 0;
    for (int i = 0; i < MAX_MEMBERS; i++) {
        if (strcmp(nickname, traumas[i].nickname) == 0) {
            found = 1;
            printf("\n--- Counseling Record for %s ---\n", nickname);
            printf("Age: %d\n", traumas[i].age);
            printf("Trauma: %s\n", traumas[i].trauma);
            for (int j = 0; j < sessionCount; j++) {
                if (strcmp(sessions[j].nickname, nickname) == 0) {
                    printf("Q: %s\n", sessions[j].question);
                    printf("A: %s\n", sessions[j].response);
                }
            }
            break;
        }
    }
    if (!found) {
        printf("No records found for that nickname.\n");
    }
}

void traumaMenu() {
    char choice;
    do {
        printf("\n[Trauma Management Menu]\n");
        printf("A. Input Trauma Information\n");
        printf("B. Start Counseling Session\n");
        printf("C. View Counseling Results\n");
        printf("Q. Return to Previous Menu\n");
        printf("Select an option: ");
        scanf(" %c", &choice);

        switch (tolower(choice)) {
            case 'a':
                inputTrauma();
                break;
            case 'b':
                startCounseling();
                break;
            case 'c':
                viewCounselingResults();
                break;
            case 'q':
                return;
            default:
                printf("Invalid selection. Try again.\n");
        }
    } while (1);
}
