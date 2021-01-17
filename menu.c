/* Dmsh: Dartmouth Menu Shell */
/* $Header: /afs/@cell/uother/source/sep/dmsh/dmsh-1.0/RCS/menu.c,v 1.3 2006/04/12 16:47:37 richard Exp $ */

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

void runprog(char *program, int debug, int pause);


/*****************************************************************************/
void menu(file)
char file[];
{
    FILE *menufd;
    MENU *item;
    int recnum = 0, i;
    char line[MAXLINE];
    char command[MAXLINE];
    char *p, *q, *u, newpath[MAXPATH], uname[MAXUSERNAME];
    struct passwd *pwent;
    static int state = 0;   /* used to determine which is top menu */
    int choose();

    state++;

    /* Added 12/16/02 RB for Bio-Epi use primarily */
    /* Resolve any "~user" at the start of the menu file pathname */
    /* We could also do this is the parse() function when the pathname is first read */
    if (file[0] == '~') {
        if ((p=strchr(file,'/'))) {
            /* need buffer overflow checking here */
            u = uname;
            for (q=file+1; q<p; q++,u++) { *u = *q; }
            *u = '\0';
#ifdef DEBUG
            sprintf(command,"Resolving username %s",uname);
            prtmsg(command,0);
            sleep(2);
#endif
            if ((pwent = getpwnam(uname)) != (struct passwd *)NULL) {
                strcpy(newpath,pwent->pw_dir);
                strcat(newpath,"/");
                strcat(newpath,p+1);
#ifdef DEBUG
                sprintf(command,"Attempting to open menu %s",newpath);
                prtmsg(command,0);
                sleep(2);
#endif
            } else {
                sprintf(command,"No such user: %s in menu line: %s",uname,file);
                prtmsg(command,0);
                sleep(2);
                return;
            }

        } else {
                sprintf(command,"Bad pathname specified in menu line: %s",file);
                prtmsg(command,0);
                sleep(2);
                return;
        }   
    } else {
        strncpy(newpath, file, MAXPATH);
    }


    /* open the menu file if it exists */
    if ((menufd = fopen(newpath,"r")) == NULL) {
        sprintf(command,"Unable to open menu %s",newpath);
        prtmsg(command,0);
        sleep(2);
        return;
    }

    /* count number of records in the menu file */
    while(fgetline(menufd,line,MAXLINE) != 0)
        recnum++;
    rewind(menufd);

    /* get enough memory to hold file dynamically */
    /* This allocates memory for non-menu items too - would be better to use a linked list
       allocated line by line */
    item = (MENU *)malloc(recnum*sizeof(MENU));
    if ( item == (MENU *)NULL) {
        sprintf(command,"Unable to allocate memory for menu %s",newpath);
        prtmsg(command,0);
        sleep(2);
        return;
    }

    for(i = 0; i < recnum; i++)  /* read file into malloc'd memory */
    {
        /* if we want to support comment or continuation lines, fgetline() would be the place */
        fgetline(menufd,line,MAXLINE);
        stripcrlf(line);
        parse(line,item+i);  /* parse the fields into structure */
    }

    do {
        display(item,recnum,state);
    } while(choose(item,recnum));

    free(item);
    state--;
    fclose(menufd);

    return;
}
/*****************************************************************************/
void 
parse(line, item)
char line[];
MENU *item;
{
    char *loc;

    if((loc = strchr(line,':')) == NULL) {
        /* any line with no ":" is counted as a menu separator */
        strcpy(item->desc,"---------------");
        item->todo = 'E';
        strcpy(item->object,"");
        return;
    }
    
    *loc = '\0';
    /* need to avoid buffer overflows, and limit to COLS-10 (maybe less left margin?) */
    strcpy(item->desc,line);  /* get first string field */
    ++loc;
    item->todo = *loc;        /* get second char field */
    loc += 2;
    strcpy(item->object,loc); /* get third string field */
    item->label = 0;
}
/*****************************************************************************/
void 
display(item,num,state)
MENU *item;
int num;
int state;
{
    int i, j;
    int offset=0140;  /* 0140 for letters, 060 for numbers */

    clrworkarea();
    move(3,4);standout();
    /* The first line is taken to be a menu title, regardless of how it is parsed */
    printw("    %s    ",item->desc); /* menu title */
    standend();
 
    j=1;
    for(i = 1; i < num; i++) {
        move(i+4,4);
        if((item+i)->todo == '*') {
            /* should unavailable items get an index code ? */
            /* it would be nice to "gray" this out, but attribute A_DIM */
            /* does nothing on some systems and does reverse video on others, so skip */
            printw("[%c]  %s (unavailable)",  j+offset,(item+i)->desc);
            /* (item+i)->label is now assigned, so we may select this item and give a customized error */
            (item+i)->label =  j+offset;
            j++;
        } else if((item+i)->todo == 'M' || (item+i)->todo == 'm') {
            printw("[%c]  %s ",j+offset, (item+i)->desc);
            attron(A_BOLD);
            printw("(menu)");
            attroff(A_BOLD);
            (item+i)->label =  j+offset;
            j++;
        } else if((item+i)->todo == 'E') {
            printw("  %s   ","--------------");
        } else if((item+i)->todo == 'T' || (item+i)->todo == 't') {
            attron(A_BOLD);
            printw("%s",(item+i)->desc); /* additional text - no associated action */
            attroff(A_BOLD);
        } else {
            /* default action is to get an assigned label - haven't sanity checked these yet */
            printw("[%c]  %s",j+offset,(item+i)->desc);
            (item+i)->label = j+offset ;
            j++;
        }
    }
    if (state == 1) {
        mvaddstr(i+5,4, "[q]  Quit");
        prtmsg("Enter a letter or [ESC/q] to quit",0);
    } else {
        mvaddstr(i+5,4, "[ESC/q]  Return to previous menu");
        prtmsg("Enter a letter or [ESC/q] to return",0);
    }
    mvaddstr(i+6,2, "Select Value: ");                     
    move(i+6,16);                       
    refresh();
}
/*****************************************************************************/
int
choose(item,num)
MENU *item;
int num;
{
    int c,i,d=0;
    /* void runprog(); */

    c = getch();
    if (isupper(c)) {
       /* Upper case input means set debug flag for the corresponding lower case menu entry */
       /* lower case means we must check for 'q' not 'Q' */
       d = 1;
       c = tolower(c);
    }
    /* Find the matching entry, by examining the labels */
    /* Original code used i = c - 0140 as an index into the array */
    for (i=1;i<num;i++) { if ((item+i)->label == c) break; }

    if(i >= 1 && i < num) {
        switch((item+i)->todo) {
            case 'M':
            case 'm':
                if (d) {
                   /* debug - just display the menu file we would have read */ 
                   prtmsg("Selection would read menu file:",0);
                   prtmsg((item+i)->object,1);
                   sleep(2);
                   clrmsg(1);
                } else {
                   /* recursively call menu() with a new file */
                   menu((item+i)->object);
                } 
                break;
            case 'N':
            case 'n':
                runprog((item+i)->object,d,0);
                break;
            case 'P':
            case 'p':
                runprog((item+i)->object,d,1);
                break;
            case '*':
                if (d) {
                   runprog((item+i)->object,d,1);
                } else { 
                   prtmsg("This item has been disabled",0);
                   sleep(2);
                   clrmsg(0);
                }
                break;
            case 'Q':
            case 'q':
                runprog((item+i)->object,d,0);
                /* Q means run this item and then immediately Quit */
                /* maybe have an option to exec() instead.  Should we clear the screen */
                clear();
                refresh();
                return 0;
                break;
        }
    } else if ( c == 'q' || c == '\033' )
        /* problem here if we had enough items to assign the letter 'q' */
        return 0;
    else {
        prtmsg("Invalid Entry, try again!",0);
        sleep(2);
        clrmsg(0);
    }
    return 1;
}
/*****************************************************************************/
void
runprog(program, debug, pause)
char program[];
int debug, pause;
{
    int c, pstat;
    char pcopy[MAXLINE];
    char *s;

    clear();
    refresh();
    move(1,1);
    endwin();  /* kill curses */

    /* New - display mode, rather than execute */
    if (debug) {
        /* display on screen, but split into lines at ";" */
        strncpy(pcopy, program, MAXLINE);
        s = strtok(pcopy, ";");
        printf("%s\n",s);
        while ((s = strtok(NULL, ";"))) {
            printf("%s\n",s);
        }
    } else {
        pstat = system(program); /* run the program */
        switch(pstat) {
        case -1:  /* fork failed */
          pause = 1;
          printf("Unable to run program: \"%s\"\n", program);
          break;
        case 0:   /* child process exited with 0 status */
          break;
        default:  /* child process exited with non-zero status */
          pause = 1;
          printf("WARNING: sh -c \"%s\" exited with status %d\n", program, pstat);
          break;
        }
    }

    if(debug||pause) {
        printf("\n\nPress [RETURN] to continue: ");
        /* IRIX wants a flush here or the prompt isn't seen */
        fflush(stdout);
        c = getchar();
    }

    initscr();         /* restart curses */
    intro(VERSION);
    initinput();
}
/*****************************************************************************/
