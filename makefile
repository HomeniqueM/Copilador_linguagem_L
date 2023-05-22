# Define o compilador
CC = g++
# Define as opções de compilação
CFLAGS = -std=c++20 -Wall -lstdc++fs
# Define o nome do executável
EXECUTABLE = lcompile
# Define o caminho para o diretório onde os arquivos de origem estão localizados
SRC_DIR = src
# Define o caminho para o diretório onde os arquivos objeto serão gerados
OBJ_DIR = build
# Lista com todos os arquivos de origem
SRC_FILES = $(wildcard $(SRC_DIR)/**/*.cpp $(SRC_DIR)/*.cpp)
# Lista com todos os arquivos objeto a serem gerados
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Regra de compilação dos arquivos objeto
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

# Regra de compilação do executável
$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

# Regra para limpar os arquivos objeto e o executável
clean:
	rm -rf $(OBJ_DIR)
	rm -f $(EXECUTABLE)

# Regra para compilar o código
make: $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJ_FILES)

# Regra para executar o código
run: $(EXECUTABLE)
	./$(EXECUTABLE)

# Indica que a regra "all" compila o executável
all: $(EXECUTABLE)

# Define a regra padrão
.DEFAULT_GOAL := all
