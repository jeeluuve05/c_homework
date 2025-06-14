#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_JAMOS 24
#define MAX_TRAINEES 10
#define MAX_NAME 50
#define MAX_ATTEMPTS 3
#define QUESTIONS_PER_GAME 5

typedef enum { CONSONANT, VOWEL } JamoType;

typedef struct {
    char hangul[10];
    char alphabet[5];
    char pronunciation[20];
    JamoType type;
} HangulJamo;

typedef struct {
    char name[MAX_NAME];
    char challengeType[3]; // "KE" or "EK"
    int scores[MAX_ATTEMPTS];
    int attemptCount;
} FlashcardResult;

HangulJamo jamoList[MAX_JAMOS] = {
    // Consonants
    {"ㄱ", "g", "giyeok", CONSONANT},
    {"ㄴ", "n", "nieun", CONSONANT},
    {"ㄷ", "d", "digeut", CONSONANT},
    {"ㄹ", "r", "rieul", CONSONANT},
    {"ㅁ", "m", "mieum", CONSONANT},
    {"ㅂ", "b", "bieup", CONSONANT},
    {"ㅅ", "s", "siot", CONSONANT},
    {"ㅇ", "ng", "ieung", CONSONANT},
    {"ㅈ", "j", "jieut", CONSONANT},
    {"ㅊ", "ch", "chieut", CONSONANT},
    {"ㅋ", "k", "kieuk", CONSONANT},
    {"ㅌ", "t", "tieut", CONSONANT},
    {"ㅍ", "p", "pieup", CONSONANT},
    {"ㅎ", "h", "hieut", CONSONANT},
    // Vowels
    {"ㅏ", "a", "a", VOWEL},
    {"ㅑ", "ya", "ya", VOWEL},
    {"ㅓ", "eo", "eo", VOWEL},
    {"ㅕ", "yeo", "yeo", VOWEL},
    {"ㅗ", "o", "o", VOWEL},
    {"ㅛ", "yo", "yo", VOWEL},
    {"ㅜ", "u", "u", VOWEL},
    {"ㅠ", "yu", "yu", VOWEL},
    {"ㅡ", "eu", "eu", VOWEL},
    {"ㅣ", "i", "i", VOWEL}
};

FlashcardResult results[MAX_TRAINEES];
int traineeCount = 0;

// Utility to get jamo subset
int getJamoSubset(HangulJamo subset[], JamoType type) {
    int count = 0;
    for (int i = 0; i < MAX_JAMOS; i++) {
        if (jamoList[i].type == type) {
            subset[count++] = jamoList[i];
        }
    }
    return count;
}

int getUniqueRandomIndices(int indices[], int max, int total) {
    int count = 0, unique, randIndex;
    while (count < total) {
        unique = 1;
        randIndex = rand() % max;
        for (int i = 0; i < count; i++) {
            if (indices[i] == randIndex) {
                unique = 0;
                break;
            }
        }
        if (unique) {
            indices[count++] = randIndex;
        }
    }
    return count;
}

FlashcardResult* findOrCreateResult(char *name, char *type) {
    for (int i = 0; i < traineeCount; i++) {
        if (strcmp(results[i].name, name) == 0 && strcmp(results[i].challengeType, type) == 0) {
            return &results[i];
        }
    }
    FlashcardResult *newResult = &results[traineeCount++];
    strcpy(newResult->name, name);
    strcpy(newResult->challengeType, type);
    newResult->attemptCount = 0;
    return newResult;
}

void playKor2EngGame(char *name) {
    int choice;
    printf("1. Consonants\n2. Vowels\nChoose type: ");
    scanf("%d", &choice);

    HangulJamo subset[MAX_JAMOS];
    int subsetCount = getJamoSubset(subset, (choice == 1) ? CONSONANT : VOWEL);

    int indices[QUESTIONS_PER_GAME];
    getUniqueRandomIndices(indices, subsetCount, QUESTIONS_PER_GAME);

    char answer[50];
    int score = 0;
    getchar(); // Clear newline

    for (int i = 0; i < QUESTIONS_PER_GAME; i++) {
        HangulJamo j = subset[indices[i]];
        printf("Q%d. What is the English letter and pronunciation of %s? (format: letter,pronunciation): ", i + 1, j.hangul);
        fgets(answer, sizeof(answer), stdin);
        answer[strcspn(answer, "\n")] = 0;

        char expected[50];
        sprintf(expected, "%s,%s", j.alphabet, j.pronunciation);
        if (strcmp(answer, expected) == 0) {
            printf("Correct!\n");
            score += 10;
        } else {
            printf("Incorrect. Correct answer: %s\n", expected);
        }
    }

    FlashcardResult *res = findOrCreateResult(name, "KE");
    if (res->attemptCount < MAX_ATTEMPTS) {
        res->scores[res->attemptCount++] = score;
    }

    printf("Your score: %d/50\n", score);
}

void playEng2KorGame(char *name) {
    int choice;
    printf("1. Consonants\n2. Vowels\nChoose type: ");
    scanf("%d", &choice);

    HangulJamo subset[MAX_JAMOS];
    int subsetCount = getJamoSubset(subset, (choice == 1) ? CONSONANT : VOWEL);

    int indices[QUESTIONS_PER_GAME];
    getUniqueRandomIndices(indices, subsetCount, QUESTIONS_PER_GAME);

    char answer[10];
    int score = 0;
    getchar();

    for (int i = 0; i < QUESTIONS_PER_GAME; i++) {
        HangulJamo j = subset[indices[i]];
        printf("Q%d. What is the Hangul character for \"%s (%s)\"? ", i + 1, j.alphabet, j.pronunciation);
        fgets(answer, sizeof(answer), stdin);
        answer[strcspn(answer, "\n")] = 0;

        if (strcmp(answer, j.hangul) == 0) {
            printf("Correct!\n");
            score += 10;
        } else {
            printf("Incorrect. Correct answer: %s\n", j.hangul);
        }
    }

    FlashcardResult *res = findOrCreateResult(name, "EK");
    if (res->attemptCount < MAX_ATTEMPTS) {
        res->scores[res->attemptCount++] = score;
    }

    printf("Your score: %d/50\n", score);
}

void showResults() {
    printf("\n--- Results ---\n");
    for (int i = 0; i < traineeCount; i++) {
        int total = 0;
        for (int j = 0; j < results[i].attemptCount; j++) {
            total += results[i].scores[j];
        }
        float avg = (results[i].attemptCount > 0) ? (float)total / results[i].attemptCount : 0;
        printf("Name: %s | Type: %s | Avg Score: %.2f\n",
               results[i].name, results[i].challengeType, avg);
    }
}

void learnHangul() {
    char name[MAX_NAME];
    printf("Enter the name of a Millieways trainee: ");
    scanf("%s", name);

    while (1) {
        int choice;
        printf("\n1. Hangul to English\n2. English to Hangul\n3. View Results\n0. Back\nChoose: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: playKor2EngGame(name); break;
            case 2: playEng2KorGame(name); break;
            case 3: showResults(); break;
            case 0: return;
            default: printf("Invalid choice.\n");
        }
    }
}

int main() {
    srand(time(NULL));
    learnHangul();
    return 0;
}
