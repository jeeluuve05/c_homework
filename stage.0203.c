#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_MEMBERS 10
#define MAX_NAME_LEN 50
#define MAX_NICKNAME_LEN 20
#define DAYS 6  // Monday-Saturday
#define MAX_EXERCISES_PER_DAY 2
#define MAX_EXERCISE_NAME_LEN 30

extern Member milliways_members[MAX_MEMBERS];
extern int total_members;

// Exercise categories (for selection)
const char* cardio[] = {"Running", "Cycling", "Fast Walking"};
const int cardio_count = 3;

const char* full_body_strength[] = {"Push-ups", "Squats"};
const int full_body_count = 2;

const char* lower_body_strength[] = {"Leg Press", "Leg Curl"};
const int lower_body_count = 2;

const char* upper_body_strength[] = {"Pull-ups", "Chin-ups"};
const int upper_body_count = 2;

const char* core_strength[] = {"Plank", "Crunches"};
const int core_count = 2;

// Store routines per member, day, exercises
char member_routine[MAX_MEMBERS][DAYS][MAX_EXERCISES_PER_DAY][MAX_EXERCISE_NAME_LEN];

// Helper: Convert string to lowercase for case insensitive compare
void strToLower(char* str) {
    for (; *str; str++) *str = tolower(*str);
}

// Helper: Check if exercise belongs to core
int isCoreExercise(const char* exercise) {
    for (int i = 0; i < core_count; i++) {
        if (strcmp(exercise, core_strength[i]) == 0) return 1;
    }
    return 0;
}

// Helper: Print list of members
void printMemberList(void) {
    printf("\nMilliways Members:\n");
    for (int i = 0; i < total_members; i++) {
        printf("%d. %s (Nickname: %s)\n", i+1, milliways_members[i].name, milliways_members[i].nickname);
    }
    printf("\n");
}

// Helper: Print exercise array with numbering
void printExerciseOptions(const char* arr[], int count) {
    for (int i = 0; i < count; i++) {
        printf("%d. %s\n", i+1, arr[i]);
    }
}

// Set the exercise routine
void setExerciseRoutine(void) {
    char buffer[100];
    int coreUsed[MAX_MEMBERS] = {0}; // Track core exercise usage count per member

    const char* day_names[DAYS] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

    printf("Set Basic Workout Routine\n");
    printMemberList();

    for (int m = 0; m < total_members; m++) {
        printf("\nSetting routine for %s (Nickname: %s)\n", milliways_members[m].name, milliways_members[m].nickname);
        coreUsed[m] = 0;  // reset core count

        for (int d = 0; d < DAYS; d++) {
            int cardio_choice = 0, strength_choice = 0;
            int valid = 0;

            while (!valid) {
                printf("\nDay: %s\n", day_names[d]);

                // Cardio selection
                printf("Choose ONE Cardio exercise:\n");
                printExerciseOptions(cardio, cardio_count);
                printf("Enter choice (1-%d): ", cardio_count);
                fgets(buffer, sizeof(buffer), stdin);
                sscanf(buffer, "%d", &cardio_choice);

                if (cardio_choice < 1 || cardio_choice > cardio_count) {
                    printf("Invalid cardio choice, try again.\n");
                    continue;
                }

                // Strength/Core selection
                printf("Choose ONE Strength or Core exercise:\n");
                // List all strength + core exercises for simplicity
                int total_strength_core = full_body_count + lower_body_count + upper_body_count + core_count;
                int choice_map[total_strength_core]; // map option to category

                // Print options and remember mapping
                int option_num = 1;
                printf("Full-body Strength:\n");
                for (int i = 0; i < full_body_count; i++, option_num++) {
                    printf("%d. %s\n", option_num, full_body_strength[i]);
                    choice_map[option_num - 1] = 1;
                }
                printf("Lower-body Strength:\n");
                for (int i = 0; i < lower_body_count; i++, option_num++) {
                    printf("%d. %s\n", option_num, lower_body_strength[i]);
                    choice_map[option_num - 1] = 2;
                }
                printf("Upper-body Strength:\n");
                for (int i = 0; i < upper_body_count; i++, option_num++) {
                    printf("%d. %s\n", option_num, upper_body_strength[i]);
                    choice_map[option_num - 1] = 3;
                }
                printf("Core Strength:\n");
                for (int i = 0; i < core_count; i++, option_num++) {
                    printf("%d. %s\n", option_num, core_strength[i]);
                    choice_map[option_num - 1] = 4;
                }

                printf("Enter choice (1-%d): ", total_strength_core);
                fgets(buffer, sizeof(buffer), stdin);
                sscanf(buffer, "%d", &strength_choice);

                if (strength_choice < 1 || strength_choice > total_strength_core) {
                    printf("Invalid strength/core choice, try again.\n");
                    continue;
                }

                // Check core exercise rule
                if (choice_map[strength_choice - 1] == 4) {  // core selected
                    if (coreUsed[m] >= 1) {
                        printf("Core exercises can only be included once per week. Choose another exercise.\n");
                        continue; // ask again for strength/core exercise
                    }
                }

                valid = 1; // both choices valid
            }

            // Store cardio exercise name
            strncpy(member_routine[m][d][0], cardio[cardio_choice - 1], MAX_EXERCISE_NAME_LEN);
            member_routine[m][d][0][MAX_EXERCISE_NAME_LEN - 1] = '\0';

            // Store strength/core exercise name
            // Map choice to actual exercise name
            int sc_choice = strength_choice - 1;
            int offset = 0;

            if (sc_choice < full_body_count) {
                strncpy(member_routine[m][d][1], full_body_strength[sc_choice], MAX_EXERCISE_NAME_LEN);
            } else if ((sc_choice -= full_body_count) < lower_body_count) {
                strncpy(member_routine[m][d][1], lower_body_strength[sc_choice], MAX_EXERCISE_NAME_LEN);
            } else if ((sc_choice -= lower_body_count) < upper_body_count) {
                strncpy(member_routine[m][d][1], upper_body_strength[sc_choice], MAX_EXERCISE_NAME_LEN);
            } else {
                sc_choice -= upper_body_count;
                strncpy(member_routine[m][d][1], core_strength[sc_choice], MAX_EXERCISE_NAME_LEN);
                coreUsed[m]++;
            }

            member_routine[m][d][1][MAX_EXERCISE_NAME_LEN - 1] = '\0';

            printf("Set for %s: %s and %s\n", day_names[d], member_routine[m][d][0], member_routine[m][d][1]);
        }
    }
    printf("\nWorkout routines set for all members.\n");
}

// Get and display exercise routine by member name
void getExerciseRoutine(void) {
    char input_name[MAX_NAME_LEN];
    int found_index = -1;

    printf("Enter member's real name: ");
    fgets(input_name, sizeof(input_name), stdin);
    input_name[strcspn(input_name, "\n")] = 0;  // strip newline

    // Find member by real name (case sensitive)
    for (int i = 0; i < total_members; i++) {
        if (strcmp(input_name, milliways_members[i].name) == 0) {
            found_index = i;
            break;
        }
    }

    if (found_index == -1) {
        printf("Member with name '%s' not found.\n", input_name);
        return;
    }

    const char* day_names[DAYS] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

    printf("\nWorkout Routine for %s (Nickname: %s):\n", milliways_members[found_index].name, milliways_members[found_index].nickname);
    for (int d = 0; d < DAYS; d++) {
        printf("%s: %s, %s\n", day_names[d], member_routine[found_index][d][0], member_routine[found_index][d][1]);
    }
}
