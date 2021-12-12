src=$(wildcard *.c)
obj=$(src:.c=.o)

all: $(obj)

%.o: %.c
	gcc -Wall $< -o $@

clean:
	@rm -rf $(obj)
