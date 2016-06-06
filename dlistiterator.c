#include "dlistiterator.h"


DListIterator *dListIterator_new( DList *list )
{
    DListIterator *iterator;

    // Create the iterator.
    iterator = (DListIterator *) malloc( sizeof( DListIterator ));
    assert( iterator != NULL );

    // Initialize it.
    iterator->list = list;
    iterator->current = NULL;

    return iterator;
}


void dListIterator_destroy( DListIterator *iterator )
{
    free( iterator );
    return;
}


DList *dListIterator_getList( DListIterator *iterator )
{
    return iterator->list;
}


void dListIterator_increment( DListIterator *iterator )
{
    assert( iterator->current );

    // It should be fast so no checks here.
    iterator->current = iterator->current->next;

    return;
}


void dListIterator_decrement( DListIterator *iterator )
{
    assert( iterator->current );

    // It should be fast so no checks here.
    iterator->current = iterator->current->prev;

    return;
}


bool dListIterator_isAtEnd( DListIterator *iterator )
{
    assert( iterator->current );

    return iterator->current == iterator->list->end;
}


bool dListIterator_isAtBegin( DListIterator *iterator )
{
    assert( iterator->current );

    return iterator->current == iterator->list->begin;
}


void dListIterator_insertAfter( DListIterator *iterator, Data data )
{
    DListNode *current = iterator->current;
    DListNode *newNode = dListNode_new( data, current, current->next );

    // Insert the new node after the current element.
    current->next->prev = newNode;
    current->next = newNode;

    // We now must increment the size of the list as well.
    DList *list = iterator->list;
    list->listSize++;

    return;
}


void dListIterator_remove( DListIterator *iterator )
{
    assert( iterator->current );
    assert( iterator->current != iterator->list->begin );
    assert( iterator->current != iterator->list->end );

    DList *list = iterator->list;
    DListNode *current = iterator->current;

    // Connect the previous and next node of the current node.
    current->prev->next = current->next;
    current->next->prev = current->prev;

    // When we remove a node in the list the current node must be updated.
    // Normally, we want to set it to the previous node of the node we've just removed.
    // However there're some cases which have to be considered:
    // The current node is the last data-node in the list. We have to set it to NULL.
    if( current->prev == list->begin && current->next == list->end )
        iterator->current = NULL;
    // The current node is at the begin of the list but there're still other data-nodes inside the list.
    // We have to set it to the next node.
    else if( current->prev == list->begin )
        iterator->current = current->next;
    // Normal case. Just set it to the previous node.
    else
        iterator->current = current->prev;

    // Destroy ONLY the node itself and NOT the data.
    dListNode_destroy( current );

    list->listSize--;

    return;
}


void dListIterator_removeAndDestroy( DListIterator *iterator )
{
    assert( iterator->current );
    assert( iterator->list->destroyFunction );

    iterator->list->destroyFunction( iterator->current->data );

    dListIterator_remove( iterator );

    return;
}


Data dListIterator_get(DListIterator *iterator)
{
    assert( iterator->current );

    return iterator->current->data;
}


Data dListIterator_search( DListIterator *iterator, Comparator *comparator )
{
    assert( iterator->current );

    Data result = NULL;

    // Start searching at the current node.
    while( !dListIterator_isAtEnd( iterator ))
    {
        Data compareData = dListIterator_get( iterator );

        // If the condition is true we've found the element.
        if( comparator_compare( comparator, compareData ))
        {
            result = compareData;
            break;
        }

        dListIterator_increment( iterator );
    }

    return result;
}


void dListIterator_toFirst( DListIterator *iterator )
{
    assert( !dList_isEmpty( iterator->list ));

    iterator->current = iterator->list->begin->next;

    return;
}


void dListIterator_toLast( DListIterator *iterator )
{
    assert( !dList_isEmpty( iterator->list ));

    iterator->current = iterator->list->end->prev;

    return;
}


void dListIterator_merge( DListIterator *iterator, DList *mergeList )
{
    assert( iterator->current );
    assert( iterator->list->destroyFunction );

    DList *mainList = iterator->list;
    DListNode *current = iterator->current;

    // Nothing to do if the merge list is empty.
    if( dList_isEmpty( mergeList ))
        return;

    // Connect the previous node with the first real element of the merge list.
    current->prev->next = mergeList->begin->next;
    mergeList->begin->next->prev = current->prev;

    // Connect the next node with the last real element of the merge list.
    current->next->prev = mergeList->end->prev;
    mergeList->end->prev->next = current->next;

    // Destroy the current node of the main list.
    // The first node with data in the merge list should act as its replacement.
    mainList->destroyFunction( current->data );
    dListNode_destroy( current );

    // The iterator's current node is the first data-node of the merge list now.
    iterator->current = mergeList->begin->next;

    // Update the size of the main list.
    mainList->listSize += mergeList->listSize - 1;

    // Since the merge list's nodes are now connected to the main list,
    // it is unusable, so we can destroy the parts that are not needed anymore.
    // However these destroy functions WON'T destroy the actual data because
    // that's still used by the main list.
    dListNode_destroy( mergeList->begin );
    dListNode_destroy( mergeList->end );
    dList_destroy( mergeList );

    return;
}
