##
# Advent of code
#
# @file
# @version 0.1

all:
	@for a in $$(ls -d */ | grep -v "template\|img"); do \
		$(MAKE) -C $$a;\
	done;

clean :
	@for a in $$(ls -d */| grep -v "template\|img"); do \
	$(MAKE) -s -C $$a clean;\
	done;


# end
