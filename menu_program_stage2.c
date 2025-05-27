#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TRAINEES 8
#define MAX_MENTORS 8
#define MAX_NAME_LEN 50

// --- Global Data ---
char traineeNames[MAX_TRAINEES][MAX_NAME_LEN] = {
    "Tae", "Minji", "Haru", "Luna", "Kai", "Yuna", "Jisoo", "Hobin"
};

struct Mentor {
    int id;
    char name[MAX_NAME_LEN];
    int matchedTrainee; // Index of matched trainee
};

int traineeIDs[MAX_TRAINEES];
int traineeAbilities[MAX_TRAINEES];
struct Mentor mentors[MAX_MENTORS];

// --- Helper Functions ---

// ASCII sum of a string (used as unique ID)
int parseIntMember(const char* name) {
    int sum = 0;
    for (int i = 0; name[i] != '\0'; i++) {
        sum += (int)name[i];
    }
    return sum;
}

// Random number between 100 and 1000
int getRandomAbility() {
    return rand() % 901 + 100;
}

// --- Initialization ---

void initializeTrainees() {
    for (int i = 0; i < MAX_TRAINEES; i++) {
        traineeIDs[i] = parseIntMember(traineeNames[i]);
        traineeAbilities[i] = getRandomAbility();
    }
}

// --- Mentor Input ---

void inputMentors() {
    printf("Enter mentor names (max 8):\n");
    for (int i = 0; i < MAX_MENTORS; i++) {
        while (1) {
            printf("Mentor %d name: ", i + 1);
            fgets(mentors[i].name, MAX_NAME_LEN, stdin);

            // Remove newline
            size_t len = strlen(mentors[i].name);
            if (len > 0 && mentors[i].name[len - 1] == '\n') {
                mentors[i].name[len - 1] = '\0';
            }

            if (strlen(mentors[i].name) == 0) {
                printf("Invalid name. Please enter again.\n");
                continue;
            }

            mentors[i].id = i + 1;
            mentors[i].matchedTrainee = -1;
            break;
        }
    }
    printf("All 8 mentors have been successfully registered.\n");
}

// --- Matching Logic ---

void matchMentoring() {
    int usedMentors[MAX_MENTORS] = {0};

    for (int i = 0; i < MAX_TRAINEES; i++) {
        int preferredIndex = traineeIDs[i] % MAX_MENTORS;

        if (!usedMentors[preferredIndex]) {
            mentors[preferredIndex].matchedTrainee = i;
            usedMentors[preferredIndex] = 1;
        } else {
            // Assign to next available mentor
            for (int j = 0; j < MAX_MENTORS; j++) {
                if (!usedMentors[j]) {
                    mentors[j].matchedTrainee = i;
                    usedMentors[j] = 1;
                    break;
                }
            }
        }
    }
}

// --- Display Output ---

void printMentorMatches() {
    printf("\n--- Mentoring Matches ---\n");
    for (int i = 0; i < MAX_MENTORS; i++) {
        if (mentors[i].matchedTrainee >= 0) {
            int tIndex = mentors[i].matchedTrainee;
            printf("Trainee #%d (%s) [ID: %d, Ability: %d] => Mentor #%d (%s)\n",
                   tIndex + 1, traineeNames[tIndex], traineeIDs[tIndex], traineeAbilities[tIndex],
                   mentors[i].id, mentors[i].name);
        }
    }
}

// --- Main ---

int main() {
    srand(time(NULL)); // Seed for random ability scores

    initializeTrainees();
    inputMentors();
    matchMentoring();
    printMentorMatches();

    return 0;
}
