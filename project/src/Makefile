COMPILER_TYPE = gnu

CC = gcc

PROG = project
SRCS = main.c Game.c MainMenu.c Mouse.c Graphics.c Button.c Bitmap.c Puzzle.c Keyboard.c Gameplay.c GameOver.c Timer.c SerialPort.c Queue.c RTC.c

CCFLAGS= -Wall -O3

DPADD += ${LIBDRIVER} ${LIBSYS}
LDADD += -llm -ldriver -lsys

LDFLAGS += -L.

MAN=
BINDIR? = /usr/sbin

.include <bsd.prog.mk>
.include <bsd.gcc.mk>