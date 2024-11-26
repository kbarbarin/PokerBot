# Nom de l'exécutable
TARGET = pokerbot

# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -g -fsanitize=address
LDFLAGS = -fsanitize=address

# Dossiers
SRC_DIR = .
BUILD_DIR = build

# Fichiers sources et objets
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Règle par défaut
all: $(TARGET)

# Construction de l'exécutable
$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "Compilation terminée : $(TARGET)"

# Compilation des fichiers objets
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers compilés
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	@echo "Nettoyage terminé."

# Nettoyage complet
fclean: clean

# Recompilation complète
re: fclean all

# Vérification des erreurs avec AddressSanitizer
asan: CFLAGS += -fsanitize=address -g
asan: re

# Utilisation simplifiée des règles
.PHONY: all clean fclean re asan
