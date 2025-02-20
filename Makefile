SOURCES = main.c include/utils/util.c
all:
	gcc $(SOURCES) -lraylib -lm -o bin/editor

clean:
	rm -rf bin/*

