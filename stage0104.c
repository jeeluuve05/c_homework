#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
    char correctness;
} QuizAnswer;

// Global arrays
Question questionBank[MAX_QUESTIONS];
Trainee trainees[MAX_TRAINEES];
QuizAnswer quizAnswers[QUIZ_QUESTIONS];

// Initialize question bank
void initQuestions() {
    for (int i = 0; i < MAX_QUESTIONS; i++) {
        questionBank[i].id = i + 1;
        sprintf(questionBank[i].content, "Translate '%s' to Korean.", 
                (char*[]){"Hello", "Thank you", "Goodbye", "Love", "Water", "Friend", "School", "Book", "Eat", "Sleep"}[i]);
        strcpy(questionBank[i].answer,
               (char*[]){"안녕하세요", "감사합니다", "안녕히 가세요", "사랑", "물", "친구", "학교", "책", "먹다", "자다"}[i]);
    }
}

// Initialize trainees
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

// Random trainee selector
int selectRandomTaker() {
    srand(time(NULL));
    int i;
    do {
        i = rand() % MAX_TRAINEES;
    } while (strcmp(trainees[i].nationality, "Korea") == 0);

    printf("\nWelcome, %s, to the Korean quiz session!\n", trainees[i].name);
    return i;
}

// Check answer correctness and store result
void isAnswer(int index, int questionId, char *userAnswer) {
    strcpy(quizAnswers[index].correctness, 'X');
    quizAnswers[index].questionId = questionId;

    for (int i = 0; i < MAX_QUESTIONS; i++) {
        if (questionBank[i].id == questionId) {
            if (strcmp(questionBank[i].answer, userAnswer) == 0) {
                quizAnswers[index].correctness = 'O';
            }
            break;
        }
    }
}

// Serve 5 unique random questions
void serveRandomQuiz(int traineeIndex) {
    int selectedIds[QUIZ_QUESTIONS];
    int used[MAX_QUESTIONS] = {0};
    int score = 0;

    printf("\nQuiz will start in 30 seconds. Prepare yourself...\n\n");
    sleep(2);  // Replace 2 with 30 in actual implementation
    printf("Starting now!\n");

    srand(time(NULL));
    for (int i = 0; i < QUIZ_QUESTIONS; ) {
        int q = rand() % MAX_QUESTIONS;
        if (!used[q]) {
            used[q] = 1;
            selectedIds[i] = questionBank[q].id;
            printf("\nQ%d: %s\nYour answer: ", i + 1, questionBank[q].content);

            char userAnswer[100];
            getchar();  // Clear buffer
            fgets(userAnswer, sizeof(userAnswer), stdin);
            userAnswer[strcspn(userAnswer, "\n")] = 0;  // Trim newline

            isAnswer(i, questionBank[q].id, userAnswer);
            if (quizAnswers[i].correctness == 'O') {
                score += 20;
            }
            i++;
        }
    }

    // Update trainee
    trainees[traineeIndex].quizScore = score;
    strcpy(trainees[traineeIndex].passStatus, score >= 80 ? "Pass" : "Fail");

    // Show result
    printf("\n=== Quiz Results for %s ===\n", trainees[traineeIndex].name);
    for (int i = 0; i < QUIZ_QUESTIONS; i++) {
        printf("Q%d (ID %d): %c\n", i + 1, quizAnswers[i].questionId, quizAnswers[i].correctness);
    }
    printf("Total Score: %d\nStatus: %s\n", score, trainees[traineeIndex].passStatus);
}

// Main quiz function from menu
void testKoreanLang() {
    int taker = selectRandomTaker();
    serveRandomQuiz(taker);
}

// Menu system (simplified)
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
