VPATH = lib
#-Werror
WALL = -W -Wall -Wint-to-pointer-cast
CFLAGS = $(WALL) -O0 -g $(DBG)
#CFLAGS = $(WALL) -O2

bin =	test_memcpy

all: $(bin)

test_memcpy: CFLAGS += --save-temps
test_memcpy: memcpy.o my_add.o

memcpy.o: ASFLAGS += --save-temps -Iinclude

tags: *.c *.cc *.h *.S
	ctags -R .

#
# Rely on the implicit rules to build
#

.PHONY: clean
clean:
	rm -vf $(bin) *.i *.o *.s
