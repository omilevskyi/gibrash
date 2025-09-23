PROG:=		gibrash
SRCS:=		$(PROG).c
HDRS:=		include/$(PROG).h
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

INSTALL:=	install
INSTALL_BIN:=	$(INSTALL) -s -m 0755

.PHONY: all clean install

all: $(PROG)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

$(PROG): $(OBJS)
	$(CC) $(OBJS) -o $(PROG) $(LDFLAGS)

install:
	$(INSTALL_BIN) $(PROG) $(DESTDIR)$(PREFIX)/bin

clean:
	rm -f -- $(PROG) *.o *.gz
