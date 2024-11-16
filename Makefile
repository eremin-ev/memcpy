VPATH = lib
#-Werror
WALL = -W -Wall -Wint-to-pointer-cast
CFLAGS = $(WALL) -O0 -g $(DBG) --save-temps
#CFLAGS = $(WALL) -O2
LDFLAGS = -fPIC
CPPFLAGS = --save-temps -Iinclude

bin =	test_memcpy \
	libunsigned.so

all: $(bin)

test_memcpy: memcpy.o memmove.o

test_add: my_add.o

libunsigned.so: memcpy.o memmove.o
	$(LD) -shared $(LDFLAGS) -o $@ $^

tags: *.c *.cc *.h *.S
	ctags -R .

#
# Rely on the implicit rules to build
#

.PHONY: clean
clean:
	rm -vf $(bin) *.i *.o *.s
