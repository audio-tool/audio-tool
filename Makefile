INC = include
CFLAGS = -g -O0 -I$(INC) -Wall
LIB = libtinyalsa.a
LINKOPTS := -static

HOSTCC := gcc
HOSTCXX := g++
HOSTAR := ar
HOSTLINKOPTS :=
HOSTCFLAGS := $(HOSTLINKOPTS) $(CFLAGS)
HOSTLDFLAGS :=
HOSTLDLIBS :=
HOSTGENGETOPT := gengetopt

# If you set ARCH to anything, then CROSS_COMPILE will be used.
ifneq ($(ARCH),)
	TARGET_CROSS_COMPILE := $(CROSS_COMPILE)
else
	TARGET_CROSS_COMPILE :=
endif
TARGETCC := $(TARGET_CROSS_COMPILE)gcc
TARGETCXX := $(TARGET_CROSS_COMPILE)g++
TARGETAR := $(TARGET_CROSS_COMPILE)ar
TARGETLINKOPTS := -static
TARGETCFLAGS := $(TARGETLINKOPTS) $(CFLAGS)
TARGETLDFLAGS :=
TARGETLDLIBS := $(LIB) -lm -lrt

TARGETS := $(LIB) \
	audio-tool \

LIB_OBJECTS = \
	tinyplay.o \
	tinycap.o \
	tinymix.o \
	mixer.o \
	pcm.o \
	pulse-generator.o \


all: $(TARGETS) all_tone_generator

audio-tool: audio-tool.o config.o cmdline.o $(LIB)
	$(TARGETCC) $(TARGETCFLAGS) $(TARGETLDFLAGS) -o $@ $^ $(TARGETLDLIBS)

config.o: config.c cmdline.h
	$(TARGETCC) $(TARGETCFLAGS) -c -o $@ $<

cmdline.h: cmdline.c

cmdline.c: cmdline.ggo
	$(HOSTGENGETOPT) --input=$< --file-name=cmdline --unamed-opts

tinymix: tinymix.o $(LIB)
	$(TARGETCC) $(TARGETCFLAGS) $(TARGETLDFLAGS) -o $@ $^ $(TARGETLDLIBS)

pulse-generator: pulse-generator.o $(LIB)
	$(TARGETCC) $(TARGETCFLAGS) $(TARGETLDFLAGS) -o $@ $^ $(TARGETLDLIBS)

$(LIB): $(LIB_OBJECTS)
	$(TARGETAR) rc $@ $^

.c.o:
	$(TARGETCC) $(TARGETCFLAGS) -c $<

clean: clean_tone_generator
	-rm -f *.o $(TARGETS)
	-rm -f cmdline.c cmdline.h

##############################################
##
## TONE GENERATOR
##
##############################################

TONEGEN_CFLAGS := $(TARGETCFLAGS)
TONEGEN_LDFLAGS := $(LDFLAGS)
TONEGEN_LDLIBS := $(LDLIBS) $(LIB) -lm

TONEGEN_WAVE_LENGTH = 2048
TONEGEN_EXE_TARGETS = generate-wave-table \
	tone-generator
TONEGEN_TABLE_TARGETS = table_square.c \
	table_sine.c \
	table_triangle.c \
	table_sawtooth.c

# Note: tone-generator depends on the wave tables, so
# they appear first in this list rather than writing
# a rule for each target.
all_tone_generator: $(TONEGEN_TABLE_TARGETS) $(TONEGEN_EXE_TARGETS)

clean_tone_generator:
	-rm -f $(TONEGEN_EXE_TARGETS) *.o
	-rm -f $(TONEGEN_TABLE_TARGETS)

# This program is intended as a build tool, so it's compiled for the host.
generate-wave-table: generate-wave-table.o
	$(HOSTCC) $(HOSTCFLAGS) $(HOSTLDFLAGS) -o $@ $^ $(HOSTLDLIBS) -lm

# This program is intended as a build tool, so it's compiled for the host.
generate-wave-table.o: generate-wave-table.c
	$(HOSTCC) -c $(HOSTCFLAGS) $(HOSTCFLAGS) -o $@ $^

table_%.c: generate-wave-table
	./generate-wave-table $* $(TONEGEN_WAVE_LENGTH) > $@

tone-generator: tone-generator.o oscillator-table.o
	$(TARGETCC) $(TARGETCFLAGS) $(TARGETLDFLAGS) -o $@ $^ $(TARGETLDLIBS)

tone-generator.o: tone-generator.c $(TONEGEN_TABLE_TARGETS)
	$(TARGETCC) -c $(TONEGEN_CPPFLAGS) $(TONEGEN_CFLAGS) -o $@ $<
