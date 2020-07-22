/*

     remove_data_base_entry.c
     Removes an entry from the data base.
     Written by Matthew Campbell.

*/

#include "userdb.h"

int remove_data_base_entry( user_auth ***list )
{
     char buffer[ 5 ];
     int device, pos, ret, save_errno;
     user_auth **ptr;

     /* Make sure we have something we can work with. */

     if ( list == NULL )
     {
          errno = EFAULT;
          return 1;
     }

     ptr = *list;

     if ( ptr == NULL )
     {
          printf( "The data base is empty.\n\n" );
          enter_to_continue();
          return 0;
     }

     printf( "Remove data base entry:\n\n" );

     /* Gather input from the user. */

     ret = read_stdin( buffer, 5,
                       "Which device would you like to remove: ", 1 );

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

     /* Find the device in the data base. */

     pos = 0;
     while( ptr[ pos ] != NULL && ptr[ pos ]->device != device )
     {
          pos++;
     }

     if ( ptr[ pos ] == NULL )
     {
          printf( "Device %d was not found in the data base.\n\n", device );
          enter_to_continue();
          return 0;
     }

     /* Free the memory used by the structure. */

     ptr[ pos ]->device = 0;
     ptr[ pos ]->name[ 0 ] = 0;
     free( ptr[ pos ] );
     ptr[ pos ] = NULL;

     /* Move the null pointer to the end of the list. */

     while( ptr[ ( pos + 1 ) ] != NULL )
     {
          ptr[ pos ] = ptr[ ( pos + 1 ) ];
          pos++;
          ptr[ pos ] = NULL;
     }

     /* Remove the extra null pointer on the end of the array. */

     if ( ( *list = realloc( ptr, ( ( pos + 1 ) *
                             sizeof( user_auth * ) ) ) ) == NULL )
     {
          save_errno = errno;

          printf( "Failed to remove device %d from the data base.\n",
                  device );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          enter_to_continue();

          errno = 0;
          return 1;
     }

     printf( "Device %d has been removed from the data base.\n\n", device );
     enter_to_continue();

     return 0;
}

/* EOF remove_data_base_entry.c */
