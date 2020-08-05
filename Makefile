all:
	gcc $(shell pkg-config --libs --cflags libevdev) counter.c -o counter
