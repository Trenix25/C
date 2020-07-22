/*

     list_data_base_items.c
     Lists the iyems in the data base.
     Written by Matthew Campbell.

*/

#include "userdb.h"

void list_data_base_items( user_auth **list )
{
     int count;

     if ( list == NULL || list[ 0 ] == NULL )
     {
          printf( "The data base is empty.\n\n" );
          enter_to_continue();
          return;
     }

     printf( "Current data base:\n\n" );

     count = 0;
     while( list[ count ] != NULL )
     {
          printf( "%2d) Device: %3d, Name: \"%s\"\n", ( count + 1 ),
                  list[ count ]->device, list[ count ]->name );
          count++;
     }

     printf( "\n" );
     enter_to_continue();

     return;
}

/* EOF list_data_base_items.c */
