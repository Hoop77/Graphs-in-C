#ifndef PATH
#define PATH


#include "basic.h"
#include "dlist.h"
#include "pathelement.h"


typedef struct
{
    DList *elements;
} Path;


#ifdef __cplusplus
extern "C" {
#endif


Path *path_new( void );
void  path_destroy( Path * );
void  path_destroyAll( Path * );
DList *path_getElements( Path * );
void  path_append( Path *, int );


#ifdef __cplusplus
}
#endif


#endif // PATH

