#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_MEMBERS 10
#define MAX_NAME_LEN 50
#define MAX_NICKNAME_LEN 20
#define NUM_TESTS 7

// Fitness test names for display
const char* fitness_test_names[NUM_TESTS] = {
    "1-Mile Running Test (min for 1.6km)",
    "Speed Sprint (sec for 100m)",
    "Push-ups (min for 30 reps)",
    "Squats (min for 50 reps)",
    "Arm Strength (min for 50 push-ups)",
    "Swimming (min for 400m)",
    "Weightlifting (bodyweight multiplier)"
};

// Member struct
typedef struct {
    char name[MAX_NAME_LEN];
    char nickname[MAX_NICKNAME_LEN];
    char gender;  // 'M' or 'F' for weightlifting multiplier rule
} Member;

// Predefined Milliways members (example)
Member milliways_members[MAX_MEMBERS] = {
    {"Alice Kim", "alice", 'F'},
    {"Bob Lee", "bob", 'M'},
    {"Cindy Park", "cindy", 'F'},
    {"David Choi", "david", 'M'}
};

int total_members = 4;

// Fitness scores: rows = members, cols = tests
float health_scores[MAX_MEMBERS][NUM_TESTS] = {0};

// Helper: Find member index by nickname
int find_member_by_nickname(const char* nickname) {
    for (int i = 0; i < total_members; i++) {
        if (strcmp(milliways_members[i].nickname, nickname) == 0) {
            return i;
        }
    }
    return -1; // Not found
}

// Parsing function for comma separated input of 7 floats
int parse_fitness_data(const char* input, float* scores) {
    char temp[256];
    strncpy(temp, input, sizeof(temp));
    temp[sizeof(temp) - 1] = '\0';

    char *token = strtok(temp, ",");
    int count = 0;

    while (token != NULL && count < NUM_TESTS) {
        scores[count] = atof(token);
        count++;
        token = strtok(NULL, ",");
    }

    if (count != NUM_TESTS) {
        return -1; // error: wrong number of values
    }
    return 0; // success
}

// setHealth(): input fitness data for each member
void setHealth(void) {
    char input_line[256];

    printf("Enter fitness data for each member as 7 comma-separated values:\n");
    printf("Format: 1-Mile Running, Speed Sprint, Push-ups, Squats, Arm Strength, Swimming, Weightlifting\n");

    for (int i = 0; i < total_members; i++) {
        printf("Member %s (%s): ", milliways_members[i].name, milliways_members[i].nickname);
        if (!fgets(input_line, sizeof(input_line), stdin)) {
            printf("Input error.\n");
            return;
        }

        // Remove trailing newline
        input_line[strcspn(input_line, "\n")] = 0;

        float scores[NUM_TESTS];
        if (parse_fitness_data(input_line, scores) != 0) {
            printf("Error: Please enter exactly 7 comma-separated numeric values.\n");
            i--; // retry this member
            continue;
        }

        // Adjust weightlifting score based on gender rule
        // Men: 1.5x bodyweight (input as multiplier)
        // Women: 0.75x bodyweight (input as multiplier)
        // If input invalid, just store raw value.
        if (milliways_members[i].gender == 'M') {
            if (scores[6] < 1.5) {
                printf("Warning: Weightlifting multiplier for men usually >= 1.5\n");
            }
        } else if (milliways_members[i].gender == 'F') {
            if (scores[6] < 0.75) {
                printf("Warning: Weightlifting multiplier for women usually >= 0.75\n");
            }
        }

        // Store scores
        for (int j = 0; j < NUM_TESTS; j++) {
            health_scores[i][j] = scores[j];
        }
    }

    printf("Fitness data recorded successfully.\n");
}

// getHealth(): display fitness data
void getHealth(void) {
    char nickname[MAX_NICKNAME_LEN];
    int member_index = -1;

    printf("[View Fitness Data]\n");
    printf("1. View all members fitness data\n");
    printf("2. View one member's complete fitness data\n");
    printf("3. View one fitness test for one member\n");
    printf("Select an option: ");

    int option;
    scanf("%d", &option);
    getchar(); // consume newline

    if (option == 1) {
        printf("\nAll Members Fitness Data:\n");
        for (int i = 0; i < total_members; i++) {
            printf("\nName: %s, Nickname: %s\n", milliways_members[i].name, milliways_members[i].nickname);
            for (int j = 0; j < NUM_TESTS; j++) {
                printf("%s: %.2f\n", fitness_test_names[j], health_scores[i][j]);
            }
        }
    } else if (option == 2) {
        printf("Enter member nickname: ");
        fgets(nickname, sizeof(nickname), stdin);
        nickname[strcspn(nickname, "\n")] = 0;

        member_index = find_member_by_nickname(nickname);
        if (member_index == -1) {
            printf("Member nickname not found.\n");
            return;
        }

        printf("\nFitness data for %s (%s):\n", milliways_members[member_index].name, nickname);
        for (int j = 0; j < NUM_TESTS; j++) {
            printf("%s: %.2f\n", fitness_test_names[j], health_scores[member_index][j]);
        }
    } else if (option == 3) {
        char test_name[100];
        printf("Enter member nickname: ");
        fgets(nickname, sizeof(nickname), stdin);
        nickname[strcspn(nickname, "\n")] = 0;

        member_index = find_member_by_nickname(nickname);
        if (member_index == -1) {
            printf("Member nickname not found.\n");
            return;
        }

        printf("Enter fitness test name (exactly as below):\n");
        for (int i = 0; i < NUM_TESTS; i++) {
            printf("- %s\n", fitness_test_names[i]);
        }
        printf("Test name: ");
        fgets(test_name, sizeof(test_name), stdin);
        test_name[strcspn(test_name, "\n")] = 0;

        int test_index = -1;
        for (int i = 0; i < NUM_TESTS; i++) {
            if (strcmp(test_name, fitness_test_names[i]) == 0) {
                test_index = i;
                break;
            }
        }
        if (test_index == -1) {
            printf("Fitness test not found.\n");
            return;
        }

        printf("%s's %s: %.2f\n", milliways_members[member_index].name, fitness_test_names[test_index], health_scores[member_index][test_index]);
    } else {
        printf("Invalid option.\n");
    }
}

// Main menu example for testing
int main(void) {
    int choice;
    while (1) {
        printf("\n[Main Menu]\n");
        printf("1. Enter Fitness Data\n");
        printf("2. View Fitness Data\n");
        printf("3. Exit\n");
        printf("Select an option: ");
        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice) {
            case 1:
                setHealth();
                break;
            case 2:
                getHealth();
                break;
            case 3:
                printf("Exiting program.\n");
                return 0;
            default:
                printf("Invalid option. Try again.\n");
        }
    }
    return 0;
}
