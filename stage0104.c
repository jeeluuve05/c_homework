#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h> // For Sleep() on Windows
#else
#include <unistd.h>  // For sleep() on Linux/Mac
#endif

#define MAX_TRAINEES 4
#define MAX_QUESTIONS 10
#define QUIZ_QUESTIONS 5

typedef struct {
    int id;
    char content[100];
    char answer[100];
} Question;

typedef struct {
    char name[50];
    char nickname[50];
    char nationality[30];
    int quizScore;
    char passStatus[10];
} Trainee;

typedef struct {
    int questionId;
    char correctness; // 'O' or 'X'
} QuizAnswer;

// Global arrays
Question questionBank[MAX_QUESTIONS];
Trainee trainees[MAX_TRAINEES];
QuizAnswer quizAnswers[QUIZ_QUESTIONS];

// Initialize question bank
void initQuestions() {
    const char *englishWords[MAX_QUESTIONS] = {
        "Hello", "Thank you", "Goodbye", "Love", "Water",
        "Friend", "School", "Book", "Eat", "Sleep"
    };
    const char *koreanTranslations[MAX_QUESTIONS] = {
        "안녕하세요", "감사합니다", "안녕히 가세요", "사랑", "물",
        "친구", "학교", "책", "먹다", "자다"
    };

    for (int i = 0; i < MAX_QUESTIONS; i++) {
        questionBank[i].id = i + 1;
        sprintf(questionBank[i].content, "Translate '%s' to Korean.", englishWords[i]);
        strcpy(questionBank[i].answer, koreanTranslations[i]);
    }
}

// Initialize trainee data
void initTrainees() {
    strcpy(trainees[0].name, "John Doe");
    strcpy(trainees[0].nickname, "JD");
    strcpy(trainees[0].nationality, "USA");

    strcpy(trainees[1].name, "Maria Gomez");
    strcpy(trainees[1].nickname, "MG");
    strcpy(trainees[1].nationality, "Mexico");

    strcpy(trainees[2].name, "Saito Kenji");
    strcpy(trainees[2].nickname, "Ken");
    strcpy(trainees[2].nationality, "Japan");

    strcpy(trainees[3].name, "Park Ji-yeon");
    strcpy(trainees[3].nickname, "Jiyeon");
    strcpy(trainees[3].nationality, "Korea");
}

// Randomly select a trainee who is not Korean
int selectRandomTaker() {
    srand(time(NULL));
    int i;
    do {
        i = rand() % MAX_TRAINEES;
    } while (strcmp(trainees[i].nationality, "Korea") == 0);

    printf("\nWelcome, %s, to the Korean quiz session!\n", trainees[i].name);
    return i;
}

// Check correctness of answer and record result
void isAnswer(int index, int questionId, char *userAnswer) {
    quizAnswers[index].questionId = questionId;
    quizAnswers[index].correctness = 'X'; // Default: incorrect

    for (int i = 0; i < MAX_QUESTIONS; i++) {
        if (questionBank[i].id == questionId) {
            if (strcmp(questionBank[i].answer, userAnswer) == 0) {
                quizAnswers[index].correctness = 'O'; // Correct
            }
            break;
        }
    }
}

// Run quiz with 5 random unique questions
void serveRandomQuiz(int traineeIndex) {
    int selectedIds[QUIZ_QUESTIONS];
    int used[MAX_QUESTIONS] = {0};
    int score = 0;

    printf("\nQuiz will start in 3 seconds. Prepare yourself...\n");
#ifdef _WIN32
    Sleep(3000);
#else
    sleep(3);
#endif
    printf("Starting now!\n");

    srand(time(NULL));
    for (int i = 0; i < QUIZ_QUESTIONS;) {
        int q = rand() % MAX_QUESTIONS;
        if (!used[q]) {
            used[q] = 1;
            selectedIds[i] = questionBank[q].id;
            printf("\nQ%d: %s\nYour answer: ", i + 1, questionBank[q].content);

            char userAnswer[100];
            fgets(userAnswer, sizeof(userAnswer), stdin);
            userAnswer[strcspn(userAnswer, "\n")] = 0; // Trim newline

            isAnswer(i, questionBank[q].id, userAnswer);
            if (quizAnswers[i].correctness == 'O') {
                score += 20;
            }
            i++;
        }
    }

    trainees[traineeIndex].quizScore = score;
    strcpy(trainees[traineeIndex].passStatus, score >= 80 ? "Pass" : "Fail");

    printf("\n=== Quiz Results for %s ===\n", trainees[traineeIndex].name);
    for (int i = 0; i < QUIZ_QUESTIONS; i++) {
        printf("Q%d (ID %d): %c\n", i + 1, quizAnswers[i].questionId, quizAnswers[i].correctness);
    }
    printf("Total Score: %d\nStatus: %s\n", score, trainees[traineeIndex].passStatus);
}

// Main function to start quiz
void testKoreanLang() {
    int taker = selectRandomTaker();
    // Clear input buffer before starting
    getchar();
    serveRandomQuiz(taker);
}

// Simple menu interface
void showMenu() {
    int choice;
    do {
        printf("\n== Millieways Training Menu ==\n");
        printf("1. Physical Training\n");
        printf("2. Mental Training\n");
        printf("3. Language and Pronunciation Training\n");
        printf("0. Exit\n");
        printf("Select: ");
        scanf("%d", &choice);

        if (choice == 3) {
            printf("\nA. Korean Quiz\n");
            printf("Select option: ");
            char sub;
            scanf(" %c", &sub);
            if (sub == 'A' || sub == 'a') {
                testKoreanLang();
            }
        }
    } while (choice != 0);
}

int main() {
    initQuestions();
    initTrainees();
    showMenu();
    return 0;
}
