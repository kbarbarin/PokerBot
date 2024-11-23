#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool isNum(char *str) {
    for (int i = 0; str[i] != '\0'; i++)
        if (str[i] < '0' || str[i] > '9')
            return false;
    return true;
}

void errorHandling(int argc, char **argv) {
    if (argc != 2 || !isNum(argv[1]))
        exit(84);
}

int main(int argc, char **argv) {
    
}