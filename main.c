#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "poker.h"

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

    // Supprime le saut de ligne s'il est présent
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    printf("%s\n", input);
    return input;
}

bool isNum(const char *str) {
    for (int i = 0; str[i] != '\0'; i++)
        if (str[i] < '0' || str[i] > '9')
            return false;
    return true;
}

bool isValidCard(char *card) {
    // Vérifie que la carte est d'une longueur correcte (2 ou 3 caractères)
    size_t len = strlen(card);
    if (len < 2 || len > 3) return false;

    // Vérifie la valeur de la carte
    char value = card[0];
    if (!(value == 'A' || value == 'K' || value == 'Q' || value == 'J' || value == 'T' ||
          (value >= '2' && value <= '9'))) {
            return false;
    }

    // Vérifie la couleur de la carte (H, D, C, S)
    char suit = card[len - 1];
    if (!(suit == 'H' || suit == 'D' || suit == 'C' || suit == 'S')) {
        return false;
    }

    return true;
}


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


void errorHandling(int argc, char **argv) {
    if (argc != 2 || !isNum(argv[1])) {
        printf("You need to use 1 argument.");
        exit(84);
    }
}

void instruction(int turn) {
    switch (turn)
    {
    case 0:
        printf("Enter your card: ");
        break;
    case 1:
        printf("Flop: Enter the cards: ");
        break;
    case 2:
        printf("Turn: Enter new card: ");
        break;
    case 3:
        printf("River: Enter new card: ");
        break;
    
    default:
        break;
    }
}

void storeCards(char **cards, char *line, int turn) {
    char *lineCopy = strdup(line); // Duplique la ligne pour ne pas la modifier
    char *card = NULL;
    if (!lineCopy) {
        perror("Duplication failed");
        exit(EXIT_FAILURE);
    }

    card = strtok(lineCopy, " ");
    for (int i = turn; card != NULL; i++) {
        if (i >= 6) { // Ajout de sécurité pour éviter les dépassements
            printf("Too many cards provided.\n");
            break;
        }
        strncpy(cards[i], card, 2); // Copie les deux premiers caractères (carte + couleur)
        cards[i][2] = '\0';         // Ajoute le caractère de fin
        card = strtok(NULL, " ");
    }

    free(lineCopy); // Libère la copie
}


void printCards(char **myCards, char **cards) {
    printf("Your cards: ");
    for (int i = 0; myCards[i] != NULL; i++) {
        printf("%s", myCards[i]);
    }
    printf("\n");
    printf("table cards: ");
    for (int i = 0; cards[i] != NULL; i++) {
        printf("%s", cards[i]);
    }
    printf("\n");
}

char **initArray(int rows, int cols) {
    char **array = malloc(sizeof(char *) * rows); // Alloue un tableau de pointeurs
    if (!array) {
        perror("Allocation failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < rows; i++) {
        array[i] = malloc(sizeof(char) * cols); // Alloue chaque ligne
        if (!array[i]) {
            perror("Allocation failed");
            exit(EXIT_FAILURE);
        }
        memset(array[i], '\0', sizeof(char) * cols); // Initialise à '\0'
    }
    return array; // Plus de `array[rows] = NULL`
}

int main(int argc, char **argv) {
    char *line = NULL;
    char **myCards = initArray(2, 3); // 2 cartes pour le joueur
    char **cards = initArray(5, 3);  // Jusqu'à 5 cartes pour la table

    errorHandling(argc, argv);
    for (int turn = 0; turn < 5; turn++) {
        if (turn == 4) {
            printf("End of the game.\nStarting new game...\n");
            turn = -1; // Recommence le jeu
            continue;
        }

        instruction(turn);
        line = my_getline();

        if (isErrorInLine(line, turn)) {
            turn--;
            continue;
        }

        if (turn == 0) {
            storeCards(myCards, line, 0);
        } else {
            storeCards(cards, line, turn == 1 ? 0 : (turn == 2 ? 3 : 4));
        }
        printCards(myCards, cards);

        free(line);
    }

    // Libération de la mémoire
    for (int i = 0; i < 2; i++) free(myCards[i]);
    free(myCards);

    for (int i = 0; i < 5; i++) free(cards[i]);
    free(cards);

    return 0;
}


// turn 0 = Init game ; asking for player card
// turn 1 = asking for flop
// turn 2 = asking for turn
// turn 3 = asking for river
// turn 5 = quiting game