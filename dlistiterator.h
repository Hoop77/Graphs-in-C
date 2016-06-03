#ifndef DLISTITERATOR
#define DLISTITERATOR


#include "basic.h"
#include "dlist.h"
#include "comparator.h"


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// DEFINITIONS
// -----------

#define dList_foreach( iterator )   for( dListIterator_toFirst( iterator ); \
                                         !dListIterator_isAtEnd( iterator ); \
                                         dListIterator_increment( iterator ))

#define dList_foreachDown( iterator )   for( dListIterator_toLast( iterator ); \
                                             !dListIterator_isAtBegin( iterator ); \
                                             dListIterator_decrement( iterator ))

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// DATA STRUCTURES
// ---------------

typedef struct
{
    DList *     list;
    DListNode * current;
} DListIterator;

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// FUNCTION DECLARATION
// --------------------

EXTERN_C_BEGIN

DListIterator *dListIterator_new( DList * );
void           dListIterator_destroy(DListIterator *);
void           dListIterator_increment( DListIterator * );
void           dListIterator_decrement( DListIterator * );
bool           dListIterator_isAtEnd( DListIterator * );
bool           dListIterator_isAtBegin( DListIterator * );
void           dListIterator_insertAfter( DListIterator *, Data );
void           dListIterator_remove(DListIterator * );
void           dListIterator_removeAndDestroy( DListIterator * );
Data           dListIterator_get( DListIterator * );
Data           dListIterator_search( DListIterator *, Comparator * );
void           dListIterator_toFirst( DListIterator * );
void           dListIterator_toLast( DListIterator * );
void           dListIterator_merge( DListIterator *, DList * );

EXTERN_C_END

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif // DLISTITERATOR

