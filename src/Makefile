DEFCFLAGS	= -fPIC
DEFLDFLAGS	= -shared -Wl,-soname=lib$(LIBNAME).so$(VERSION)

all: lib$(LIBNAME).so$(VERSION)

lib$(LIBNAME).so$(VERSION): $(LIBNAME).o
	$(CC) $(LDFLAGS) $^ -o $@ $(DEFLDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(DEFCFLAGS)

clean:
	-$(RM) -f *.o
	-$(RM) -f *.so

.PHONY: all clean
