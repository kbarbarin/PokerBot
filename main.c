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


bool isErrorInLine (char *str, int turn) {
    char *token = NULL;
    bool returnValue = false;

    if ((turn == 0 && strlen(str) != TWO_CARD) || (turn == 1 && strlen(str) != THREE_CARD) || (turn == 2 && strlen(str) != ONE_CARD) || (turn == 3 && strlen(str) != ONE_CARD)) {
        printf("Wrong number of card.\n");
        return true;
    }
    token = strtok(str, " ");
    while (token != NULL) {
        if (!isValidCard(token)) {
            printf("%s card is not valid\n", token);
            returnValue = true; // Une carte est invalide
        }
        token = strtok(NULL, " "); // Passe à la carte suivante
    }
    return returnValue;
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
        printf("Enter your card:");
        break;
    case 1:
        printf("Flop: Enter the cards:");
        break;
    case 2:
        printf("Turn: Enter new card");
        break;
    case 3:
        printf("River: Enter new card");
        break;
    
    default:
        break;
    }
}

int main(int argc, char **argv) {
    char *line = NULL;

    errorHandling(argc, argv);
    for (int turn = 0 ; turn != 5; turn++) {
        if (turn == 4)
            turn = 0;
        instruction(turn);
        line = my_getline();
        if (isErrorInLine(line, turn)) {
            turn--;
            continue;
        };

    }
}

// turn 0 = Init game ; asking for player card
// turn 1 = asking for flop
// turn 2 = asking for turn
// turn 3 = asking for river
// turn 5 = quiting game