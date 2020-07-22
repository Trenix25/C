/*

     userdb.h
     This is the header file for the userdb program.
     Written by Matthew Campbell.

*/

#ifndef _USERDB_H
#define _USERDB_H

#define _POSIX_C_SOURCE 200809L

/* Include what we need: */

#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

/* Define the file name used by the data base file: */

// #define DATA_BASE_FILE "/data/data/com.termux/files/adm/user.db"
#define DATA_BASE_FILE "/storage/emulated/0/C/userdb/user.db"

/* Define the user authorization structure: */

struct _user_auth
{
     int  device;         /* Device number                */
     char name[ 26 ];     /* User's name                  */
};

typedef struct _user_auth user_auth;

/* Function prototypes: */

int   add_data_base_entry( user_auth ***list );
void  enter_to_continue( void );
void  list_data_base_items( user_auth **list );
int   load_data_base( user_auth ***list );
void  print_menu( void );
int   read_stdin( char *buffer, const int length, const char *prompt,
                  const int reprompt );
int   remove_data_base_entry( user_auth ***list );
int   save_data_base( user_auth ***list );
int   sort_data_base( const void *item1, const void *item2 );

#endif  /* _USERDB_H */

/* EOF userdb.h */
