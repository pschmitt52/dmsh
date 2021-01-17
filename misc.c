/* Dmsh: Dartmouth Menu Shell */
/* $Header: /afs/@cell/uother/source/sep/dmsh/dmsh-1.0/RCS/misc.c,v 1.3 2006/04/12 16:48:25 richard Exp $ */

/**********************************************************************
 
       Copyright (c) 1995 by the Trustees of Dartmouth College.

   Permission to use, copy, modify and distribute these programs and
   their  documentation  for  any  purpose and without fee is hereby
   granted, provided that this copyright notice and  permission  ap-
   pear on all copies and supporting documentation.  The name of the
   Trustees of Dartmouth College may not be used in  advertising  or
   publicity  relating  to  distribution  of  these programs without
   specific prior permission.  Neither  the  Trustees  of  Dartmouth
   College nor the author make representations about the suitability
   of this software for any purpose.  It is provided "as is" without
   expressed or implied warranty.

************************************************************************/
#include "menu.h"

/******************************************************************************
 * CLRMSG - This clears the message line selected
 *****************************************************************************/
void clrmsg(int offset)
{
    move(MESSAGE+offset,10);
    clrtoeol();
    move(0,0);
    refresh();
}
/******************************************************************************
 * CLRWORKAREA - This clears the window between line 3 and MESSAGE line.
 *****************************************************************************/
void clrworkarea()
{
    int i;

    for(i = 3; i < MESSAGE; i++)
    {
        move(i,0);
        clrtoeol();
    }
    refresh();
}

void xclrworkarea()
{
    int i;

    for(i = 3; i < MESSAGE; i++)
    {
        move(i,5);
        clrtoeol();
    }
}
/******************************************************************************
 * PRTMSG - This will print highlighted messages from the MESSAGE line on down.
 *          The offset argument determines which line from the MESSAGE line to
 *          print the message.
 *****************************************************************************/
void prtmsg(message,offset)
char *message;
int offset;
{
    clrmsg(offset);
    move(MESSAGE+offset,12);
    standout();
    printw(" %s ",message);
    standend();
    fflush(stdout);
    refresh();
    return;
}
/******************************************************************************
 *  GET_TIME - returns today's date and time followed by a '\n'
 *****************************************************************************/
char *
get_time()
{
    extern char *ctime();
    static double Tstart;
    static struct timeval start;


    (void) gettimeofday(&start, (struct timezone *)NULL);
    Tstart = (double) start.tv_sec + ((double)start.tv_usec)/1000000.0;
    return(ctime(&start.tv_sec));
}
/******************************************************************************
 * GET_INPUT - This routine takes string input in curses and allows for
 *             backspaces that are interpreted correctly
 *****************************************************************************/
void get_input(string)
char string[];
{
    char c; 
    int i = 0, y, x, Y, X;

    noecho();
    getyx(stdscr,Y,X);
    while((c = getch()) != '\r')
    {
        if (c == '\010' || c == '\177')
        {
            i -= 1;
            if (i < 0) 
            {
                i = 0;
                move(Y,X);
            }
            else
            {
                getyx(stdscr,y,x);
                move(y,x-1);
                delch();
            }
            refresh();
        }
        else
        {
            addch(c);
            refresh();
            string[i++] = c;
        }
    }
    string[i] = '\0';
    return;
}
/******************************************************************************
 * STRIPCRLF - Strip CR-LF from inputted strings here.
 *****************************************************************************/
void
stripcrlf(s)
char s[];
{
    char *cp;

    cp = s + strlen(s) - 1;
    while (cp >= s && (*cp == '\n' || *cp == '\r'))
        *cp-- = '\0';
    return;
}
/****************************************************************************** 
 * FGETLINE - to get a line of text from stdin
 *            borrowed from page 118 of "C! Programming and Practices"
 *            by M.Tim Grady who borrowed it from the K&R white book
 *
 *            6/27/90: Added file i/o instead of just stdin and
 *                     renamed from getline to fgetline
 *
 * 2002/12/30 RB Skip line if the first character is a '#'
 * 
 *****************************************************************************/
int fgetline(fd, s, lim)
FILE *fd;
char s[];
int lim;
{
    int c = 0, i = 0;

    while ((c=fgetc(fd)) != EOF && c == '#') {
        /* skip the rest of this line */
        while ((c=fgetc(fd)) != EOF && c != '\n') continue;
    }
    if (c == EOF) {
        s[i] = '\0';
        return i;
    } else if (c == '\n') {
        s[i++] = c;
        return i;
    } else {
        /* store the first character then continue as before */
        s[i++] = c;
        lim--;
    }

    while (--lim > 0 && (c=fgetc(fd)) != EOF && c != '\n')
        s[i++] = c;
    
    if (c == '\n')
        s[i++] = c;

    s[i] = '\0';

    return(i);
}
/******************************************************************************
 * GET_HIDDEN - This routine takes string input in curses and allows for
 *              backspaces that are interpreted correctly.
 *              All input is hidden and '*' are output instead.
 *****************************************************************************/
void get_hidden(string)
char string[];
{
    int c;
    int i = 0, y, x, Y, X;

    noecho();
    getyx(stdscr,Y,X);
    while((c = getch()) != '\r')
    {
        if (c == '\010' || c == '\177')
        {
            i -= 1;
            if (i < 0)
            {
                i = 0;
                move(Y,X);
            }
            else
            {
                getyx(stdscr,y,x);
                move(y,x-1);
                echo();
                delch();
                noecho();
            }
            refresh();
        }
        else
        {
            addch('*');
            refresh();
            string[i++] = c;
        }
    }
    string[i] = '\0';
    echo();
    return;
}

