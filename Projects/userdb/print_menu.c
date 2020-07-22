/*

     print_menu.c
     Prints the main menu.
     Written by Matthew Campbell.

*/

#include "userdb.h"

void print_menu( void )
{
     printf( "\
Main Menu:\n\n\
1) List data base entries.\n\
2) Add a new entry to the data base.\n\
3) Remove an entry from the data base.\n\
4) Load the data base entries from the file.\n\
5) Write the data base entries to the file.\n\
6) Exit.\n\n" );
     return;
}

/* EOF print_menu.c */
