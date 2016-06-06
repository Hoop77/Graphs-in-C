/**
 * @file dlistiterator.h
 * @author Philipp Badenhoop
 * @date 6 Jun 2016
 * @brief A bidirectional iterator for a doubly linked list.
 */


#ifndef DLISTITERATOR
#define DLISTITERATOR


#include "basic.h"
#include "dlist.h"
#include "comparator.h"


/**
 * @brief A simple bidirectional iterator.
 *
 * @attention You <b>must</b> prevent the iterator from operating on an empty list!
 * The iterator's current node is initialized with NULL. This means when you start to use it,
 * please set the iterator to a valid node in the list by using 'dListIterator_toFirst()' or
 * 'dListIterator_toLast()' for example!
 */
typedef DListNode DListIterator;


/**
 * @brief The iterator is set to its next node.
 * @param iterator Pointer to the iterator.
 *
 * @attention Do not increment if the iterator is at the discrete end node!
 */
#define dListIterator_increment( iterator )     iterator = iterator->next


/**
 * @brief The iterator is set to its previous node.
 * @param iterator Pointer to the iterator.
 *
 * @attention Do not decrement if the iterator is at the discrete begin node!
 */
#define dListIterator_decrement( iterator )     iterator = iterator->prev


/**
 * @brief Sets the iterator to the <b>first</b> node of the list.
 * @param iterator Pointer to the iterator.
 * @param list Pointer to the list.
 *
 * @attention It is not meant that the iterator will be set to the discrete begin node!
 * Please check out the definitions of DList for more information.
 */
#define dListIterator_toFirst( iterator, list )     iterator = list->begin->next


/**
 * @brief Sets the iterator to the <b>last</b> node of the lsit.
 * @param iterator Pointer to the iterator.
 * @param list Pointer to the list.
 *
 * @attention  It is not meant that the iterator will be set to the discrete end node!
 * Please check out the definitions of DList for more information.
 */
#define dListIterator_toLast( iterator, list )      iterator = list->end->prev


/**
 * @brief With the DListIterator you can simply mimic a foreach loop.
 * It iterates through each node in the list starting with the first node and ending on the discrete end node.
 * @param iterator Pointer to the iterator
 * @param list Pointer to the list.
 *
 * @attention When the loop is over, the iterator is on the discrete end node. This means:
 * If you want to get the data of the current node afterwards, you <b>must</b> set the iterator to a valid node first
 * (not the discrete begin or end nodes)!
 */
#define dList_foreach( iterator, list )     \
    for( iterator = list->begin->next;      \
         iterator != list->end;             \
         iterator = iterator->next )


/**
 * @brief With the DListIterator you can simply mimic a foreach loop.
 * It iterates through each node in the list starting with the last node and ending on the discrete begin node.
 * @param iterator Pointer to the iterator
 * @param list Pointer to the list.
 *
 * @attention When the loop is over, the iterator is on the discrete begin node. This means:
 * If you want to get the data of the current node afterwards, you <b>must</b> set the iterator to a valid node first
 * (not the discrete begin or end nodes)!
 */
#define dList_foreachDown( iterator, list )     \
    for( iterator = list->end->prev;            \
         iterator != list->begin;               \
         iterator = iterator->prev )


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @param list
 * @return An iterator at the first node of the list.
 */
DListIterator *dListIterator_getIteratorFromList( DList *list );


/**
 * @param iterator
 * @return The data of the node of the iterator.
 */
Data dListIterator_get( DListIterator *iterator );

/**
 * @param iterator
 * @param list
 * @return true, if the iterator is on the <b>discrete end node</b> of the list.
 *
 * @attention It is not meant that the iterator is the <b>last node</b> of the list.
 * Please check out the definitions of DList for more information.
 */
bool dListIterator_isAtEnd( DListIterator *iterator, DList *list );


/**
 * @param iterator
 * @param list
 * @return true, if the iterator is the <b>discrete begin node</b> of the list.
 *
 * @attention It is not meant that the iterator is the <b>first node</b> of the list.
 * Please check out the definitions of DList for more information.
 */
bool dListIterator_isAtBegin( DListIterator *iterator, DList *list );


/**
 * @brief Inserts a new node with given data between the iterator and its next node.
 * @param iterator
 * @param data
 */
void dListIterator_insertAfter( DListIterator *iterator, DList *list, Data data );


/**
 * @brief Destroys the data of the iterator using the destroy function of the list
 * and the iterator's node itself.
 * @param iterator
 * @param list
 *
 * @attention You cannot use the iterator anymore after calling this function!
 */
void dListIterator_destroy( DListIterator *iterator, DList *list );


/**
 * @brief Uses a comparator object to search a node in the list.
 * The search is incremental and starts at the current node.
 * @param iteratorRef
 * @param list
 * @param comparator
 * @return The data of the node to search for if it finds it, else NULL.
 *
 * @attention If it does not find the data to search for, the iterator
 * will be on the discrete end node. This means:
 * If you want to get the data of the current node afterwards, you <b>must</b> set the iterator to a valid node first
 * (not the discrete begin or end nodes)!
 */
Data dListIterator_search( DListIterator **iteratorRef, DList *list, Comparator *comparator );


/**
 * @brief Inserts the nodes of the specified merge list (excluding the discrete begin and end nodes)
 * after the current node.
 * @param iteratorRef
 * @param mainList
 * @param mergeList
 *
 * @attention After doing this, it should be obvious that the merge list cannot be used anymore.
 * Because of that, the merge list is going to be destroyed after calling this function! So do not access it anymore!
 */
void dListIterator_merge( DListIterator **iteratorRef, DList *mainList, DList *mergeList );


#ifdef __cplusplus
}
#endif


#endif // DLISTITERATOR

