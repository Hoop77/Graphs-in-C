#include "dlist.h"


DList *dList_new( int elementSize, DestroyFunction destroyFunction )
{
    DListNode *begin;
    DListNode *end;

    // Create the list.
    DList *list = (DList *) malloc( sizeof( DList ));
    assert( list != NULL );

    // Initialize the list.
    list->elementSize = elementSize;
    list->listSize = 0;
    list->destroyFunction = destroyFunction;

    // We create a default begin and end node whose data is NULL.
    begin = dListNode_new( NULL, NULL, NULL );
    end = dListNode_new( NULL, begin, NULL );
    begin->next = end;

    // Add them to the list BUT: we do NOT count them to the list size!
    list->begin = begin;
    list->end = end;

    return list;
}


void dList_destroy( DList *list )
{
    free( list );
    return;
}


void dList_destroyAll( DList *list )
{
    // Iteratively destroy the nodes.
    DListNode *current = list->begin->next;
    while( current != list->end )
    {
        // Execute the custom free function to the data of the node.
        if( list->destroyFunction )
            list->destroyFunction( current->data );

        current = current->next;
        dListNode_destroy( current->prev );
    }

    free( list->begin );
    free( list->end );

    free( list );

    return;
}


int dList_getSize( DList *list )
{
    return list->listSize;
}


bool dList_isEmpty( DList *list )
{
    return ( list->listSize == 0 );
}


void dList_append( DList *list, Data data )
{
    // We will insert the data between the list's end node and its previous node.
    DListNode *end = list->end;
    DListNode *endPrev = end->prev;
    DListNode *node = dListNode_new( data, endPrev, end );
    endPrev->next = node;
    end->prev = node;

    // Increment the list size.
    list->listSize++;

    return;
}


Data dList_get( DList *list, int i )
{
    DListNode *node = list->begin->next;
    // Due to performance, we don't mind walking off the end element of the list.
    while( i > 0 )
    {
        node = node->next;
        i--;
    }

    return node->data;
}
