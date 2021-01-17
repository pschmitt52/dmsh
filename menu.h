/* Dmsh: Dartmouth Menu Shell */
/* Updated: Sun Jan 17 07:18:35 CST 2021 */
#include <stdio.h>
#include <string.h>	   /* Most systems use string.h now */
/* #include <strings.h> */ /* Some systems may need strings.h instead */
#include <ctype.h>
#include <curses.h>
#include <signal.h>
#include <sys/file.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>       /* Some systems may need this for sleep() */
/* #include <malloc.h> */    /* included in stdlib.h now */
#include <pwd.h>

#define VERSION "1.03"
#define MAXLINE 512       /* 2002/12/05 RB Increased from 250 for Bio-Epi */
#define MESSAGE (LINES-5) /* was fixed to 19 originally */

#ifndef MAXPATH           /* some systems define this in system headers    */
#define MAXPATH	256       /* 2002/12/16 RB Added for menu filename munging */
#endif

#define MAXUSERNAME 12


typedef struct menu {
   char label;
   char desc[80];
   char todo;
   char object[MAXLINE];  /* changed from 256, to match input buffer length */
}MENU;

/* local function prototypes */

void wrap(char *menufile);
void initinput();
void intro(char *version);
void menu(char *file);
void runprog(char *program, int debug, int pause);
void parse(char *line, MENU *item);
void display(MENU *item, int num, int state);

void clrmsg(int offset);
void clrworkarea();
void xclrworkarea();
void prtmsg(char *message,int offset);
char *get_time();
void get_input(char *string);
void stripcrlf(char *s);
int fgetline(FILE *fd, char *s, int lim);
void get_hidden(char *string);
