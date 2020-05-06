/*

     string_to_array.c

     Converts a string to a two dimensional character array, an array of
     pointers to character strings.  It allows quoted substrings along
     with nested substrings, as long as the ends of which do not overlap.

     The purpose of this function is to duplicate the action of a command
     shell when a command is typed on the command line, except for the
     substitution of environment variables.

     Returns a two dimensional character array, an array of character
     pointers each pointing to a null terminated character string.  The
     last element in the first dimension of the array will be a null
     pointer.

     Returns an array with a single element containing a null pointer if
     str is an empty string.  ( str[ 0 ] == 0 )

     Returns a null pointer if an error occurs.

     Sets errno to:

     EFAULT if str holds a null pointer.

     EINVAL if str contains an unterminated quoted string.

     EINVAL if str contains invalid data such as overlapping quoted
            substrings.

     EINVAL if str ends with a back slash character.

     Also indirectly sets errno if malloc(3) is unable to allocate enough
     memory to create the array or its associated character strings.

     Written by Matthew Campbell.

*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

char **string_to_array( char *str )
{
     char **ret;
     int count, exit_loop, in_word, index, length;
     int pos, pos2, save_errno, slash;
     int stack[ 2 ] = { 0, 0 }, start, total;

     /* Make sure we have a valid character string to use. */

     if ( str == NULL )
     {
          errno = EFAULT;
          return NULL;
     }

     /* Did we receive an empty string? */

     if ( str[ 0 ] == 0 )
     {
          ret = malloc( sizeof( char * ) );

          if ( ret == NULL )
          {
               return NULL;
          }

          ret[ 0 ] = NULL;
          return ret;
     }

     /* Find out how many words the string has while detecting substrings. */

     in_word = pos = slash = total = 0;

     while( str[ pos ] != 0 )
     {
          if ( str[ pos ] == '\'' && slash == 0 )
          {
               if ( stack[ 0 ] == 0 )  /* The stack is empty. */
               {
                    stack[ 0 ] = 1;  /* Start a single quoted substring. */
               }
               else if ( stack[ 0 ] == 1 && stack[ 1 ] == 0 )
               {
                    stack[ 0 ] = 0;  /* End a single quoted substring. */
               }
               else if ( stack[ 0 ] == 1 && stack[ 1 ] == 2 )
               {
                    errno = EINVAL;  /* Overlapping substrings */
                    return NULL;
               }
               else if ( stack[ 0 ] == 2 && stack[ 1 ] == 0 )
               {
                    /* Start a single quoted nested substring. */

                    stack[ 1 ] = 1;
               }
               else if ( stack[ 0 ] == 2 && stack[ 1 ] == 1 )
               {
                    /* End a single quoted nested substring. */

                    stack[ 1 ] = 0;

               }    /* if ( stack[ 0 ] == 0 ) */
          }
          else if ( str[ pos ] == '\"' && slash == 0 )
          {
               if ( stack[ 0 ] == 0 )  /* The stack is empty. */
               {
                    stack[ 0 ] = 2;  /* Start a double quoted substring. */
               }
               else if ( stack[ 0 ] == 2 && stack[ 1 ] == 0 )
               {
                    stack[ 0 ] = 0;  /* End a double quoted substring. */
               }
               else if ( stack[ 0 ] == 2 && stack[ 1 ] == 1 )
               {
                    errno = EINVAL;  /* Overlapping substrings */
                    return NULL;
               }
               else if ( stack[ 0 ] == 1 && stack[ 1 ] == 0 )
               {
                    /* Start a double quoted nested substring. */

                    stack[ 1 ] = 2;
               }
               else if ( stack[ 0 ] == 1 && stack[ 1 ] == 2 )
               {
                    /* End a double quoted nested substring. */

                    stack[ 1 ] = 0;

               }    /* if ( stack[ 0 ] == 0 ) */
          }
          else if ( str[ pos ] == 32 )  /* Space */
          {
               if ( stack[ 0 ] == 0 && slash == 0 )
               {
                    if ( in_word == 1 )
                    {
                         in_word = 0;
                    }
               }
          }
          else
          {
               if ( str[ pos ] == '\\' )
               {
                    if ( str[ ( pos + 1 ) ] != '\'' &&
                         str[ ( pos + 1 ) ] != '\"' &&
                         str[ ( pos + 1 ) ] != 32 )
                    {
                         if ( in_word == 0 )
                         {
                              in_word = 1;

                              if ( slash == 0 )
                              {
                                   slash = 1;
                                   total++;
                              }
                              else
                              {
                                   slash = 0;
                              }

                         }    /* if ( in_word == 0 ) */
                    }
                    else
                    {
                         slash = 1;

                    }    /* if ( str[ pos ] not '\'' or '\"' ) */
               }
               else  /* str[ pos ] != '\\' */
               {
                    if ( in_word == 0 )
                    {
                         in_word = 1;
                         total++;
                    }

                    if ( slash == 1 )
                    {
                         slash = 0;
                    }

               }    /* if ( str[ pos ] == '\\' ) */

          }    /* if ( single or double quote, or a space... ) */

          pos++;  /* Move to the next character. */

     }    /* while( str[ pos ] != 0 ) */

     /* The last byte must not be a back slash. */

     if ( slash == 1 )
     {
          errno = EINVAL;
          return NULL;
     }

     /* Make sure we don't have an unterminated quoted string. */

     if ( stack[ 0 ] != 0 )
     {
          errno = EINVAL;
          return NULL;
     }

     /* Create the array of pointers. */

     ret = malloc( sizeof( char * ) * ( total + 1 ) );

     if ( ret == NULL )
     {
          return NULL;
     }

     /* The last element must be a null pointer. */

     ret[ total ] = NULL;

     /* Create each string and copy the data into it. */

     for( count = 0, pos = 0; count < total; count++ )
     {
          exit_loop = in_word = length = slash = 0;
          start = ( -1 );

          while( str[ pos ] != 0 && exit_loop == 0 )
          {
               if ( str[ pos ] == '\'' && slash == 0 )
               {
                    if ( stack[ 0 ] == 0 )  /* The stack is empty. */
                    {
                         stack[ 0 ] = 1;

                         if ( start == ( -1 ) )
                         {
                              start = pos;
                         }
                    }
                    else if ( stack[ 0 ] == 1 && stack[ 1 ] == 0 )
                    {
                         stack[ 0 ] = 0;
                    }
                    else if ( stack[ 0 ] == 2 && stack[ 1 ] == 0 )
                    {
                         length++;
                         stack[ 1 ] = 1;
                    }
                    else if ( stack[ 0 ] == 2 && stack[ 1 ] == 1 )
                    {
                         length++;
                         stack[ 1 ] = 0;

                    }    /* if ( stack[ 0 ] == 0 ) */
               }
               else if ( str[ pos ] == '\"' && slash == 0 )
               {
                    if ( stack[ 0 ] == 0 )  /* The stack is empty. */
                    {
                         stack[ 0 ] = 2;

                         if ( start == ( -1 ) )
                         {
                              start = pos;
                         }
                    }
                    else if ( stack[ 0 ] == 2 && stack[ 1 ] == 0 )
                    {
                         stack[ 0 ] = 0;
                    }
                    else if ( stack[ 0 ] == 1 && stack[ 1 ] == 0 )
                    {
                         length++;
                         stack[ 1 ] = 2;
                    }
                    else if ( stack[ 0 ] == 1 && stack[ 1 ] == 2 )
                    {
                         length++;
                         stack[ 1 ] = 0;

                    }    /* if ( stack[ 0 ] == 0 ) */
               }
               else if ( str[ pos ] == 32 )  /* Space */
               {
                    if ( stack[ 0 ] > 0 || slash != 0 )
                    {
                         /*

                              We are in a quoted string,
                              or escaped the space.

                         */

                         length++;

                         if ( stack[ 0 ] == 0 && slash == 1 )
                         {
                              slash = 0;
                         }
                    }
                    else
                    {
                         exit_loop = 1;

                    }    /* if stack[ 0 ] > 0 || slash != 0 ) */
               }
               else  /* Not an escaped single or double quote or a space */
               {
                    if ( str[ pos ] == '\\' )
                    {
                         if ( slash == 0 )
                         {
                              slash = 1;
                         }
                         else if ( slash == 1 )
                         {
                              length++;
                              slash = 0;

                              if ( in_word == 0 )
                              {
                                   in_word = 1;

                                   if ( start == ( -1 ) )
                                   {
                                        start = pos;
                                   }
                              }

                         }    /* if ( slash == 0 ) */
                    }
                    else  /* str[ pos ] != '\\' */
                    {
                         if ( in_word == 0 )
                         {
                              in_word = 1;

                              if ( slash == 1 )
                              {
                                   length += 2;  /* Count the slash too. */

                                   if ( start == ( -1 ) )
                                   {
                                        slash = 0;
                                        start = pos - 1;
                                   }
                              }
                              else
                              {
                                   length++;

                                   if ( start == ( -1 ) )
                                   {
                                        start = pos;
                                   }

                              }    /* if ( slash == 1 ) */
                         }
                         else  /* in_word == 1 */
                         {
                              length++;

                         }    /* if ( in_word == 0 ) */

                    }    /* if ( str[ pos ] == '\\' ) */

               }    /* if ( single or double quote, or a space... ) */

               pos++;  /* Move to the next character. */

          }    /* while( str[ pos ] != 0 && exit_loop == 0 ) */

          /* Copy the substring to a new slot in the array. */

          ret[ count ] = malloc( ( length + 1 ) );
          save_errno = errno;

          if ( ret[ count ] == NULL )
          {
               pos = 0;

               while( pos < count )
               {
                    ret[ pos ][ 0 ] = 0;
                    free( ret[ pos ] );
                    ret[ pos ] = NULL;
                    pos++;
               }

               free( ret );
               ret = NULL;

               errno = save_errno;
               return NULL;

          }    /* if ( ret[ count ] == NULL ) */

          exit_loop = index = slash = 0;
          pos2 = start;

          while( str[ pos2 ] != 0 && exit_loop == 0 )
          {
               if ( str[ pos2 ] == '\'' && slash == 0 )
               {
                    if ( stack[ 0 ] == 0 )  /* The stack is empty. */
                    {
                         stack[ 0 ] = 1;
                    }
                    else if ( stack[ 0 ] == 1 && stack[ 1 ] == 0 )
                    {
                         stack[ 0 ] = 0;
                    }
                    else if ( stack[ 0 ] == 2 && stack[ 1 ] == 0 )
                    {
                         ret[ count ][ index ] = str[ pos2 ];
                         index++;
                         stack[ 1 ] = 1;
                    }
                    else if ( stack[ 0 ] == 2 && stack[ 1 ] == 1 )
                    {
                         ret[ count ][ index ] = str[ pos2 ];
                         index++;
                         stack[ 1 ] = 0;
                    }
               }
               else if ( str[ pos2 ] == '\"' && slash == 0 )
               {
                    if ( stack[ 0 ] == 0 )  /* The stack is empty. */
                    {
                         stack[ 0 ] = 2;
                    }
                    else if ( stack[ 0 ] == 2 && stack[ 1 ] == 0 )
                    {
                         stack[ 0 ] = 0;
                    }
                    else if ( stack[ 0 ] == 1 && stack[ 1 ] == 0 )
                    {
                         ret[ count ][ index ] = str[ pos2 ];
                         index++;
                         stack[ 1 ] = 2;
                    }
                    else if ( stack[ 0 ] == 1 && stack[ 1 ] == 2 )
                    {
                         ret[ count ][ index ] = str[ pos2 ];
                         index++;
                         stack[ 1 ] = 0;
                    }
               }
               else if ( str[ pos2 ] == 32 )  /* Space */
               {
                    if ( stack[ 0 ] == 0 && slash == 0 )
                    {
                         exit_loop = 1;
                         ret[ count ][ index ] = 0;  /* Add the null byte. */
                    }
                    else
                    {
                         /*

                              We are in a quoted string,
                              or escaped the space.

                         */

                         ret[ count ][ index ] = str[ pos2 ];
                         index++;

                         if ( stack[ 0 ] == 0 && slash == 1 )
                         {
                              slash = 0;
                         }

                    }    /* if ( stack[ 0 ] == 0 && slash == 0 ) */
               }
               else  /* Not an escaped single or double quote or a space */
               {
                    if ( str[ pos2 ] == '\\' )
                    {
                         if ( slash == 0 )
                         {
                              slash = 1;
                         }
                         else  /* slash == 1 */
                         {
                              ret[ count ][ index ] = str[ pos2 ];
                              index++;
                              slash = 0;

                         }    /* if ( slash == 0 ) */
                    }
                    else  /* ptr[ index ] != '\\' */
                    {
                         if ( slash == 1 )
                         {
                              ret[ count ][ index ] = '\\';
                              index++;
                              slash = 0;
                         }

                         ret[ count ][ index ] = str[ pos2 ];
                         index++;

                    }    /* if ( ptr[ index ] == '\\' ) */

               }    /* if ( single or double quote, or a space... ) */

               pos2++;  /* Move to the next character. */

               if ( str[ pos2 ] == 0 )
               {
                    ret[ count ][ index ] = 0;  /* Add the null byte. */
               }

          }    /* while( ptr[ index ] != 0 && exit_loop == 0 ) */

     }    /* for( count = 0; count < total; count++ ) */

     return ret;
}

/* EOF string_to_array.c */
