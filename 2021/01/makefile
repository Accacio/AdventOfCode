src=$(wildcard *.c)
obj=$(src:.c=.o)

all: $(obj)

%.o: %.c
	gcc $< -o $@

clean:
	@rm -rf $(obj)
