/*

     demo.c
     This is a demo program for string_to_array.c.
     Written by Matthew Campbell.

*/

/* Include what we need: */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Prototype the library function: */

char **string_to_array( char *str );

int main( void )
{
     char **ret;
     char str[] = "\
one two three forty\\ five \"one hundred three\" 'fifty seven' \
'Anything \"here\"' \"Whatever 'else' here\" \\\"Something";
     int count, save_errno;

     printf( "Calling string_to_array() with: %s\n\n", str );

     errno = 0;
     ret = string_to_array( str );
     save_errno = errno;

     if ( ret == NULL )
     {
          printf( "Something went wrong while processing the string.\n" );

          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }

          printf( "\n" );
          return 1;
     }

     /* Print everything. */

     count = 0;
     printf( "ret:\n\n" );

     while( ret[ count ] != NULL )
     {
          printf( "ret[ %d ]: %s\n", count, ret[ count ] );
          count++;
     }

     printf( "ret[ %d ]: NULL\n", count );

     /* Free everything. */

     count = 0;

     while( ret[ count ] != NULL )
     {
          ret[ count ][ 0 ] = 0;
          free( ret[ count ] );
          ret[ count ] = NULL;
          count++;
     }

     free( ret );
     ret = NULL;

     return 0;
}

/* EOF demo.c */

