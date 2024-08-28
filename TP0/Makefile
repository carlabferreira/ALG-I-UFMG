#---------------------------------------------------------------------
# Arquivo       : Makefile que funciona no moodle
# Conteúdo      : Compilar o TP0
# Autor         : Carla Beatriz Ferreira (2022097470)
#---------------------------------------------------------------------
# Opções        : make all - compila tudo
#               : make clean - remove objetos e executável
#---------------------------------------------------------------------

CC = g++
LIBS = -lm
SRC = src
OBJ = obj
INC = include
BIN = bin
OBJS = $(OBJ)/arquivosAleatorios.o $(OBJ)/ordenacao.o $(OBJ)/fibonacci.o $(OBJ)/tp0.o
HDRS = $(INC)/arquivosAleatorios.hpp $(INC)/ordenacao.hpp $(INC)/fibonacci.hpp #.hpp
CFLAGS = -pg -Wall -c -I$(INC)

EXE = $(BIN)/tp0.exe

all: $(EXE)

clean:
	rm -f $(EXE) $(OBJS) gmon.out

$(EXE): $(OBJS) | $(BIN)
	$(CC) $(OBJS) -o $(EXE) $(LIBS)

$(BIN):
	mkdir -p $(BIN)

$(OBJ)/%.o: $(SRC)/%.cpp $(HDRS) | $(OBJ)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ):
	mkdir -p $(OBJ)
	