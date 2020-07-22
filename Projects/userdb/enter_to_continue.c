/*

     enter_to_continue.c
     Allows a program to tell the user to press Enter to continue.
     Written by Matthew Campbell on Saturday November 3, 2018.

     Example usage:

     printf( "Press Enter to continue." );
     fflush( stdout );
     enter_to_continue();

*/

#include "userdb.h"

void enter_to_continue( void )
{
     char buffer[ 3 ];

     printf( "Press Enter to continue..." );
     fflush( stdout );
     fgets( buffer, 3, stdin );
     printf( "\n" );
     return;
}

/* EOF enter_to_continue.c */
