#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TRAINEES 8
#define MAX_MENTORS 8
#define MAX_NAME_LEN 50

// Trainee data
char traineeNames[MAX_TRAINEES][MAX_NAME_LEN] = {
    "Tae", "Minji", "Haru", "Luna", "Kai", "Yuna", "Jisoo", "Hobin"
};

// Struct for mentors
struct Mentor {
    int id; // Unique ID between 1 and 8
    char name[MAX_NAME_LEN];
    int matchedTrainee; // Trainee index
};

// Global arrays
int traineeIDs[MAX_TRAINEES];
int traineeAbilities[MAX_TRAINEES];
struct Mentor mentors[MAX_MENTORS];

// Converts a name to an integer using ASCII sum
int parseIntMember(const char* name) {
    int sum = 0;
    for (int i = 0; name[i] != '\0'; i++) {
        sum += (int)name[i];
    }
    return sum;
}

// Returns a random ability score between 100 and 1000
int getRandomAbility() {
    return rand() % 901 + 100; // 100–1000
}

// Initializes trainees with IDs and random abilities
void initializeTrainees() {
    for (int i = 0; i < MAX_TRAINEES; i++) {
        traineeIDs[i] = parseIntMember(traineeNames[i]);
        traineeAbilities[i] = getRandomAbility();
    }
}

// Adds up to 8 mentors
void inputMentors() {
    printf("Enter mentor names (max 8):\n");
    for (int i = 0; i < MAX_MENTORS; i++) {
        printf("Mentor %d name: ", i + 1);
        scanf(" %[^"]%*c", mentors[i].name); // Read line with spaces
        mentors[i].id = i + 1;
        mentors[i].matchedTrainee = -1;
    }
}

// Matches each trainee to a mentor (1:1 guaranteed)
void matchMentoring() {
    int availableMentors[MAX_MENTORS] = {0};

    for (int i = 0; i < MAX_TRAINEES; i++) {
        int mentorIndex = i % MAX_MENTORS;
        if (!availableMentors[mentorIndex]) {
            mentors[mentorIndex].matchedTrainee = i;
            availableMentors[mentorIndex] = 1;
        } else {
            // Already matched — find next available
            for (int j = 0; j < MAX_MENTORS; j++) {
                if (!availableMentors[j]) {
                    mentors[j].matchedTrainee = i;
                    availableMentors[j] = 1;
                    break;
                }
            }
        }
    }
}

// Displays matched pairs
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

int main() {
    srand(time(NULL)); // Seed randomness

    initializeTrainees();
    inputMentors();
    matchMentoring();
    printMentorMatches();

    return 0;
}
