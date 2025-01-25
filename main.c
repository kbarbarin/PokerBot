#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BUFFER_SIZE 256

// Tableau contenant toutes les cartes possibles
const int TOTAL_CARDS = 52;
const char *ALL_CARDS[TOTAL_CARDS] = {
    "2H", "2D", "2C", "2S", "3H", "3D", "3C", "3S", "4H", "4D", "4C", "4S",
    "5H", "5D", "5C", "5S", "6H", "6D", "6C", "6S", "7H", "7D", "7C", "7S",
    "8H", "8D", "8C", "8S", "9H", "9D", "9C", "9S", "TH", "TD", "TC", "TS",
    "JH", "JD", "JC", "JS", "QH", "QD", "QC", "QS", "KH", "KD", "KC", "KS",
    "AH", "AD", "AC", "AS"
};
bool isPair(char **, char **);
bool isDoublePair(char **, char **);
bool isThreeOfAKind(char **, char **);
bool isStraight(char **, char **);
bool isFlush(char **, char **);
bool isFullHouse(char **, char **);
bool isFourOfAKind(char **, char **);
bool isStraightFlush(char **, char **);

bool (*checkCombinaisons[8])(char **playerCard, char **cards) = {isPair, isDoublePair, isThreeOfAKind, isStraight, isFlush, isFullHouse, isFourOfAKind, isStraightFlush };

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

bool isPair(char **playerCard, char **cards) {
    for (int i = 0; playerCard[i] != NULL; i++) {
        for (int j = 0; cards[j] != NULL; j++) {
            if (playerCard[i][0] ==cards[j][0]) {
                return true;
            }
        }
    }
    return false;
}

bool isDoublePair(char **playerCard, char **cards) {
    int howManyPair = 0;
    
    for (int i = 0; playerCard[i] != NULL; i++) {
        for (int j = 0; cards[j] != NULL; j++) {
            if (playerCard[i][0] ==cards[j][0]) {
                howManyPair++;
            }
        }
    }
    if (howManyPair > 1)
        return true;
    return false;
}

bool isThreeOfAKind(char **playerCard, char **cards) {
    int cardCount[13] = {0};

    for (int i = 0; playerCard[i] != NULL; i++) {
        int index = (playerCard[i][0] >= '2' && playerCard[i][0] <= '9') ? playerCard[i][0] - '2' : 
                    (playerCard[i][0] == 'T') ? 8 :
                    (playerCard[i][0] == 'J') ? 9 :
                    (playerCard[i][0] == 'Q') ? 10 :
                    (playerCard[i][0] == 'K') ? 11 : 12;
        cardCount[index]++;
    }

    for (int j = 0; cards[j] != NULL; j++) {
        int index = (cards[j][0] >= '2' && cards[j][0] <= '9') ? cards[j][0] - '2' : 
                    (cards[j][0] == 'T') ? 8 :
                    (cards[j][0] == 'J') ? 9 :
                    (cards[j][0] == 'Q') ? 10 :
                    (cards[j][0] == 'K') ? 11 : 12;
        cardCount[index]++;
    }

    for (int k = 0; k < 13; k++) {
        if (cardCount[k] == 3) {
            return true;
        }
    }
    return false;
}

bool isStraight(char **playerCard, char **cards) {
    int cardCount[13] = {0};
    int consecutive = 0;

    for (int i = 0; playerCard[i] != NULL; i++) {
        int index = (playerCard[i][0] >= '2' && playerCard[i][0] <= '9') ? playerCard[i][0] - '2' : 
                    (playerCard[i][0] == 'T') ? 8 :
                    (playerCard[i][0] == 'J') ? 9 :
                    (playerCard[i][0] == 'Q') ? 10 :
                    (playerCard[i][0] == 'K') ? 11 : 12;
        cardCount[index]++;
    }

    for (int j = 0; cards[j] != NULL; j++) {
        int index = (cards[j][0] >= '2' && cards[j][0] <= '9') ? cards[j][0] - '2' : 
                    (cards[j][0] == 'T') ? 8 :
                    (cards[j][0] == 'J') ? 9 :
                    (cards[j][0] == 'Q') ? 10 :
                    (cards[j][0] == 'K') ? 11 : 12;
        cardCount[index]++;
    }

    for (int k = 0; k < 13; k++) {
        if (cardCount[k] > 0) {
            consecutive++;
            if (consecutive == 5) return true;
        } else {
            consecutive = 0;
        }
    }
    return false;
}

bool isFlush(char **playerCard, char **cards) {
    int suitCount[4] = {0}; // 0: Hearts, 1: Diamonds, 2: Clubs, 3: Spades

    for (int i = 0; playerCard[i] != NULL; i++) {
        char suit = playerCard[i][1];
        int index = (suit == 'H') ? 0 : (suit == 'D') ? 1 : (suit == 'C') ? 2 : 3;
        suitCount[index]++;
    }

    for (int j = 0; cards[j] != NULL; j++) {
        char suit = cards[j][1];
        int index = (suit == 'H') ? 0 : (suit == 'D') ? 1 : (suit == 'C') ? 2 : 3;
        suitCount[index]++;
    }

    for (int k = 0; k < 4; k++) {
        if (suitCount[k] >= 5) {
            return true;
        }
    }
    return false;
}

bool isFullHouse(char **playerCard, char **cards) {
    return isThreeOfAKind(playerCard, cards) && isDoublePair(playerCard, cards);
}

bool isFourOfAKind(char **playerCard, char **cards) {
    int cardCount[13] = {0};

    for (int i = 0; playerCard[i] != NULL; i++) {
        int index = (playerCard[i][0] >= '2' && playerCard[i][0] <= '9') ? playerCard[i][0] - '2' : 
                    (playerCard[i][0] == 'T') ? 8 :
                    (playerCard[i][0] == 'J') ? 9 :
                    (playerCard[i][0] == 'Q') ? 10 :
                    (playerCard[i][0] == 'K') ? 11 : 12;
        cardCount[index]++;
    }

    for (int j = 0; cards[j] != NULL; j++) {
        int index = (cards[j][0] >= '2' && cards[j][0] <= '9') ? cards[j][0] - '2' : 
                    (cards[j][0] == 'T') ? 8 :
                    (cards[j][0] == 'J') ? 9 :
                    (cards[j][0] == 'Q') ? 10 :
                    (cards[j][0] == 'K') ? 11 : 12;
        cardCount[index]++;
    }

    for (int k = 0; k < 13; k++) {
        if (cardCount[k] == 4) {
            return true;
        }
    }
    return false;
}

bool isStraightFlush(char **playerCard, char **cards) {
    return isStraight(playerCard, cards) && isFlush(playerCard, cards);
}

// Fonction pour vérifier si une carte est utilisée
    bool isThisCardUsed(const char *card, char **usedCards) {
        for (int i = 0; usedCards[i] != NULL; i++) {
            if (strcmp(card, usedCards[i]) == 0) {
                return true; // La carte est utilisée
            }
        }
        return false; // La carte n'est pas utilisée
    }

char **getRestCard(char **playerCard, char **cards) {
    // Allocation dynamique pour restCard
    char **restCard = malloc(sizeof(char *) * (TOTAL_CARDS + 1)); // +1 pour le NULL terminal
    int restIndex = 0;

    // Parcourir toutes les cartes du jeu
    for (int i = 0; i < TOTAL_CARDS; i++) {
        if (!isThisCardUsed(ALL_CARDS[i], playerCard) && !isThisCardUsed(ALL_CARDS[i], cards)) {
            // Ajouter les cartes restantes à restCard
            restCard[restIndex] = malloc(sizeof(char) * 3); // 2 caractères + '\0'
            strcpy(restCard[restIndex], ALL_CARDS[i]);
            restIndex++;
        }
    }

    // Ajouter le NULL terminal
    restCard[restIndex] = NULL;

    return restCard;
}

void simulateOpponents(int playerScore, char **restCard, char **cards) {
    bool buff = false;
    int wins = 0;
    int losses = 0;
    int equity = 0;
    int countCardOne = 0;
    int countCardTwo = 0;
    char **opponentCards = initArray(2, 3);

    for (;  restCard[countCardOne] != NULL; countCardOne++) {
        for (countCardTwo = countCardOne;  restCard[countCardOne] != NULL; countCardTwo++) {
            printf("here\n");
            strcpy(opponentCards[0], restCard[countCardOne]);
            strcpy(opponentCards[1], restCard[countCardTwo]);
            for(int k = 7; k != playerScore - 1; k--) {
                if (checkCombinaisons[k](restCard, cards)) {
                    wins++;
                    buff = true;
                }
            }
            if (buff == false) {
                if (checkCombinaisons[playerScore - 1](restCard, cards))
                    equity++;
                else
                    losses++;
            } else {
                buff = false;
            }
        }
    }
    printf("Wins: %d, Losses: %d, Equity: %d\n", wins, losses, equity);

}

int *generateCombinaison(char **playerCard, char **cards) {
    int possiblesResult = 0;
    int *possibleHands = malloc(sizeof(int) * 9);
    char **restCard = getRestCard(playerCard, cards);

    if (isStraightFlush(restCard, cards)) {
        possibleHands[possiblesResult] = 8;
        possiblesResult++;
    }
    if (isFourOfAKind(restCard, cards)) {
        possibleHands[possiblesResult] = 7;
        possiblesResult++;
    }
    if (isFullHouse(restCard, cards)) {
        possibleHands[possiblesResult] = 6;
        possiblesResult++;
    }
    if (isFlush(restCard, cards)) {
        possibleHands[possiblesResult] = 5;
        possiblesResult++;
    }
    if (isStraight(restCard, cards)) {
        possibleHands[possiblesResult] = 4;
        possiblesResult++;
    }
    if (isThreeOfAKind(restCard, cards)) {
        possibleHands[possiblesResult] = 3;
        possiblesResult++;
    }
    if (isDoublePair(restCard, cards)) {
        possibleHands[possiblesResult] = 2;
        possiblesResult++;
    }
    if (isPair(restCard, cards)) {
        possibleHands[possiblesResult] = 1;
        possiblesResult++;
    }
    possibleHands[possiblesResult] = -1;
    return possibleHands;
}

// Fonction principale
int main(int argc, char **argv) {
    if (argc != 2 || !isNum(argv[1])) {
        printf("Usage: %s <number_of_players>\n", argv[0]);
        return 1;
    }

    char **myCards = initArray(2, 3);
    char **cards = initArray(5, 3);
//    int *possibleHands;
    int numPlayers = atoi(argv[1]);
    (void)numPlayers; // avoid warning

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
            simulateOpponents(3, getRestCard(myCards, cards), cards);
            // possibleHands = generateCombinaison(myCards, cards);
            // for (int i = 0; possibleHands[i] != -1;i++) {
            //     if (possibleHands[i] == 8)
            //         printf("possible Straight Flush\n");
            //     if (possibleHands[i] == 7)
            //         printf("possible Four Of A Kind\n");
            //     if (possibleHands[i] == 6)
            //         printf("possible Full House\n");
            //     if (possibleHands[i] == 5)
            //         printf("possible Flush\n");
            //     if (possibleHands[i] == 4)
            //         printf("possible Straight\n");
            //     if (possibleHands[i] == 3)
            //         printf("possible Three Of A Kind\n");
            //     if (possibleHands[i] == 2)
            //         printf("possible  Double Pair\n");
            //     if (possibleHands[i] == 1)
            //         printf("possible Pair\n");
            // }
        }
        free(line);
    }

    for (int i = 0; i < 2; i++) free(myCards[i]);
    free(myCards);
    for (int i = 0; i < 5; i++) free(cards[i]);
    free(cards);

    return 0;
}
