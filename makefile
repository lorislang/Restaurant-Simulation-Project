CFLAGS = -std=gnu11 -g3 -O2 -pthread


.PHONY: all
all: restaurant


.PHONY: clean
clean:
	$(RM) restaurant

restaurant: restaurant.c
	$(CC) $(CFLAGS) restaurant.c -o $@