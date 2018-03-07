##################################################

MKC_REQUIRE_PROGS +=	awk

##################################################
WARNS   ?=	4
#WARNERR ?=	yes

LIB  =		maa
INCS =		maa.h

SRCS =		xmalloc.c \
	 hash.c set.c stack.c list.c error.c memory.c string.c \
	 debug.c flags.c maa.c prime.c bit.c timer.c \
	 arg.c pr.c sl.c base64.c base26.c source.c parse-concrete.c \
	 text.c log.c

MKC_CHECK_SIZEOF  =	long
MKC_CHECK_HEADERS =	sys/resource.h alloca.h

arg.o arg.os: ${.OBJDIR}/arggram.c arg.c
${.OBJDIR}/arggram.c: arggram.txt
	awk -f ${SRCDIR_maa}/arggram2c < ${SRCDIR_maa}/arggram.txt > $@

CFLAGS +=	-DMAA_MAJOR=${MAA_MAJOR}
CFLAGS +=	-DMAA_MINOR=${MAA_MINOR}
CFLAGS +=	-DMAA_TEENY=${MAA_TEENY}
CFLAGS +=	-Wno-error=strict-prototypes

CPPFLAGS +=	-I. -I${.OBJDIR}

#MAN =		maa.1

#MKC_FEATURES =	strlcpy strlcat

CLEANFILES +=	arggram.c

.PHONY: _manpages
_manpages: ${MAN}

.include "../Makefile.common"

SHLIB_MAJOR       =	${MAA_MAJOR}
SHLIB_MINOR       =	${MAA_MINOR}
#SHLIB_TEENY       =	${MAA_TEENY}

EXPORT_SYMBOLS    =	export.sym

##################################################

.include <mkc.lib.mk>
