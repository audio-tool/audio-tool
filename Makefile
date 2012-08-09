INC = include
CFLAGS = -g -O0 -I$(INC) -Wall
LIB = libtinyalsa.a
LINKOPTS := -static

# Versions are 1.0, 1.0.1, 1.0.2, 1.1, 1.1.1, etc.
VERSION_STR := 1.0.99
# Version string bytes: 0xMMmmPP (MM=major, mm=minor, PP=patch)
VERSION := 0x010063

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
TARGETCFLAGS := $(TARGETLINKOPTS) $(CFLAGS) \
	-DVERSION_STR="\"$(VERSION_STR)\"" \
	-DCMDLINE_PARSER_VERSION="\"$(VERSION_STR)\"" \
	-DVERSION=$(VERSION) \

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
	tone-generator.o \
	oscillator-table.o \
	save.o \
	restore.o \
	mixer_cache.o \
	module.o \
	defaults.o \
	alsa-control.o \
	config_cmd.o \

MODULES = \
	card-omap-abe.o \

all: $(TARGETS)

audio-tool: main.o config.o cmdline.o $(LIB) $(MODULES)
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

card-omap45.o: card-omap45.c card-omap-common-4-5.h
	$(TARGETCC) $(TARGETCFLAGS) -o $@ -c $<

card-sdp4430.o: card-sdp4430.c card-omap-common-4-5.h
	$(TARGETCC) $(TARGETCFLAGS) -o $@ -c $<

##############################################
##
## TONE GENERATOR
##
##############################################

TONEGEN_CFLAGS := $(TARGETCFLAGS)
TONEGEN_LDFLAGS := $(LDFLAGS)
TONEGEN_LDLIBS := $(LDLIBS) $(LIB) -lm

TONEGEN_WAVE_LENGTH = 2048
TONEGEN_WAVE_FORMAT = S16
TONEGEN_EXE_TARGETS = generate-wave-table \
	tone-generator
TONEGEN_TABLE_TARGETS = table_square.c \
	table_sine.c \
	table_triangle.c \
	table_sawtooth.c

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
	./generate-wave-table $* $(TONEGEN_WAVE_LENGTH) $(TONEGEN_WAVE_FORMAT) > $@

tone-generator.o: tone-generator.c $(TONEGEN_TABLE_TARGETS)
	$(TARGETCC) -c $(TONEGEN_CPPFLAGS) $(TONEGEN_CFLAGS) -o $@ $<
