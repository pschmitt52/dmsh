#
#	Makefile for the Dartmouth Menu Shell (dmsh)
#

# Compile time options
# Use if usage logging via syslogd is desired
# DEFS        = -DLOGGING
# For some additional on-screen diagnostics
# DEFS        = -DDEBUG

# Choose appropriate complier
# CC	      = cc
CC	      = gcc
# Use -Wall with gcc to get maximum warnings.
CFLAGS        = -O -Wall $(DEBUG) $(DEFS)
# Libraries - older systems need both termcap and curses.  At least for RHEL4
# linux, termcap is no longer needed
# LIBS	      = -lcurses -ltermcap 
LIBS	      = -lcurses

BINDEST	      = /usr/local/bin
MANDEST       = /usr/local/man/man1
# Skip trying to use "install" -- too many differences in syntax between the
# various platforms we support.
# (AIX)  INSTALL = /usr/bin/install -M destdir -m mode
# (IRIX) INSTALL = /sbin/install -m destdir -m mode

HDRS	      = menu.h

OBJ = main.o menu.o misc.o

SRCS = main.c menu.c misc.c

dmsh:	        $(OBJ)
		$(CC) $(CFLAGS) $(DEFS) $(OBJ) $(LIBS) -o dmsh

all:		dmsh 

debug:
		make DEBUG=-DDEBUG dmsh

clean:;		rm -f $(OBJ) core dmsh 

index:;		ctags -wx $(HDRS) $(SRCS)

install:        dmsh
		cp -p dmsh $(BINDEST)
		chmod 755 $(BINDEST)/dmsh
		cp -p dmsh.1 $(MANDEST)
		chmod 755 $(MANDEST)/dmsh.1

lpr:		$(HDRS) $(SRCS)
		lpr -p $(HDRS) $(SRCS)

tags:           $(HDRS) $(SRCS); ctags $(HDRS) $(SRCS)

lint:		$(SRCS)
		lint $(SRCS)
