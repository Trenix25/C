/*

     add_data_base_entry.c
     Adds an entry to the data base.
     Written by Matthew Campbell.

*/

#include "userdb.h"

int add_data_base_entry( user_auth ***list )
{
     char buffer[ 27 ];
     int device, match, pos, ret, save_errno;
     user_auth **ptr;

     /* Make sure we have something we can work with. */

     if ( list == NULL )
     {
          errno = EFAULT;
          return 1;
     }

     printf( "Add data base entry:\n\n" );

     /* Gather information from the user. */

     ret = read_stdin( buffer, 5, "Device number: ", 1 );
     save_errno = errno;
     printf( "\n" );

     if ( ret != 0 )
     {
          return 1;
     }

     ret = sscanf( buffer, "%d", &device );

     if ( ret != 1 )
     {
          printf( "That is not an integer number.\n\n" );
          enter_to_continue();
          return 0;
     }

     if ( device < 2 || device > 254 )
     {
          printf( "That is not a valid device number.\n\n" );
          enter_to_continue();
          return 0;
     }

     ret = read_stdin( buffer, 27, "User's name: ", 1 );
     save_errno = errno;
     printf( "\n" );

     if ( ret != 0 )
     {
          errno = save_errno;
          return 1;
     }

     if ( buffer[ 0 ] == 0 )
     {
          printf( "That is not a valid name.\n\n" );
          enter_to_continue();
          return 0;
     }

     buffer[ 25 ] = 0;  /* Make sure. */

     ptr = *list;

     if ( ptr == NULL )
     {
          /* Create a new list. */

          ptr = malloc( sizeof( user_auth * ) * 2 );

          if ( ptr == NULL )
          {
               return 1;
          }

          /* Create the new slot. */

          ptr[ 0 ] = malloc( sizeof( user_auth ) );

          if ( ptr[ 0 ] == NULL )
          {
               save_errno = errno;
               free( ptr );
               ptr = NULL;

               errno = save_errno;
               return 1;
          }

          /* Clear the block of memory. */

          memset( ptr[ 0 ], 0, sizeof( user_auth ) );

          /* Add the entry to the first slot. */

          ptr[ 0 ]->device = device;
          strcpy( ptr[ 0 ]->name, buffer );

          /* Add the null pointer to the end of the list. */

          ptr[ 1 ] = NULL;

          /* Save the new list. */

          *list = ptr;

          /* Don't sort the list if we only have one entry. */

          pos = 0;
     }
     else  /* ptr != NULL */
     {
          /* Don't list the same device twice. */

          match = pos = 0;
          while( ptr[ pos ] != NULL && match == 0 )
          {
               if ( ptr[ pos ]->device == device )
               {
                    match = 1;
               }
               pos++;
          }

          if ( match == 1 )
          {
               printf( "\
Device number %d is already listed in the data base.\n\n", device );
               enter_to_continue();
               return 0;
          }

          /* Find out how long the list is. */

          pos = 0;
          while( ptr[ pos ] != NULL )
          {
               pos++;
          }

          /* Count the null pointer and add one slot. */

          pos += 2;

          if ( ( *list = realloc( ptr,
               ( pos * sizeof( user_auth * ) ) ) ) == NULL )
          {
               return 1;
          }

          ptr = *list;
          pos--;
          ptr[ pos ] = NULL;  /* Add the null pointer.   */
          pos--;              /* Use the new empty slot. */

          /* Create the new slot. */

          ptr[ pos ] = malloc( sizeof( user_auth ) );
          if ( ptr[ pos ] == NULL )
          {
               save_errno = errno;
               printf( "Failed to add the new entry to the data base.\n" );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
               enter_to_continue();

               errno = 0;
               return 1;
          }

          /* Clear the block of memory. */

          memset( ptr[ pos ], 0, sizeof( user_auth ) );

          /* Save the information in the new slot. */

          ptr[ pos ]->device = device;
          strcpy( ptr[ pos ]->name, buffer );

     }    /* if ( ptr == NULL ) */

     /* Sort the data base. */

     if ( pos != 0 )
     {
          qsort( ptr, ( pos + 1 ), sizeof( user_auth * ), sort_data_base );
     }

     return 0;
}

/* EOF add_data_base_entry.c */
