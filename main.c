#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BUFFER_SIZE 256

// Tableau contenant toutes les cartes possibles
const char *ALL_CARDS[] = {
    "2H", "2D", "2C", "2S", "3H", "3D", "3C", "3S", "4H", "4D", "4C", "4S",
    "5H", "5D", "5C", "5S", "6H", "6D", "6C", "6S", "7H", "7D", "7C", "7S",
    "8H", "8D", "8C", "8S", "9H", "9D", "9C", "9S", "TH", "TD", "TC", "TS",
    "JH", "JD", "JC", "JS", "QH", "QD", "QC", "QS", "KH", "KD", "KC", "KS",
    "AH", "AD", "AC", "AS"
};
const int TOTAL_CARDS = 52;

// Lecture de l'entrée utilisateur
char* my_getline() {
    char *input = malloc(BUFFER_SIZE);
    if (!input) {
        perror("Allocation failed");
        exit(EXIT_FAILURE);
    }

    if (!fgets(input, BUFFER_SIZE, stdin)) {
        free(input);
        perror("Error reading input");
        exit(EXIT_FAILURE);
    }

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    return input;
}

// Vérification si une chaîne est un nombre
bool isNum(const char *str) {
    for (int i = 0; str[i] != '\0'; i++)
        if (str[i] < '0' || str[i] > '9')
            return false;
    return true;
}

// Vérifie si une carte est valide
bool isValidCard(char *card) {
    size_t len = strlen(card);
    if (len < 2 || len > 3) return false;
    char value = card[0];
    if (!(value == 'A' || value == 'K' || value == 'Q' || value == 'J' || value == 'T' || 
          (value >= '2' && value <= '9'))) {
            return false;
    }
    char suit = card[len - 1];
    if (!(suit == 'H' || suit == 'D' || suit == 'C' || suit == 'S')) {
        return false;
    }
    return true;
}

// Vérifie les erreurs dans une ligne de cartes
bool isErrorInLine(char *str, int turn) {
    int expectedCards = (turn == 0) ? 2 : (turn == 1 ? 3 : 1);
    int cardCount = 0;
    char *lineCopy = strdup(str);
    char *token = strtok(lineCopy, " ");
    bool hasError = false;

    while (token != NULL) {
        cardCount++;
        if (!isValidCard(token)) {
            printf("%s card is not valid.\n", token);
            hasError = true;
        }
        token = strtok(NULL, " ");
    }
    free(lineCopy);

    if (cardCount != expectedCards) {
        printf("Wrong number of cards: expected %d, got %d.\n", expectedCards, cardCount);
        hasError = true;
    }
    return hasError;
}

// Affiche les instructions pour chaque étape
void instruction(int turn) {
    switch (turn) {
    case 0:
        printf("Enter your cards (2 cards): ");
        break;
    case 1:
        printf("Enter the Flop (3 cards): ");
        break;
    case 2:
        printf("Enter the Turn (1 card): ");
        break;
    case 3:
        printf("Enter the River (1 card): ");
        break;
    default:
        break;
    }
}

// Stocke les cartes fournies par l'utilisateur
void storeCards(char **cards, char *line, int startIndex) {
    char *lineCopy = strdup(line);
    char *card = strtok(lineCopy, " ");
    for (int i = startIndex; card != NULL; i++) {
        strncpy(cards[i], card, 2);
        cards[i][2] = '\0';
        card = strtok(NULL, " ");
    }
    free(lineCopy);
}

// Initialisation d'un tableau dynamique
char **initArray(int rows, int cols) {
    char **array = malloc(sizeof(char *) * (rows + 1));
    for (int i = 0; i < rows; i++) {
        array[i] = malloc(sizeof(char) * cols);
        memset(array[i], '\0', cols);
    }
    array[rows] = NULL;
    return array;
}

// Vérifie si une carte est utilisée
bool isCardUsed(const char *card, char **myCards, char **tableCards) {
    for (int i = 0; myCards[i] != NULL; i++) {
        if (strcmp(myCards[i], card) == 0) return true;
    }
    for (int i = 0; tableCards[i] != NULL; i++) {
        if (strcmp(tableCards[i], card) == 0) return true;
    }
    return false;
}

// Génère les cartes restantes
char **generateRemainingDeck(char **myCards, char **tableCards, int *remainingCount) {
    char **remainingDeck = malloc(sizeof(char *) * (TOTAL_CARDS + 1));
    int index = 0;

    for (int i = 0; i < TOTAL_CARDS; i++) {
        printf("%d < 52\n", i);
        if (!isCardUsed(ALL_CARDS[i], myCards, tableCards)) {
            remainingDeck[index] = strdup(ALL_CARDS[i]);
            index++;
        }
    }
    remainingDeck[index] = NULL;
    *remainingCount = index;
    return remainingDeck;
}

// Évalue une main (simplifié)
int evaluateHand(char **cards) {
    (void)cards;
    return 0; // Toujours carte haute dans cette version
}

// Simule les adversaires et compare les mains
void simulateOpponents(char **myCards, char **tableCards, char **remainingDeck, int remainingCount, int numPlayers) {
    int wins = 0, losses = 0;
    (void)tableCards;   // Évite le warning
    (void)numPlayers;

    for (int i = 0; i < remainingCount; i++) {
        for (int j = i + 1; j < remainingCount; j++) {
            char *opponentHand[] = { remainingDeck[i], remainingDeck[j], NULL };
            int opponentScore = evaluateHand(opponentHand);
            int playerScore = evaluateHand(myCards);

            if (playerScore > opponentScore) {
                wins++;
            } else {
                losses++;
            }
        }
    }

    printf("Wins: %d, Losses: %d\n", wins, losses);
}



// Fonction principale
int main(int argc, char **argv) {
    if (argc != 2 || !isNum(argv[1])) {
        printf("Usage: %s <number_of_players>\n", argv[0]);
        return 1;
    }

    char **myCards = initArray(2, 3);
    char **cards = initArray(5, 3);
    int numPlayers = atoi(argv[1]);

    for (int turn = 0; turn < 4; turn++) {
        instruction(turn);
        char *line = my_getline();

        if (isErrorInLine(line, turn)) {
            printf("Invalid input, please try again.\n");
            turn--;
            free(line);
            continue;
        }

        if (turn == 0) {
            storeCards(myCards, line, 0);
        } else {
            storeCards(cards, line, turn == 1 ? 0 : (turn == 2 ? 3 : 4));
        }
        free(line);
    }
    printf("Simulation part begin\n");
    int remainingCount = 0;
    char **remainingDeck = generateRemainingDeck(myCards, cards, &remainingCount);
    simulateOpponents(myCards, cards, remainingDeck, remainingCount, numPlayers);

    for (int i = 0; i < 2; i++) free(myCards[i]);
    free(myCards);
    for (int i = 0; i < 5; i++) free(cards[i]);
    free(cards);
    for (int i = 0; i < remainingCount; i++) free(remainingDeck[i]);
    free(remainingDeck);

    return 0;
}
