/*

     save_data_base.c
     Saves the data base to the file.
     Written by Matthew Campbell.

*/

#include "userdb.h"

int save_data_base( user_auth ***list )
{
     FILE *fp;
     int count, pos, ret, save_errno;
     user_auth **ptr;

     /* Make sure we have a list pointer that we can use. */

     if ( list == NULL )
     {
          errno = EFAULT;
          return 1;
     }

     ptr = *list;

     if ( ptr == NULL || ptr[ 0 ] == NULL )
     {
          errno = ENODATA;
          return 1;
     }

     /* Find the size of the list. */

     pos = 0;
     while( ptr[ pos ] != NULL )
     {
          pos++;
     }

     /* Allocate the array for the data base. */

     user_auth array[ pos ];

     /* Clear the memory in the array. */

     memset( array, 0, ( sizeof( user_auth ) * pos ) );

     /* Copy the data base into the array. */

     for( count = 0; count < pos; count++ )
     {
          array[ count ].device = ptr[ count ]->device;
          strcpy( array[ count ].name, ptr[ count ]->name );
     }

     /* Open the file. */

     fp = fopen( DATA_BASE_FILE, "w" );
     if ( fp == NULL )
     {
          return 1;
     }

     /* Write the array to the file. */

     count = fwrite( array, sizeof( user_auth ), pos, fp );
     save_errno = errno;

     if ( count == ( -1 ) )
     {
          if ( ferror( fp ) )
          {
               clearerr( fp );
               fclose( fp );
               fp = NULL;
          }

          errno = save_errno;
          return 1;
     }

     if ( ferror( fp ) )
     {
          clearerr( fp );
          fclose( fp );
          fp = NULL;

          printf( "Something went wrong while writing to the file.\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          enter_to_continue();

          errno = 0;
          return 1;
     }

     ret = fflush( fp );
     save_errno = errno;

     if ( ret != 0 || ferror( fp ) )
     {
          if ( ferror( fp ) )
          {
               clearerr( fp );
          }
          fclose( fp );
          fp = NULL;

          printf( "Failed to write out all of the data to the file.\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          enter_to_continue();

          errno = 0;
          return 1;
     }

     ret = fclose( fp );
     fp = NULL;
     save_errno = errno;

     if ( ret != 0 )
     {
          printf( "Something went wrong while trying to close the file.\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          enter_to_continue();

          errno = 0;
          return 1;
     }

     printf( "Wrote %d data base records using %d bytes.\n\n", count,
             ( pos * sizeof( user_auth ) ) );
     enter_to_continue();

     return 0;
}

/* EOF save_data_base.c */
