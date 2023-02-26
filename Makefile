make:
	gcc -L/usr/local/Cellar/sdl2/2.26.3/lib -lSDL2 -I/usr/local/Cellar/sdl2/2.26.3/include main.c player.c map.c -o wolfenstein