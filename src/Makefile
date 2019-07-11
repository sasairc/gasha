#
#    Makefile for libgasha
#

LIBNAME		= libgasha
VERSION		= 1
PATCHLEVEL	= 0
SUBLEVEL	= 0
VERSIONSTR	= $(VERSION).$(PATCHLEVEL).$(SUBLEVEL)
SRCS		= $(wildcard *.c)
OBJS		= $(SRCS:.c=.o)
DEFCFLAGS	= -fPIC
# for GNU/Linux
ifeq ($(shell uname), Linux)
LINKERNAME	= $(LIBNAME).so
DEFLDFLAGS	= -shared		\
		  -Wl,-soname=$(LINKERNAME).$(VERSION)
endif
# for Darwin
ifeq ($(shell uname), Darwin)
LINKERNAME	= $(LIBNAME).dylib
DEFLDFLAGS 	= -dynamiclib -undefined suppress -flat_namespace 	\
		   -install_name $(LINKERNAME)				\
		   -current_version $(VERSIONSTR)			\
		   -compatibility_version $(VERSION).$(PATCHLEVEL)
endif
# for UNIX similar systems
ifndef	LINKERNAME
LINKERNAME	= $(LIBNAME).so
DEFLDFLAGS	= -shared						\
		  -Wl,-soname=$(LINKERNAME).$(VERSION)
endif

all: $(LINKERNAME).$(VERSIONSTR)

$(LINKERNAME).$(VERSIONSTR): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@ $(DEFLDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(DEFCFLAGS)

install-lib: $(LINKERNAME).$(VERSIONSTR)
	install -pd $(LIBDIR)
	install -pm 755 $(LINKERNAME).$(VERSIONSTR) $(LIBDIR)/
	ln -sf $(LIBDIR)/$(LINKERNAME).$(VERSIONSTR) $(LIBDIR)/$(LINKERNAME).$(VERSION)
	ln -sf $(LIBDIR)/$(LINKERNAME).$(VERSIONSTR) $(LIBDIR)/$(LINKERNAME)
# for Darwin
ifeq ($(shell uname), Darwin)
	install_name_tool -change "$(LINKERNAME)" "$(LIBDIR)/$(LINKERNAME)" $(LIBDIR)/$(LINKERNAME)
endif

install-header: gasha.h
	install -pd $(INCLUDEDIR)
	install -pm 644 $< $(INCLUDEDIR)/

clean:
	-$(RM) -f $(LINKERNAME).$(VERSIONSTR)
	-$(RM) -f $(OBJS)

.PHONY: all		\
	install-lib	\
	install-header	\
	clean
