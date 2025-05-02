.PHONY: all

all: check_uintr

check_uintr: check_uintr.c
	gcc -o check_uintr check_uintr.c

clean:
	rm -f check_uintr
