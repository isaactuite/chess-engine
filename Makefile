all:
	gcc -o chess main.c defs.c game_logic.c bot.c -Iinclude -Llib -lSDL3 -lSDL3_image

	./chess