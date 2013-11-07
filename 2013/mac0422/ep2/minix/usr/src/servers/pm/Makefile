# Makefile for Process Manager (PM)

###########################################################
#		adicionei o nome 'lista_ligada.c' na linha 9
###########################################################


PROG=	pm
SRCS=	fila.c lista_ligada.c main.c forkexit.c break.c exec.c time.c timers.c alarm.c \
	signal.c utility.c table.c trace.c getset.c misc.c \
	profile.c dma.c mcontext.c schedule.c

DPADD+=	${LIBSYS} ${LIBTIMERS}
LDADD+=	-lsys -ltimers

MAN=

BINDIR?= /usr/sbin
INSTALLFLAGS+=	-S 32k

CPPFLAGS.main.c+=	-I${MINIXSRCDIR}
CPPFLAGS.misc.c+=	-I${MINIXSRCDIR}
CPPFLAGS.schedule.c+=	-I${MINIXSRCDIR}
CPPFLAGS.utility.c+=	-I${MINIXSRCDIR}

.include <minix.prog.mk>
