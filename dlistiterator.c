#include "dlistiterator.h"


DListIterator *dListIterator_getIteratorFromList( DList *list )
{
    assert( list->listSize > 0 );
    return list->begin->next;
}


Data dListIterator_get( DListIterator *iterator )
{
    return iterator->data;
}


bool dListIterator_isAtEnd( DListIterator *iterator, DList *list )
{
    return iterator == list->end;
}


bool dListIterator_isAtBegin( DListIterator *iterator, DList *list )
{
    return iterator == list->begin;
}


void dListIterator_insertAfter( DListIterator *iterator, DList *list, Data data )
{
    assert( iterator );

    DListNode *newNode = dListNode_new( data, iterator, iterator->next );

    // Insert the new node after the current element.
    iterator->next->prev = newNode;
    iterator->next = newNode;

    // We now must increment the size of the list as well.
    list->listSize++;

    return;
}


void dListIterator_destroy( DListIterator *iterator, DList *list )
{
    assert( iterator );
    assert( iterator != list->begin );
    assert( iterator != list->end );

    // Connect the previous and next nodes of the iterator.
    iterator->prev->next = iterator->next;
    iterator->next->prev = iterator->prev;

    // Destroy the iterator's node and its data.
    list->destroyFunction( iterator->data );
    dListNode_destroy( iterator );

    list->listSize--;

    return;
}


Data dListIterator_search( DListIterator **iteratorRef, DList *list, Comparator *comparator )
{
    DListIterator *iterator = *iteratorRef;
    assert( iterator );

    Data result = NULL;

    // Start searching at the current node.
    while( !dListIterator_isAtEnd( iterator, list ))
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

    *iteratorRef = iterator;

    return result;
}


void dListIterator_merge( DListIterator **iteratorRef, DList *mainList, DList *mergeList )
{
    DListIterator *iterator = *iteratorRef;
    assert( iterator );
    assert( mainList->destroyFunction );

    // Nothing to do if the merge list is empty.
    if( dList_isEmpty( mergeList ))
        return;

    // Connect the previous node with the first real element of the merge list.
    iterator->prev->next = mergeList->begin->next;
    mergeList->begin->next->prev = iterator->prev;

    // Connect the next node with the last real element of the merge list.
    iterator->next->prev = mergeList->end->prev;
    mergeList->end->prev->next = iterator->next;

    // Destroy the current node of the main list.
    // The first node with data in the merge list should act as its replacement.
    mainList->destroyFunction( iterator->data );
    dListNode_destroy( iterator );

    // The iterator's current node is the first data-node of the merge list now.
    *iteratorRef = mergeList->begin->next;

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
