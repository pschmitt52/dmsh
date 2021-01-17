/* Dmsh: Dartmouth Menu Shell */
/* $Header: /afs/@cell/uother/source/sep/dmsh/dmsh-1.0/RCS/main.c,v 1.3 2006/04/12 16:10:18 richard Exp $ */

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

int main(argc,argv)
int argc;
char *argv[];
{
    void die();
    char menufile[MAXLINE];

#ifdef LOGGING
    /* 
    Dartmouth Research Computing uses LOCAL2 for application usage logging, but
    this syntax does not match the standard form used by other wrappers. 
    LOCAL4 is unassigned as on 2006/04/11 - use that.  LOCAL0 is used by portsentry
    */
    openlog("admin",LOG_PID,LOG_LOCAL4);
    syslog(LOG_INFO,"MENU version %s Login Starting",VERSION);
#endif
    signal(SIGINT, (void *)die);

    if (argc < 2)
        strcpy(menufile,"main.mnu");
    else
        strcpy(menufile,argv[1]);

    initscr();
    initinput();
    wrap(menufile);
    die();
    return 0;
}
/******************************************************************************
 * INITINPUT - Set up curses
 *****************************************************************************/
void initinput()
{
    raw();
    echo();
    nonl();
}
/******************************************************************************
 * DIE - This function is where to end entire program
 *****************************************************************************/
void die()
{
    signal(SIGINT, SIG_IGN);
    mvcur(0, COLS-1, LINES-1, 0);
    endwin();
    exit(0);
}
/******************************************************************************
 * WRAP - This wraps up the whole program.
 *****************************************************************************/
void wrap(menufile)
char menufile[];
{
    intro(VERSION);
    menu(menufile);
    return;
}
/******************************************************************************
 * INTRO - This prints the program name, version and current time at the top
 *         of the display.
 *****************************************************************************/
void intro(version)
char version[];
{
    int x1, x2, y;
    char *tp;

    /* maybe horizontally centre this in the window, after computing the length */
    
    move(1,2);
    standout();
    addstr(" Dartmouth Menu Shell, version: ");
    getyx(stdscr, y, x1);
    printw("%s", version);
    getyx(stdscr, y, x2);
    while (x2++ - x1 < 6)
        addch(' ');
    tp = (char *)get_time();
    stripcrlf(tp);
    addstr(tp);
    addstr(" ");
    standend();
    move(MESSAGE,1);
    addstr("MESSAGES:");
    clrtobot();
    move(0,0);
    refresh();
}
