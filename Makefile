INC = include
CFLAGS = -g -O0 -I$(INC) -Wall
OBJECTS = mixer.o pcm.o
LIB = libtinyalsa.a
LINKOPTS := -static

TARGETS := $(LIB) \
	tinyplay \
	tinycap \
	tinymix \
	pulse-generator \


all: $(TARGETS) all_tone_generator

tinyplay: tinyplay.o $(LIB)
	$(CC) $(LINKOPTS) $(CFLAGS) -o $@ $^ $(LDLIBS)

tinycap: tinycap.o $(LIB)
	$(CC) $(LINKOPTS) $(CFLAGS) -o $@ $^ $(LDLIBS)

tinymix: tinymix.o $(LIB)
	$(CC) $(LINKOPTS) $(CFLAGS) -o $@ $^ $(LDLIBS)

pulse-generator: pulse-generator.o $(LIB)
	$(CC) $(LINKOPTS) $(CFLAGS) -o $@ $^ $(LDLIBS) -lrt

$(LIB): $(OBJECTS)
	$(AR) rc $@ $(OBJECTS)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean: clean_tone_generator
	-rm *.o $(TARGETS)

##############################################
##
## TONE GENERATOR
##
##############################################

TONEGEN_CFLAGS := $(CFLAGS)
TONEGEN_CPPFLAGS := $(CPPFLAGS)
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
	$(CC) -o $@ $(TONEGEN_LDFLAGS) $^ $(LDLIBS) -lm

generate-wave-table.o: generate-wave-table.c
	$(CC) -c -o $@  $(TONEGEN_CPPFLAGS) $(TONEGEN_CFLAGS) $<

table_%.c: generate-wave-table
	./generate-wave-table $* $(TONEGEN_WAVE_LENGTH) > $@

tone-generator: tone-generator.o oscillator-table.o
	$(CC) $(LINKOPTS) -o $@ $(TONEGEN_CPPFLAGS) $(TONEGEN_CFLAGS) $^ $(TONEGEN_LDLIBS)

tone-generator.o: tone-generator.c $(TONEGEN_TABLE_TARGETS)
	$(CC) -c -o $@ $(TONEGEN_CPPFLAGS) $(TONEGEN_CFLAGS) $<

oscillator-table.o: oscillator-table.c
	$(CC) -c -o $@ $(TONEGEN_CPPFLAGS) $(TONEGEN_CFLAGS) $<
