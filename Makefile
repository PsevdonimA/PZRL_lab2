TARGET = sed_simplified
.PHONY: all clean test
all: $(TARGET)
clean:
	rm $(TARGET) *.o
test: $(TARGET)
	./$(TARGET) input.txt '/^[^[:print:]]*$$/d'
main.o: main.c
	gcc -c main.c -Wall -Wextra
lab2.o: lab2.c
	gcc -c lab2.c -Wall -Wextra
lab2f.o: lab2f.c
	gcc -c lab2f.c -Wall -Wextra
$(TARGET): main.o lab2.o lab2f.o
	gcc main.o lab2.o lab2f.o -o $(TARGET) -Wall -Wextra
