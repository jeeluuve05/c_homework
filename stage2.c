#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TRAINEES 8
#define MAX_MENTORS 8

typedef struct {
    int id;
    char name[50];
    int menteeNumber;
} Mentor;

typedef struct {
    char nickname[50];
    int ability;
    int mentorId;
} Trainee;

int parseMemberID(char *name) {
    int sum = 0;
    for (int i = 0; name[i] != '\0'; i++) {
        sum += name[i];
    }
    return sum;
}

int getRandomAbility() {
    return (rand() % 901) + 100; // random number between 100 and 1000
}

void initTrainees(Trainee trainees[], int count) {
    for (int i = 0; i < count; i++) {
        printf("Enter nickname for trainee %d: ", i + 1);
        scanf("%s", trainees[i].nickname);
        int parsed = parseMemberID(trainees[i].nickname);
        trainees[i].ability = getRandomAbility();
        trainees[i].mentorId = parsed % MAX_MENTORS + 1;
    }
}

void initMentors(Mentor mentors[], int count) {
    for (int i = 0; i < count; i++) {
        mentors[i].id = i + 1;
        printf("Enter name for mentor %d: ", mentors[i].id);
        scanf("%s", mentors[i].name);
        mentors[i].menteeNumber = -1; // initially no mentee
    }
}

void matchTraineesWithMentors(Trainee trainees[], int tCount, Mentor mentors[], int mCount) {
    for (int i = 0; i < tCount; i++) {
        int mId = trainees[i].mentorId;
        for (int j = 0; j < mCount; j++) {
            if (mentors[j].id == mId && mentors[j].menteeNumber == -1) {
                mentors[j].menteeNumber = i;
                break;
            }
        }
    }
}

void printResult(Trainee trainees[], int tCount, Mentor mentors[], int mCount) {
    printf("\nTrainee Matching Result:\n");
    for (int i = 0; i < tCount; i++) {
        int mentorIndex = -1;
        for (int j = 0; j < mCount; j++) {
            if (mentors[j].id == trainees[i].mentorId) {
                mentorIndex = j;
                break;
            }
        }
        if (mentorIndex != -1) {
            printf("Trainee: %s | Ability: %d | Mentor ID: %d | Mentor Name: %s\n",
                   trainees[i].nickname, trainees[i].ability,
                   mentors[mentorIndex].id, mentors[mentorIndex].name);
        } else {
            printf("Trainee: %s | No matching mentor found.\n", trainees[i].nickname);
        }
    }
}

int main() {
    srand(time(NULL));

    Trainee trainees[MAX_TRAINEES];
    Mentor mentors[MAX_MENTORS];

    initMentors(mentors, MAX_MENTORS);
    initTrainees(trainees, MAX_TRAINEES);
    matchTraineesWithMentors(trainees, MAX_TRAINEES, mentors, MAX_MENTORS);
    printResult(trainees, MAX_TRAINEES, mentors, MAX_MENTORS);

    return 0;
}
