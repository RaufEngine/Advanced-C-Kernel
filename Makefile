# Compiler and flags
CC = gcc
CFLAGS = -shared -fPIC -O3 -lm

# Target: Build the shared library
all: libkernel.so

# Rule to compile kernel.c into libkernel.so
libkernel.so: kernel.c
	$(CC) $(CFLAGS) -o $@ $^

# Clean up compiled files
clean:
	rm -f libkernel.so