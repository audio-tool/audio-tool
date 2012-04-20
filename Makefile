INC = include
CFLAGS = -g -O0 -I$(INC) -Wall
OBJECTS = mixer.o pcm.o
LIB = libtinyalsa.a
LINKOPTS := -static

HOSTCC := gcc
HOSTCXX := g++
HOSTAR := ar
HOSTLINKOPTS :=
HOSTCFLAGS := $(HOSTLINKOPTS) $(CFLAGS)
HOSTLDFLAGS :=
HOSTLDLIBS :=

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
	tinyplay \
	tinycap \
	tinymix \
	pulse-generator \


all: $(TARGETS) all_tone_generator

tinyplay: tinyplay.o $(LIB)
	$(TARGETCC) $(TARGETCFLAGS) $(TARGETLDFLAGS) -o $@ $^ $(TARGETLDLIBS)

tinycap: tinycap.o $(LIB)
	$(TARGETCC) $(TARGETCFLAGS) $(TARGETLDFLAGS) -o $@ $^ $(TARGETLDLIBS)

tinymix: tinymix.o $(LIB)
	$(TARGETCC) $(TARGETCFLAGS) $(TARGETLDFLAGS) -o $@ $^ $(TARGETLDLIBS)

pulse-generator: pulse-generator.o $(LIB)
	$(TARGETCC) $(TARGETCFLAGS) $(TARGETLDFLAGS) -o $@ $^ $(TARGETLDLIBS)

$(LIB): $(OBJECTS)
	$(TARGETAR) rc $@ $(OBJECTS)

.c.o:
	$(TARGETCC) $(TARGETCFLAGS) -c $<

clean: clean_tone_generator
	-rm *.o $(TARGETS)

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
