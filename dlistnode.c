#include "dlistnode.h"


DListNode *dListNode_new( Data data, DListNode *prev, DListNode *next )
{
    DListNode *node = (DListNode *) malloc( sizeof( DListNode ));
    assert( node != NULL );

    node->data = data;
    node->prev = prev;
    node->next = next;

    return node;
}


void dListNode_destroy( DListNode *node )
{
    free( node );
    return;
}
