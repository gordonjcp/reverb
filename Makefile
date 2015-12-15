# Makefile
# Part of the simple reverb plugin
#
# Copyright (c) 2015 Gordon JC Pearce <gordonjcp@gjcp.net>
#
# Permission to use, copy, modify, and/or distribute this software for any purpose
# with or without fee is hereby granted, provided that the above copyright notice
# and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
# REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
# FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
# INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
# OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
# TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
# THIS SOFTWARE.

CC	= gcc
CFLAGS	+= -I. -O3 -Wall -fomit-frame-pointer -fstrength-reduce -funroll-loops -ffast-math -c -fPIC -DPIC
LDFLAGS	+= -nostartfiles -shared -Wl,-Bsymbolic -lc -lm -lrt
TARGET = reverb.so
HEADERS = ladspa.h reverb.h
OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))

default: $(TARGET)

all: default

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	echo $(OBJECTS)
	$(CC) -o reverb.so $^ $(LDFLAGS)

clean:
	rm *.o
	rm *.so
	
