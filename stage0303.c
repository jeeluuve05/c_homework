#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define KEYWORD "specter"
#define KEYWORD_LEN 7

typedef struct {
    char keyword[KEYWORD_LEN + 1];
    char message[512];
} EasterEgg;

EasterEgg arthurEgg = {
    KEYWORD,
    "I confess. After graduating from university, I was blinded by the arrogance of starting a startup "
    "and recklessly blocked my friends' paths. I painfully learned that when I am the only one convinced "
    "by my idea, it leads to disastrous results. The past Arthur was a ghost of dogmatism and stubbornness."
};

// Converts a character to an 8-bit binary string (MSB to LSB)
void charToBinary(char c, char *binaryStr) {
    for (int i = 7; i >= 0; i--) {
        binaryStr[7 - i] = ((c >> i) & 1) ? '1' : '0';
    }
    binaryStr[8] = '\0';
}

// Print reversed binary representation of keyword letters
void printReversedBinary(const char *keyword, char binaryArr[KEYWORD_LEN][9]) {
    printf("\nKeyword in reversed binary order:\n");
    for (int i = KEYWORD_LEN - 1; i >= 0; i--) {
        printf("%s\n", binaryArr[i]);
    }
}

// Check if user input matches the reversed keyword letters (case-insensitive)
int isRightChar(const char *keyword, const char *input) {
    for (int i = 0; i < KEYWORD_LEN; i++) {
        if (tolower(input[i]) != tolower(keyword[KEYWORD_LEN - 1 - i])) {
            return 0;
        }
    }
    return 1;
}

// Check if user input matches the keyword (case-insensitive)
int isEasterEgg(const char *keyword, const char *inputWord) {
    return strcasecmp(keyword, inputWord) == 0;
}

// Bonus: Shuffle the keyword and print binary with odd/even rearrangement
void shuffleKeywordAndPrintBinary(const char *keyword) {
    char shuffled[KEYWORD_LEN + 1];
    strcpy(shuffled, keyword);

    srand((unsigned)time(NULL));
    for (int i = 0; i < KEYWORD_LEN; i++) {
        int r = rand() % KEYWORD_LEN;
        char tmp = shuffled[i];
        shuffled[i] = shuffled[r];
        shuffled[r] = tmp;
    }

    printf("\n[Bonus] Shuffled keyword: %s\n", shuffled);

    char binaries[KEYWORD_LEN][9];
    for (int i = 0; i < KEYWORD_LEN; i++) {
        charToBinary(shuffled[i], binaries[i]);
    }

    printf("\nReversed binaries:\n");
    for (int i = KEYWORD_LEN - 1; i >= 0; i--) {
        printf("%s\n", binaries[i]);
    }

    printf("\nRearranged by odd and even indices:\n");
    printf("Odd indices:\n");
    for (int i = 1; i < KEYWORD_LEN; i += 2) {
        printf("%s\n", binaries[i]);
    }
    printf("Even indices:\n");
    for (int i = 0; i < KEYWORD_LEN; i += 2) {
        printf("%s\n", binaries[i]);
    }
}

void find_easter_egg() {
    printf("\n<<Arthur's Easter Egg>>\n");

    char binaries[KEYWORD_LEN][9];
    for (int i = 0; i < KEYWORD_LEN; i++) {
        charToBinary(arthurEgg.keyword[i], binaries[i]);
    }

    printReversedBinary(arthurEgg.keyword, binaries);

    char userInput[KEYWORD_LEN + 1];
    while (1) {
        printf("\nEnter the 7 characters corresponding to the reversed binary letters: ");
        scanf("%7s", userInput);

        if (strlen(userInput) != KEYWORD_LEN) {
            printf("You must enter exactly 7 characters.\n");
            continue;
        }

        if (isRightChar(arthurEgg.keyword, userInput)) {
            printf("Correct! Now, combine the characters into the word: ");
            scanf("%7s", userInput);

            if (isEasterEgg(arthurEgg.keyword, userInput)) {
                printf("\n##Easter Egg Discovered!$$\n");
                printf("%s\n", arthurEgg.message);
                break;
            } else {
                printf("That is not the correct keyword. Try again.\n");
            }
        } else {
            printf("Incorrect characters. Try again.\n");
        }
    }

    shuffleKeywordAndPrintBinary(arthurEgg.keyword);
}

void selfManagementAndTeamworkMenu() {
    char input[50];
    while (1) {
        printf("\n-- 2. Self-Management and Teamwork --\n");
        printf("Enter a member nickname or 'exit' to return: ");
        scanf("%49s", input);

        if (strcasecmp(input, "exit") == 0) break;

        if (strcasecmp(input, "Arthur") == 0) {
            find_easter_egg();
        } else {
            printf("No Easter egg for %s. Try again or type 'exit'.\n", input);
        }
    }
}

int main() {
    printf("Welcome to the Millieway Training System!\n");

    char choice;
    while (1) {
        printf("\nMain Menu:\n");
        printf("1. II. Training > 2. Self-Management and Teamwork\n");
        printf("0. Exit\n");
        printf("Choose: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                selfManagementAndTeamworkMenu();
                break;
            case '0':
                printf("Goodbye!\n");
                return 0;
            default:
                printf("Invalid option.\n");
        }
    }
    return 0;
}
