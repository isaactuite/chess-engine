all:
	gcc -g -o chess main.c defs.c game_logic.c -Iinclude -Llib -lSDL3 -lSDL3_image

	./chess.exe