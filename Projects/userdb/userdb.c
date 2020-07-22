/*

     userdb.c
     This program creates and manages the user data base.
     Written by Matthew Campbell.

*/

#include "userdb.h"

int main( void )
{
     char buffer[ 3 ];
     int choice, count, exit_loop, ret, save_errno;
     user_auth **list;

     /* Initialize the data base. */

     list = NULL;

     exit_loop = 0;
     do
     {
          print_menu();

          errno = 0;
          ret = read_stdin( buffer, 3, ">> ", 1 );
          save_errno = errno;
          printf( "\n" );

          if ( ret != 0 )
          {
               printf( "Something went wrong while reading your input.\n" );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
               exit( EXIT_FAILURE );
          }

          ret = sscanf( buffer, "%d", &choice );
          if ( ret != 1 )
          {
               printf( "That is not an integer number.\n\n" );
          }
          else
          {
               if ( choice < 1 || choice > 6 )
               {
                    printf( "That is not a valid option.\n\n" );
               }
               else
               {
                    switch( choice )
                    {
                         case 1 : list_data_base_items( list );
                                  break;

                         case 2 : errno = 0;
                                  ret = add_data_base_entry( &list );
                                  save_errno = errno;

                                  if ( ret != 0 )
                                  {
                                       printf( "\
Something went wrong while trying to add the data base entry.\n" );
                                       if ( save_errno != 0 )
                                       {
                                            printf( "Error: %s.\n",
                                                    strerror( save_errno ) );
                                       }
                                       printf( "\n" );
                                       exit( EXIT_FAILURE );
                                  }
                                  break;

                         case 3 : errno = 0;
                                  ret = remove_data_base_entry( &list );
                                  save_errno = errno;

                                  if ( ret != 0 )
                                  {
                                       printf( "\
Something went wrong while trying to remove the data base entry.\n" );
                                       if ( save_errno != 0 )
                                       {
                                            printf( "Error: %s.\n",
                                                    strerror( save_errno ) );
                                       }
                                       printf( "\n" );
                                       exit( EXIT_FAILURE );
                                  }
                                  break;

                         case 4 : errno = 0;
                                  ret = load_data_base( &list );
                                  save_errno = errno;

                                  if ( ret != 0 && save_errno != 0 )
                                  {
                                       printf( "\
Something went wrong while trying to load the data base from the file.\n" );
                                       printf( "Error: %s.\n\n",
                                               strerror( save_errno ) );
                                       exit( EXIT_FAILURE );
                                  }
                                  break;

                         case 5 : errno = 0;
                                  ret = save_data_base( &list );
                                  save_errno = errno;

                                  if ( ret != 0 && save_errno != 0 )
                                  {
                                       printf( "\
Something went wrong while trying to save the data base to the file.\n" );
                                       printf( "Error: %s.\n\n",
                                               strerror( save_errno ) );
                                       exit( EXIT_FAILURE );
                                  }
                                  break;

                         case 6 : exit_loop = 1;
                                  break;

                    }    /* switch( choice ) */

               }    /* if ( choice < 1 || choice > 6 ) */

          }    /* if ( ret != 1 ) */

     }    while( exit_loop == 0 );

     /* Free the list. */

     count = 0;
     while( list[ count ] != NULL )
     {
          list[ count ]->device = 0;
          list[ count ]->name[ 0 ] = 0;
          free( list[ count ] );
          list[ count ] = NULL;
          count++;
     }
     free( list );
     list = NULL;

     return 0;
}

/* EOF userdb.c */
