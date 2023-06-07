# Detect the operating system
# A variável OS é definida como o resultado do comando shell 'uname'
OS := $(shell uname)

# Set the compiler
# Dependendo do sistema operativo, é definido o compilador
# Neste caso, é usado o mesmo compilador para todos os sistemas operativos, mas você pode mudá-los conforme necessário
ifeq ($(OS), Linux)
	CC = g++
else ifeq ($(OS), Windows_NT)
	CC = g++
else
	CC = g++
endif

# Set the directories
# SRCDIR é o diretório onde estão os arquivos fonte
# BUILDDIR é o diretório onde os arquivos objeto e o executável serão colocados
# TARGET é o caminho para o arquivo executável
SRCDIR = src
BUILDDIR = build
TARGET = $(BUILDDIR)/main

# Set the source files and object files
# SRC encontra todos os arquivos .cpp no diretório de origem e seus subdiretórios
# OBJ mapeia os arquivos .cpp para arquivos .o correspondentes no diretório de construção
SRC = $(wildcard $(SRCDIR)/**/*.cpp $(SRCDIR)/*.cpp)
OBJ = $(SRC:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)


# Set the flags
# CFLAGS são as flags do compilador
CFLAGS = -g -Wall -lstdc++fs

# A target 'all' é a target padrão quando você executa 'make'
# Ela depende de TARGET, então 'make' irá garantir que TARGET seja construído
all: $(TARGET)

# Esta regra diz como construir TARGET
# TARGET depende de OBJ, então make irá primeiro construir OBJ (ou seja, todos os arquivos .o) antes de executar esta regra
# O comando '$(CC) $^ -o $@ $(CFLAGS)' invoca o compilador para linkar todos os arquivos .o em um executável
$(TARGET): $(OBJ)
	$(CC) $^ -o $@ $(CFLAGS)

# Esta regra diz como construir um arquivo .o de um arquivo .cpp correspondente
# A função 'mkdir -p $(@D)' cria o diretório para o arquivo .o, se necessário
# O comando '$(CC) -c $< -o $@ $(CFLAGS)' invoca o compilador para compilar o arquivo .cpp em um arquivo .o
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(@D)
	$(CC) -c $< -o $@ $(CFLAGS)

# .PHONY é usado para especificar targets que não são arquivos
.PHONY: clean run

# A target 'clean' remove o diretório de build inteiro
# No Linux, o comando 'rm -rf $(BUILDDIR)' é usado para excluir o diretório de build
# No Windows, o comando 'RMDIR /S /Q $(subst /,\\,$(BUILDDIR))' é usado para excluir o diretório de build
clean:
ifeq ($(OS), Linux)
	rm -rf $(BUILDDIR)
else ifeq ($(OS), Windows_NT)
	RMDIR /S /Q $(subst /,\\,$(BUILDDIR))
endif

# A target 'run' executa o programa compilado
run: all
ifeq ($(OS), Linux)
	./$(TARGET)
else ifeq ($(OS), Windows_NT)
	$(TARGET)
else
	echo "Unsupported OS"
endif