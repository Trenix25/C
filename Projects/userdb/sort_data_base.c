/*

     sort_data_base.c
     This function is called by qsort(3) to sort the data base entries.
     Written by Matthew Campbell.

*/

#include "userdb.h"

int sort_data_base( const void *item1, const void *item2 )
{
     user_auth *ptr1, *ptr2, **tmp;

     /* Make sure we have parameters that we can use. */

     if ( item1 == NULL || item2 == NULL )
     {
          errno = EFAULT;
          return 0;
     }

     /* Type cast the parameters to make them useful. */

     tmp = ( user_auth ** )item1;
     ptr1 = ( user_auth * )( *tmp );
     tmp = ( user_auth ** )item2;
     ptr2 = ( user_auth * )( *tmp );
     tmp = NULL;

     /* Compare the device numbers. */

     if ( ptr1->device < ptr2->device )
     {
          return ( -1 );
     }
     else if ( ptr1->device == ptr2->device )  /* This should never happen. */
     {
          return 0;
     }
     else  /* ptr1->device > ptr2->device */
     {
          return 1;
     }
}

/* EOF sort_data_base.c */
