INC = include
CFLAGS = -g -O0 -I$(INC)
OBJECTS = mixer.o pcm.o
LIB = libtinyalsa.a

TARGETS := $(LIB) \
	tinyplay \
	tinycap \
	tinymix \


all: $(TARGETS)

tinyplay: tinyplay.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

tinycap: tinycap.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

tinymix: tinymix.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

$(LIB): $(OBJECTS)
	$(AR) rc $@ $(OBJECTS)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	-rm *.o $(TARGETS)
