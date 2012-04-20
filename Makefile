INC = include
CFLAGS = -g -O0 -I$(INC) -Wall
OBJECTS = mixer.o pcm.o
LIB = libtinyalsa.a

TARGETS := $(LIB) \
	tinyplay \
	tinycap \
	tinymix \
	pulse-generator \


all: $(TARGETS)

tinyplay: tinyplay.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

tinycap: tinycap.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

tinymix: tinymix.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

pulse-generator: pulse-generator.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS) -lrt

$(LIB): $(OBJECTS)
	$(AR) rc $@ $(OBJECTS)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	-rm *.o $(TARGETS)
