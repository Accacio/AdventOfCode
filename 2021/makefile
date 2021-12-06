##
# Advent of code
#
# @file
# @version 0.1

OBJS=$(SRCS:.c=.o)
.PHONY: $(FOLDERS)

all:
	@for a in $$(ls -d */); do \
		$(MAKE) -s -C $$a;\
	done;

clean :
	@for a in $$(ls -d */); do \
	$(MAKE) -s -C $$a clean;\
	done;


# end
