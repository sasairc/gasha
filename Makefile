LIBNAME    = gasha
VERSION    =
MAKE	   := make
CC	   := cc
RM	   := rm
CFLAGS	   := -O2 -g -Wall -Wextra
LDFLAGS    :=
export

all clean:
	@$(MAKE) -C ./src	$@

.PHONY: all clean
