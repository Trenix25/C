/*

     load_data_base.c
     Loads the data base from the file.
     Written by Matthew Campbell.

*/

#include "userdb.h"

int load_data_base( user_auth ***list )
{
     FILE *fp;
     int bytes, count, pos, ret, save_errno;
     struct stat stat_buffer;
     user_auth **ptr;

     /* Make sure we have a list pointer that we can use. */

     if ( list == NULL )
     {
          errno = EFAULT;
          return 1;
     }

     /* If we already have a list, free it first. */

     ptr = *list;

     if ( ptr != NULL )
     {
          count = 0;
          while( ptr[ count ] != NULL )
          {
               ptr[ count ]->device = 0;
               ptr[ count ]->name[ 0 ] = 0;
               free( ptr[ count ] );
               ptr[ count ] = NULL;
               count++;
          }
          free( ptr );
          ptr = NULL;
          *list = NULL;

     }    /* if ( ptr != NULL ) */

     /* Find out how big the data file is. */

     ret = stat( DATA_BASE_FILE, &stat_buffer );
     save_errno = errno;

     if ( ret != 0 )
     {
          if ( save_errno == ENOENT )
          {
               printf( "Failed to find the data base file.\n\n" );
               enter_to_continue();
               return 0;
          }
          else
          {
               return 1;
          }
     }

     bytes = ( int )stat_buffer.st_size;

     if ( bytes == 0 )
     {
          printf( "The data base file is empty.\n\n" );
          enter_to_continue();
          return 0;
     }

     if ( ( bytes % sizeof( user_auth ) ) != 0 )
     {
          printf( "The data base file has been corrupted.\n\n" );
          enter_to_continue();
          return 0;
     }

     /* Find out how many records are in the data base. */

     pos = bytes / sizeof( user_auth );

     /* Allocate the array to hold the data base. */

     user_auth array[ pos ];

     /* Open the file. */

     fp = fopen( DATA_BASE_FILE, "r" );
     save_errno = errno;

     if ( fp == NULL )
     {
          printf( "Failed to open the data base file.\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          enter_to_continue();

          errno = 0;
          return 1;
     }

     /* Read the file. */

     count = fread( array, sizeof( user_auth ), pos, fp );
     save_errno = errno;

     if ( count == ( -1 ) )
     {
          if ( ferror( fp ) )
          {
               clearerr( fp );
          }
          fclose( fp );
          fp = NULL;

          printf( "Failed to read the data base file.\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          enter_to_continue();

          errno = 0;
          return 1;
     }

     if ( count != pos )
     {
          if ( ferror( fp ) )
          {
               clearerr( fp );
          }
          fclose( fp );
          fp = NULL;

          printf( "Failed to read all of the data from the file.\n\n" );
          enter_to_continue();

          errno = 0;
          return 1;
     }

     if ( ferror( fp ) )
     {
          clearerr( fp );
     }
     fclose( fp );
     fp = NULL;

     printf( "Read %d data base records using %d bytes.\n\n", pos,
             ( pos * sizeof( user_auth ) ) );

     /* Create the list. */

     *list = malloc( ( pos + 1 ) * sizeof( user_auth * ) );

     if ( *list == NULL )
     {
          return 1;
     }

     ptr = *list;

     for( count = 0; count < pos; count++ )
     {
          ptr[ count ] = malloc( sizeof( user_auth ) );
          if ( ptr[ count ] == NULL )
          {
               save_errno = errno;

               /* Free everything else. */

               ret = 0;
               while( ret < count )
               {
                    ptr[ ret ]->device = 0;
                    ptr[ ret ]->name[ 0 ] = 0;
                    free( ptr[ ret ] );
                    ptr[ ret ] = NULL;
               }
               free( ptr );
               ptr = NULL;

               printf( "Failed to build the data base list.\n" );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
               enter_to_continue();

               errno = 0;
               return 1;

          }    /* if ( ptr[ count ] == NULL ) */

          ptr[ count ]->device = array[ count ].device;
          strcpy( ptr[ count ]->name, array[ count ].name );

     }    /* for ( count = 0; count < pos; count++ ) */

     /* Add the null pointer to the end of the list. */

     ptr[ pos ] = NULL;

     enter_to_continue();

     return 0;
}

/* EOF load_data_base.c */
