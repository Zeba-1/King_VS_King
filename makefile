CC=gcc
cflag=-Wall
lflag=-lMLV
OBJ=src/main.o src/printConsol.o src/linkedList.o src/printGraphique.o
HEAD=src/header/printConsol.h src/header/linkedList.h src/header/printGraphique.h src/header/structure.h
EXEC=King-VS-King

$(EXEC): $(OBJ)
	$(CC) $^ -o $@ $(cflag) $(lflag)

src/main.o: src/main.c $(HEAD)

src/printConsol.o: src/printConsol.c src/header/printConsol.h src/header/structure.h

src/printGraphique.o: src/printGraphique.o src/header/printConsol.h src/header/structure.h

src/linkedList.o: src/linkedList.o src/header/linkedList.h src/header/structure.h

src/%.o: src/%.c
	$(CC) $< -c  $(cflag) $(lflag)

clean:
	rm -f src/*.o

mrproper: clean
	rm -f $(EXEC)