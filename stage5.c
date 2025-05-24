#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#define CLEAR_SCREEN "cls"
#define SLEEP(seconds) Sleep((seconds) * 1000)
#else
#include <unistd.h>
#define CLEAR_SCREEN "clear"
#define SLEEP(seconds) sleep(seconds)
#endif

#define MAX_DANCE_STEPS 10
#define MAX_DANCES 6
#define MAX_MEMBERS 5
#define MAX_NAME_LENGTH 100
#define MAX_NICKNAME_LENGTH 50

typedef struct {
    char korean_name[MAX_NAME_LENGTH];
    char english_name[MAX_NAME_LENGTH];
    char steps[MAX_DANCE_STEPS][MAX_NAME_LENGTH];
    int num_steps;
} DanceRoutine;

typedef struct {
    char full_name[MAX_NAME_LENGTH];
    char alias[MAX_NICKNAME_LENGTH];
    int dance_score;
} CrewMember;

DanceRoutine routines[MAX_DANCES];
CrewMember crew[MAX_MEMBERS];

// Read dance steps from file
int loadDanceSteps() {
    FILE *file = fopen("dance_step.txt", "r");
    if (!file) {
        printf("Error opening dance_step.txt file!\n");
        return -1;
    }

    for (int i = 0; i < MAX_DANCES; i++) {
        if (fscanf(file, "%s %s", routines[i].korean_name, routines[i].english_name) != 2) {
            printf("Error reading dance names.\n");
            fclose(file);
            return -1;
        }
        if (fscanf(file, "%d", &routines[i].num_steps) != 1) {
            printf("Error reading number of steps.\n");
            fclose(file);
            return -1;
        }
        for (int j = 0; j < routines[i].num_steps; j++) {
            if (fscanf(file, "%s", routines[i].steps[j]) != 1) {
                printf("Error reading dance step %d.\n", j + 1);
                fclose(file);
                return -1;
            }
        }
    }
    fclose(file);
    return 0;
}

// Shuffle dance steps
void shuffleSteps(char steps[MAX_DANCE_STEPS][MAX_NAME_LENGTH], int num_steps) {
    for (int i = 0; i < num_steps; i++) {
        int j = rand() % num_steps;
        char temp[MAX_NAME_LENGTH];
        strcpy(temp, steps[i]);
        strcpy(steps[i], steps[j]);
        strcpy(steps[j], temp);
    }
}

// Display steps for 10 seconds
void displayStepsWithTimer(char steps[MAX_DANCE_STEPS][MAX_NAME_LENGTH], int num_steps) {
    for (int i = 0; i < num_steps; i++) {
        printf("%d. %s\n", i + 1, steps[i]);
    }
    printf("\nDisplaying the steps for 10 seconds...\n");
    SLEEP(10);
    system(CLEAR_SCREEN);
}

// Evaluate learner's input and score
int evaluateDanceSteps(char correct_steps[MAX_DANCE_STEPS][MAX_NAME_LENGTH], int num_steps,
                       char learner_steps[MAX_DANCE_STEPS][MAX_NAME_LENGTH]) {
    int correct_count = 0;
    int order_match = 1;

    for (int i = 0; i < num_steps; i++) {
        if (strcmp(correct_steps[i], learner_steps[i]) == 0) {
            correct_count++;
        } else {
            order_match = 0;
        }
    }

    if (correct_count == num_steps && order_match == 1) {
        return 100;
    }
    if (correct_count == num_steps && order_match == 0) {
        return 50;
    }
    if (correct_count > 0) {
        return 20;
    }
    return 0;
}

int validateCrewAlias(char *alias) {
    for (int i = 0; i < MAX_MEMBERS; i++) {
        if (strcmp(crew[i].alias, alias) == 0) {
            return i;
        }
    }
    return -1;
}

void learnDanceRoutine() {
    char alias[MAX_NICKNAME_LENGTH];
    int member_index = -1;

    while (member_index == -1) {
        printf("Enter your alias: ");
        scanf("%s", alias);

        member_index = validateCrewAlias(alias);
        if (member_index == -1) {
            printf("Alias not found. Try again or enter 'quit' to exit.\n");
            if (strcmp(alias, "quit") == 0) {
                return;
            }
        }
    }

    int routine_index = rand() % MAX_DANCES;
    printf("\nYou have to learn the routine: %s (%s)\n", routines[routine_index].korean_name, routines[routine_index].english_name);

    displayStepsWithTimer(routines[routine_index].steps, routines[routine_index].num_steps);

    char shuffled_steps[MAX_DANCE_STEPS][MAX_NAME_LENGTH];
    for (int i = 0; i < routines[routine_index].num_steps; i++) {
        strcpy(shuffled_steps[i], routines[routine_index].steps[i]);
    }
    shuffleSteps(shuffled_steps, routines[routine_index].num_steps);

    printf("\nEnter the steps in the correct order:\n");
    char learner_steps[MAX_DANCE_STEPS][MAX_NAME_LENGTH];
    for (int i = 0; i < routines[routine_index].num_steps; i++) {
        printf("Step %d: ", i + 1);
        scanf("%s", learner_steps[i]);
    }

    int score = evaluateDanceSteps(routines[routine_index].steps, routines[routine_index].num_steps, learner_steps);
    crew[member_index].dance_score = score;

    printf("\nCorrect answer steps:\n");
    for (int i = 0; i < routines[routine_index].num_steps; i++) {
        printf("%d. %s\n", i + 1, routines[routine_index].steps[i]);
    }
    printf("Your input steps:\n");
    for (int i = 0; i < routines[routine_index].num_steps; i++) {
        printf("%d. %s\n", i + 1, learner_steps[i]);
    }
    printf("Your score: %d\n", score);
}

int main() {
    srand((unsigned int)time(NULL));

    // New crew members
    strcpy(crew[0].full_name, "Alice Johnson");
    strcpy(crew[0].alias, "AJ");
    strcpy(crew[1].full_name, "Bob Smith");
    strcpy(crew[1].alias, "Bobby");
    strcpy(crew[2].full_name, "Carol Lee");
    strcpy(crew[2].alias, "Caz");
    strcpy(crew[3].full_name, "David Kim");
    strcpy(crew[3].alias, "DK");
    strcpy(crew[4].full_name, "Eva Martinez");
    strcpy(crew[4].alias, "Evie");

    if (loadDanceSteps() != 0) {
        return -1;
    }

    learnDanceRoutine();

    return 0;
}
