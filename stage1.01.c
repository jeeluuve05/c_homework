#include <stdio.h>
#include <string.h>

#define MAX_MEMBERS 10
#define MAX_NAME_LEN 50
#define DAYS 6
#define MAX_EXERCISE_LEN 50

const char *days[DAYS] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

const char *exercise_types[5][3] = {
    { "Running", "Cycling", "Fast Walking" },           // Cardio
    { "Push-ups", "Squats", NULL },                     // Full-body Strength
    { "Leg Press", "Leg Curl", NULL },                  // Lower-body Strength
    { "Pull-ups", "Chin-ups", NULL },                   // Upper-body Strength
    { "Plank", "Crunches", NULL }                       // Core
};

typedef struct {
    char name[MAX_NAME_LEN];
    char nickname[MAX_NAME_LEN];
    char routine[DAYS][2][MAX_EXERCISE_LEN]; // [day][0] = cardio, [1] = strength/core
} Member;

Member milliways_members[MAX_MEMBERS];
int member_count = 0;

void listMembers() {
    printf("\n--- Member List ---\n");
    for (int i = 0; i < member_count; i++) {
        printf("%d. %s (%s)\n", i + 1, milliways_members[i].name, milliways_members[i].nickname);
    }
    printf("-------------------\n");
}

void setExerciseRoutine() {
    listMembers();
    int idx;
    printf("Select member index to set routine: ");
    scanf("%d", &idx);
    idx--;

    if (idx < 0 || idx >= member_count) {
        printf("Invalid member index.\n");
        return;
    }

    Member *m = &milliways_members[idx];
    int core_used = 0;

    printf("\n--- Setting Routine for %s ---\n", m->name);
    for (int i = 0; i < DAYS; i++) {
        printf("\n%s:\n", days[i]);

        printf("Choose a cardio exercise (0: Running, 1: Cycling, 2: Fast Walking): ");
        int cardio;
        scanf("%d", &cardio);
        strcpy(m->routine[i][0], exercise_types[0][cardio]);

        printf("Choose a type of strength/core exercise:\n");
        printf("1: Full-body Strength (0: Push-ups, 1: Squats)\n");
        printf("2: Lower-body Strength (0: Leg Press, 1: Leg Curl)\n");
        printf("3: Upper-body Strength (0: Pull-ups, 1: Chin-ups)\n");
        printf("4: Core (0: Plank, 1: Crunches) — allowed once/week\n");

        int type, ex;
        do {
            printf("Enter type (1-4): ");
            scanf("%d", &type);
        } while (type < 1 || type > 4);

        if (type == 4) {
            if (core_used) {
                printf("Core already used this week! Choose another type.\n");
                i--;
                continue;
            } else {
                core_used = 1;
            }
        }

        printf("Enter exercise index (0 or 1): ");
        scanf("%d", &ex);
        strcpy(m->routine[i][1], exercise_types[type][ex]);
    }

    printf("\nWorkout routine set for %s.\n", m->name);
}

void getExerciseRoutine() {
    char search_name[MAX_NAME_LEN];
    printf("Enter member name: ");
    scanf("%s", search_name);

    for (int i = 0; i < member_count; i++) {
        if (strcmp(search_name, milliways_members[i].name) == 0) {
            Member *m = &milliways_members[i];
            printf("\n--- Workout Routine for %s (%s) ---\n", m->name, m->nickname);
            for (int d = 0; d < DAYS; d++) {
                printf("%s: Cardio: %s | Strength/Core: %s\n",
                       days[d],
                       m->routine[d][0],
                       m->routine[d][1]);
            }
            return;
        }
    }

    printf("Member not found.\n");
}

// Добавь пример инициализации участников
void seedMembers() {
    strcpy(milliways_members[0].name, "Zaphod");
    strcpy(milliways_members[0].nickname, "Zaph");
    strcpy(milliways_members[1].name, "Trillian");
    strcpy(milliways_members[1].nickname, "Trill");
    member_count = 2;
}

// Пример главного меню
int main() {
    seedMembers();
    char choice;

    while (1) {
        printf("\n[1. Physical Strength & Knowledge]\n");
        printf("C. Set Basic Workout Routine\n");
        printf("D. View Basic Workout Routine\n");
        printf("Q. Quit\n");
        printf("Select: ");
        scanf(" %c", &choice);

        switch (choice) {
            case 'C': setExerciseRoutine(); break;
            case 'D': getExerciseRoutine(); break;
            case 'Q': return 0;
            default: printf("Invalid option.\n");
        }
    }
}
