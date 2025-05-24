#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PATTERNS 6
#define MAX_DESCRIPTION_LENGTH 200
#define MAX_NAME_LENGTH 100
#define MAX_SONGS 4
#define MAX_PATTERN_NAME_LENGTH 100

// Choreography pattern structure
typedef struct {
    char name[MAX_PATTERN_NAME_LENGTH];
    char description[MAX_DESCRIPTION_LENGTH];
} ChoreographyPattern;

// Tree node for storing choreography patterns in songs
typedef struct TreeNode {
    ChoreographyPattern pattern;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// Global choreography patterns array
ChoreographyPattern patterns[MAX_PATTERNS];

// Sample K-POP song choreography analysis tree
TreeNode* songTrees[MAX_SONGS]; // Stores tree roots for songs

// Function to read choreography patterns from file
int readDancePatterns() {
    FILE *file = fopen("dance_pattern.txt", "r");
    if (!file) {
        printf("Error opening dance pattern file!\n");
        return -1;
    }

    for (int i = 0; i < MAX_PATTERNS; i++) {
        if (fscanf(file, "%s\n", patterns[i].name) != 1) {
            printf("Error reading pattern name\n");
            fclose(file);
            return -1;
        }
        if (!fgets(patterns[i].description, MAX_DESCRIPTION_LENGTH, file)) {
            printf("Error reading pattern description\n");
            fclose(file);
            return -1;
        }
        // Remove trailing newline from description
        patterns[i].description[strcspn(patterns[i].description, "\n")] = 0;
    }

    fclose(file);
    return 0;
}

// Helper: find pattern index by name
int findPatternIndexByName(const char* name) {
    for (int i = 0; i < MAX_PATTERNS; i++) {
        if (strcmp(patterns[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// Insert node at the end of leftmost chain (like a linked list)
void insertLeftmost(TreeNode** root, TreeNode* newNode) {
    if (*root == NULL) {
        *root = newNode;
    } else {
        TreeNode* current = *root;
        while (current->left != NULL) {
            current = current->left;
        }
        current->left = newNode;
    }
}

// Function to read analysis report CSV and build trees for each song
int readAnalysisReport() {
    FILE *file = fopen("analyz_dance-pattern.csv", "r");
    if (!file) {
        printf("Error opening analysis report file!\n");
        return -1;
    }

    char line[512];
    for (int i = 0; i < MAX_SONGS; i++) {
        songTrees[i] = NULL;

        if (!fgets(line, sizeof(line), file)) {
            printf("Error reading song %d line\n", i + 1);
            fclose(file);
            return -1;
        }

        // Parse the line: first token is song name, then patterns separated by commas
        char *token = strtok(line, ",");
        if (!token) {
            printf("Invalid line format for song %d\n", i + 1);
            fclose(file);
            return -1;
        }
        // Song name token is token[0], ignore or store if needed

        // Read remaining tokens as pattern names
        while ((token = strtok(NULL, ",")) != NULL) {
            // Remove newline or spaces
            while (*token == ' ') token++;
            char *newline = strchr(token, '\n');
            if (newline) *newline = 0;

            int idx = findPatternIndexByName(token);
            if (idx == -1) {
                printf("Pattern '%s' not found in patterns list\n", token);
                continue; // skip unknown patterns
            }

            TreeNode* newNode = malloc(sizeof(TreeNode));
            if (!newNode) {
                printf("Memory allocation failed\n");
                fclose(file);
                return -1;
            }
            newNode->pattern = patterns[idx];
            newNode->left = NULL;
            newNode->right = NULL;

            insertLeftmost(&songTrees[i], newNode);
        }
    }

    fclose(file);
    return 0;
}

// Select random pattern description for quiz
int selectRandomPatternDescription(char *selectedDescription) {
    int index = rand() % MAX_PATTERNS;
    strcpy(selectedDescription, patterns[index].description);
    return index;
}

// Verify if user input matches pattern name
int verifyPatternName(int patternIndex, char *inputName) {
    return strcmp(patterns[patternIndex].name, inputName) == 0;
}

// Traverse and print choreography patterns in tree
void printSongPatterns(TreeNode *root) {
    if (root == NULL) return;
    printf("%s: %s\n", root->pattern.name, root->pattern.description);
    printSongPatterns(root->left);
    printSongPatterns(root->right);
}

// Free the tree memory
void freeTree(TreeNode* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

// Main learning function
void learnDancePattern() {
    int correctAnswers = 0;

    // Generate 4 quiz questions
    for (int i = 0; i < 4; i++) {
        char description[MAX_DESCRIPTION_LENGTH];
        int correctIndex = selectRandomPatternDescription(description);

        printf("Description: %s\n", description);
        printf("Enter the pattern name: ");

        char userInput[MAX_PATTERN_NAME_LENGTH];
        if (!fgets(userInput, sizeof(userInput), stdin)) {
            printf("Input error\n");
            return;
        }
        userInput[strcspn(userInput, "\n")] = 0; // remove newline

        if (verifyPatternName(correctIndex, userInput)) {
            printf("Correct!\n");
            correctAnswers++;
        } else {
            printf("Incorrect. The correct answer was: %s\n", patterns[correctIndex].name);
        }
    }

    // Check if user passed quiz
    if (correctAnswers < 3) {
        printf("You answered fewer than 3 questions correctly. Exiting...\n");
        return;
    }

    // Passed - read and build analysis report trees
    if (readAnalysisReport() != 0) {
        printf("Failed to load analysis report\n");
        return;
    }

    // Display song patterns for each song
    printf("\nSong Choreography Patterns:\n");
    for (int i = 0; i < MAX_SONGS; i++) {
        printf("Song %d:\n", i + 1);
        printSongPatterns(songTrees[i]);
        printf("\n");
    }

    // Interactive quiz on chosen song patterns
    printf("Select a song (1-%d): ", MAX_SONGS);
    int songSelection;
    if (scanf("%d", &songSelection) != 1 || songSelection < 1 || songSelection > MAX_SONGS) {
        printf("Invalid song selection\n");
        return;
    }
    getchar(); // consume leftover newline

    TreeNode *current = songTrees[songSelection - 1];
    while (current != NULL) {
        printf("Next pattern: %s\n", current->pattern.description);
        printf("Your answer: ");

        char userInput[MAX_PATTERN_NAME_LENGTH];
        if (!fgets(userInput, sizeof(userInput), stdin)) {
            printf("Input error\n");
            return;
        }
        userInput[strcspn(userInput, "\n")] = 0;

        if (strcmp(current->pattern.name, userInput) == 0) {
            printf("Correct! Proceeding to the next pattern.\n");
            current = current->left; // Move along left branch (linked list)
        } else {
            printf("Incorrect. Exiting...\n");
            break;
        }
    }

    if (current == NULL) {
        printf("Congratulations! You've completed the choreography pattern sequence!\n");
    }

    // Free allocated trees
    for (int i = 0; i < MAX_SONGS; i++) {
        freeTree(songTrees[i]);
        songTrees[i] = NULL;
    }
}

int main() {
    srand((unsigned int)time(NULL)); // Initialize random seed once

    if (readDancePatterns() != 0) {
        return -1;
    }

    learnDancePattern();

    return 0;
}
