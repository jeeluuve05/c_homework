#include <stdio.h>
#include <string.h>

#define MAX_MEMBERS 10
#define MAX_NAME 50
#define DAYS_IN_WEEK 7

const char* days[DAYS_IN_WEEK] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };

// Member structure
struct Member {
    char name[MAX_NAME];
    int age;
    int isVegan; // 1 = vegan, 0 = not vegan
};

// Physical condition structure
struct PhysicalCondition {
    float weight;
    float height;
    int energyLevel;
};

// Workout routine structure
struct WorkoutRoutine {
    char routine[DAYS_IN_WEEK][100];
};

// Diet plan structure
struct DietPlan {
    char meals[DAYS_IN_WEEK][100];
    int calories[DAYS_IN_WEEK];
    char allergies[100];
    char allowedIngredients[100];
};

// Member data
struct Member members[MAX_MEMBERS] = {
    { "Alice", 22, 1 },
    { "Bob", 24, 0 },
    { "Charlie", 21, 1 }
};

int memberCount = 3;

struct PhysicalCondition physicalConditions[MAX_MEMBERS];
struct WorkoutRoutine workoutRoutines[MAX_MEMBERS];
struct DietPlan dietPlans[MAX_MEMBERS];

void setDietPlan(int index) {
    printf("Set Diet Plan for %s\n", members[index].name);
    printf("Enter allergies: ");
    scanf(" %[^\n]", dietPlans[index].allergies);
    printf("Enter allowed ingredients: ");
    scanf(" %[^\n]", dietPlans[index].allowedIngredients);
    for (int i = 0; i < DAYS_IN_WEEK; i++) {
        printf("Enter meal for %s: ", days[i]);
        scanf(" %[^\n]", dietPlans[index].meals[i]);
        printf("Enter calories for %s: ", days[i]);
        scanf("%d", &dietPlans[index].calories[i]);
    }
    printf("Diet plan saved.\n");
}

void viewDietPlan(int index) {
    printf("Diet Plan for %s\n", members[index].name);
    printf("Allergies: %s\n", dietPlans[index].allergies);
    printf("Allowed Ingredients: %s\n", dietPlans[index].allowedIngredients);
    for (int i = 0; i < DAYS_IN_WEEK; i++) {
        printf("%s: %s (%d kcal)\n", days[i], dietPlans[index].meals[i], dietPlans[index].calories[i]);
    }
}

void setWorkoutRoutine(int index) {
    printf("Set Workout Routine for %s\n", members[index].name);
    for (int i = 0; i < DAYS_IN_WEEK; i++) {
        printf("Enter workout for %s: ", days[i]);
        scanf(" %[^\n]", workoutRoutines[index].routine[i]);
    }
}

void viewWorkoutRoutine(int index) {
    printf("Workout Routine for %s\n", members[index].name);
    for (int i = 0; i < DAYS_IN_WEEK; i++) {
        printf("%s: %s\n", days[i], workoutRoutines[index].routine[i]);
    }
}

void enterPhysicalCondition(int index) {
    printf("Enter Physical Condition for %s\n", members[index].name);
    printf("Weight (kg): ");
    scanf("%f", &physicalConditions[index].weight);
    printf("Height (cm): ");
    scanf("%f", &physicalConditions[index].height);
    printf("Energy Level (1-10): ");
    scanf("%d", &physicalConditions[index].energyLevel);
}

void viewPhysicalCondition(int index) {
    printf("Physical Condition of %s:\n", members[index].name);
    printf("Weight: %.2f kg\n", physicalConditions[index].weight);
    printf("Height: %.2f cm\n", physicalConditions[index].height);
    printf("Energy Level: %d\n", physicalConditions[index].energyLevel);
}

int selectMember() {
    printf("Select a member:\n");
    for (int i = 0; i < memberCount; i++) {
        printf("%d. %s\n", i + 1, members[i].name);
    }
    int choice;
    scanf("%d", &choice);
    return choice - 1;
}

void displayMenu() {
    printf("\n===== Health & Knowledge Menu =====\n");
    printf("1. Enter Physical Condition\n");
    printf("2. View Physical Condition\n");
    printf("3. Set Workout Routine\n");
    printf("4. View Workout Routine\n");
    printf("5. Set Diet Plan\n");
    printf("6. View Diet Plan\n");
    printf("0. Exit\n");
    printf("Select an option: ");
}

int main() {
    int choice, index;
    while (1) {
        displayMenu();
        scanf("%d", &choice);
        if (choice == 0) break;

        index = selectMember();

        switch (choice) {
            case 1: enterPhysicalCondition(index); break;
            case 2: viewPhysicalCondition(index); break;
            case 3: setWorkoutRoutine(index); break;
            case 4: viewWorkoutRoutine(index); break;
            case 5: setDietPlan(index); break;
            case 6: viewDietPlan(index); break;
            default: printf("Invalid choice.\n");
        }
    }

    return 0;
}
