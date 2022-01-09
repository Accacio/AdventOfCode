##
# Advent of code 2021
#
# @file
# @version 0.1

all:
	@for a in $$(ls -d */ | grep -v img); do \
		$(MAKE) -C $$a;\
	done;

clean :
	@for a in $$(ls -d */| grep -v img); do \
	$(MAKE) -s -C $$a clean;\
	done;


# end
