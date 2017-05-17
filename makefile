all: centered normal2D

centered: centered.c
	gcc centered.c `sdl-config --cflags --libs` -lm -std=c11 -o centered

normal2D: normal2D.c
	gcc normal2D.c `sdl-config --cflags --libs` -lm -std=c11 -o normal2D

clean:
	rm centered
	rm normal2D
