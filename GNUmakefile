PROG:=		gibrash
SRCS:=		$(PROG).c option_string.c
HDRS:=		include/$(PROG).h include/option_string.h
OBJS:=		$(SRCS:.c=.o)

CFLAGS= 	-O3 -pipe -flto -ffunction-sections -fdata-sections -fno-semantic-interposition
CFLAGS+=	-Wall -Wextra -Wformat -Wformat-security -Werror=format-security -Wstrict-prototypes -Wmissing-prototypes -Wshadow -Wpointer-arith -Wcast-qual -Wwrite-strings

CFLAGS+=	-DPROG=\"$(PROG)\"

ifneq ($(strip $(VERSION)),)
CFLAGS+=	-DVERSION=\"$(VERSION)\"
endif

ifneq ($(strip $(COMMIT_HASH)),)
CFLAGS+=	-DCOMMIT_HASH=\"$(COMMIT_HASH)\"
endif

LDFLAGS:=	-O3 -flto
LDFLAGS+=	-Wl,-O3
LDFLAGS+=	-Wl,-flto
LDFLAGS+=	-Wl,--gc-sections
LDFLAGS+=	-Wl,--as-needed
LDFLAGS+=	-Wl,--sort-common
LDFLAGS+=	-Wl,-z,pack-relative-relocs
LDFLAGS+=	-Wl,-z,defs

TEST_CFLAGS:=	$(CFLAGS) -I.
TEST_LDFLAGS:=	-lcriterion

INSTALL:=	install
INSTALL_BIN:=	$(INSTALL) -s -m 0755

.PHONY: all clean test install

all: $(PROG)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

$(PROG): $(OBJS)
	$(CC) $(OBJS) -o $(PROG) $(LDFLAGS)

test:
	find test -type f -name '*.c' -print | while read -r f; do \
		bin_file=$$(basename --suffix=.c $${f}); \
		c_source=$${f#test/test_}; \
		$(CC) $(TEST_CFLAGS) $(TEST_LDFLAGS) -o $${bin_file} $${f} $${c_source} && ./$${bin_file}; \
	done

install:
	$(INSTALL_BIN) $(PROG) $(DESTDIR)$(PREFIX)/bin

clean:
	rm -f -- $(PROG) *.o *.gz
