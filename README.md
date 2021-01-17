# dmsh
Dartmouth Menu Shell

dmsh(1) 			     MENU			       dmsh(1)

NAME
       dmsh - Dartmouth Menu Shell

SYNOPSIS
       dmsh [options] [menufile]

OPTIONS
       No options are supported at this time.

DESCRIPTION
       dmsh  is a curses-based menu generator.	It has the ability to run pro-
       grams and access additional menu  files.   The  default	menu  file  is
       called main.mnu in the current working directory.  A menu file pathname
       may also be given on the command line.

MENU FILE DESCRIPTION
       The menu file is a text	file  consisting  of  ":"  delimited  records.
       Lines  beginning  with a '#' are considered comments and ignored.  Each
       record (line) has three fields.	There is a line length	limit  of  512
       bytes

       For  backward  compatability,  the  first  record  in  the menu file is
       reserved for a title of the menu, and is always treated as a "T"  line.
       This  feature  may  go  away.  Also for backward compatability, a blank
       line in the menu file is treated as a "T" line consisting of  a	series
       of dashes, to make simple screen dividers for menu groups.

   Fields
       Field 1 is the description that will show up as a menu selection on the
       screen.	It has a length limit of 80 characters.

       Field 2 is a 1-character (case insensitive)  flag  indicating  how  the
       rest  of the record is to be interpreted.  It is selected from the fol-
       lowing list.  Any other characters in that field will cause the	record
       to be displayed on screen as "(invalid)".

       Field 3 -- the rest of the line, consists of the command to be executed
       when this menu item is selected.  It is passed to the system()  library
       function  and so can contain shell metacharacters.  In the case of sub-
       menus, field 3 is the pathname of the menu file to be opened.

   Field 2 Options
       T      "T" is used to indicate  that field 1 contains  text  to	become
	      part  of	the displayed menu.  It is used for headers or section
	      separators, and any field 3  is  ignored.   No  menu  letter  is
	      assigned	to this line.  Completely blank lines may be generated
	      with a null first field.

       P      "P" indicates that field 3 contains a program or script  to  run
	      (via system()) when this menu item is selected.  dmsh will pause
	      after running the program, until the [RETURN] key is pressed

       N      "N" indicates that field 3 contains a program or script  to  run
	      (via  system()) when this menu item is selected.	On completion,
	      dmsh continues immediately and redraws the current menu.	If the
	      system()	call  exits with a non-zero value, then a warning mes-
	      sage is displayed, and dmsh pauses to allow error messages to be
	      seen, as if "P" had been used.

       Q      "Q"  is interpreted as for "N", but dmsh exits immediately after
	      running the command.  The menu is not redisplayed.

       *      "*" is used  to  disable	menu  items.   The  description  still
	      appears,	but the string "(unavailable)" is appended.  It can be
	      used to disable an item without removing the item completely.

       M      "M" indicates a sub-menu.  The string "(menu)"  is  appended  to
	      the  description and field 3 is interpreted as a file name to be
	      openend and read as a new menu  file.   The  "~user"  syntax  is
	      interpreted if found at the start of the pathname.

   Field 3 options
       The  program  can be a series of shell commands, and programs delimited
       by the semi-colon.  Any /bin/sh metacharacters may be used.   The  com-
       mand  may  contain ":" since no further parsing for fields is performed
       after locating the start of field 3 on the line.

MENU SELECTION
       Each  menu selection is given a letter label,  starting with [a].

       The menu item [q] is added to every menu, and either quits the  program
       (if  at	the  tope level) or returns to the previous menu (if in a sub-
       menu).  The ESC key may also be used in place of 'q'.

       Pressing a single  letter  key  will  run  the  corresponding  command.
       Pressing  the uppercase menu letter will display what would be run, for
       debugging.

Examples
       Main Menu:T:noop
       Internet Tools:M:sub1.mnu
       Today's Date:p:clear;date
       Who's On:p:clear;finger|sort
       Compile a Program:*:noop
       Shell:N:clear;echo "Type exit to return";PS1='$ ';export PS1;$SHELL
       Directory Listing:p:clear;ls -lA
       <this empty line will show as dashes>

       If the program is invoked with the above file as the argument, the display
       appears as follows:
	  Dartmouth Menu Shell, version: <version>  <date and time>

	       Main Menu
	   [a]	Internet Tools (menu)
	   [b]	Today's Date
	   [c]	Who's On
	   [d]	Compile a Program (unavailable)
	   [e]	Shell
	   [f]	Directory Listing
	    ---------------
	   [q]	Quit

	 Select Value:

	MESSAGES:   Enter a letter or [q] to quit

FILES
       main.mnu

REVISIONS
       Sun Jan 17 07:50:19 CST 2021

Dartmouth College		   Jan 2021			       dmsh(1)
