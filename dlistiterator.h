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
 * @brief With the DListIterator you can simply mimic a foreach loop.
 * It iterates through each node in the list starting with the first node and ending on the discrete end node.
 *
 * @attention When the loop is over, the iterator stands on the discrete end node. This means:
 * If you want to get the data of the current node afterwards, you <b>must</b> set the iterator to a valid node first
 * (not the discrete begin or end nodes)!
 */
#define dList_foreach( iterator )   for( dListIterator_toFirst( iterator ); \
                                         !dListIterator_isAtEnd( iterator ); \
                                         dListIterator_increment( iterator ))

/**
 * @brief With the DListIterator you can simply mimic a foreach loop.
 * It iterates through each node in the list starting with the last node and ending on the discrete begin node.
 *
 * @attention When the loop is over, the iterator stands on the discrete begin node. This means:
 * If you want to get the data of the current node afterwards, you <b>must</b> set the iterator to a valid node first
 * (not the discrete begin or end nodes)!
 */
#define dList_foreachDown( iterator )   for( dListIterator_toLast( iterator ); \
                                             !dListIterator_isAtBegin( iterator ); \
                                             dListIterator_decrement( iterator ))

/**
 * @brief A simple bidirectional iterator.
 *
 * @attention You <b>must</b> prevent the iterator from operating on an empty list!
 * The iterator's current node is initialized with NULL. This means when you start to use it,
 * please set the iterator to a valid node in the list by using 'dListIterator_toFirst()' or
 * 'dListIterator_toLast()' for example!
 */
typedef struct
{
    DList *     list;       /**< The list which the iterator iterates over. */
    DListNode * current;    /**< The node to interact with in the list. */
} DListIterator;


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Allocates and initializes a new iterator.
 * @param list
 * @return The pointer to the new iterator.
 *
 * @attention The iterator's current node is initialized with NULL. This means when you start to use it,
 * please set the iterator to a valid node in the list by using 'dListIterator_toFirst()' or
 * 'dListIterator_toLast()' for example!
 */
DListIterator *dListIterator_new( DList *list );


/**
 * @brief Simply frees the pointer to the iterator.
 * @param iterator
 */
void dListIterator_destroy( DListIterator *iterator );


/**
 * @param iterator
 * @return The list the iterator operates on.
 */
DList *dListIterator_getList( DListIterator *iterator );


/**
 * @brief The current node is set to its next node.
 * @param iterator
 *
 * @attention Do not increment if the current node is the discrete end node!
 */
void dListIterator_increment( DListIterator *iterator );


/**
 * @brief The current node is set to its previous node.
 * @param iterator
 *
 * @attention Do not decrement if the current node is the discrete begin node!
 */
void dListIterator_decrement( DListIterator *iterator );


/**
 * @param iterator
 * @return true, if the current node is the <b>discrete end node</b> of the list.
 *
 * @attention It is not meant that the current node is the <b>last node</b> of the list.
 * Please check out the definitions of DList for more information.
 */
bool dListIterator_isAtEnd( DListIterator *iterator );


/**
 * @param iterator
 * @return true, if the current node is the <b>discrete begin node</b> of the list.
 *
 * @attention It is not meant that the current node is the <b>first node</b> of the list.
 * Please check out the definitions of DList for more information.
 */
bool dListIterator_isAtBegin( DListIterator *iterator );


/**
 * @brief Inserts a new node with given data between the current node
 * and the next node of the current node.
 * @param iterator
 * @param data
 */
void dListIterator_insertAfter( DListIterator *iterator, Data data );


/**
 * @brief Destroys the current node and reconnects the nodes.
 * @param iterator
 *
 * @attention This does not destroy the data of the current node!
 */
void dListIterator_remove( DListIterator *iterator );


/**
 * @brief Destroy the current node, reconnects the nodes and
 * also destroys the data of the current node.
 * @param iterator
 */
void dListIterator_removeAndDestroy( DListIterator *iterator );


/**
 * @brief Destroys the current node, reconnects the nodes and
 * also uses the custom destroy function to destroy the data of the current node.
 * @param iterator
 * @param destroyFunction
 */
void dListIterator_removeAndDestroyWith( DListIterator *iterator, DestroyFunction destroyFunction );


/**
 * @param iterator
 * @return The data of the current node.
 */
Data dListIterator_get(DListIterator *iterator);


/**
 * @brief Uses a comparator object to search a node in the list.
 * The search is incremental and starts at the current node.
 * @param iterator
 * @param comparator
 * @return The data of the node to search for if it finds it, else NULL.
 *
 * @attention If it does not find the data to search for, the iterator's current
 * node will be the discrete end node. This means:
 * If you want to get the data of the current node afterwards, you <b>must</b> set the iterator to a valid node first
 * (not the discrete begin or end nodes)!
 */
Data dListIterator_search( DListIterator *iterator, Comparator *comparator );


/**
 * @brief Sets the current node to the <b>first</b> node of the list.
 * @param iterator
 *
 * @attention It is not meant that the current node will be set to the discrete begin node!
 * Please check out the definitions of DList for more information.
 */
void dListIterator_toFirst( DListIterator *iterator );


/**
 * @brief Sets the current node to the <b>last</b> node of the lsit.
 * @param iterator
 *
 * @attention  It is not meant that the current node will be set to the discrete end node!
 * Please check out the definitions of DList for more information.
 */
void dListIterator_toLast( DListIterator *iterator );


/**
 * @brief Inserts the nodes of the specified merge list (excluding the discrete begin and end nodes)
 * after the current node.
 * @param iterator
 * @param mergeList
 *
 * @attention After doing this, it should be obvious that the merge list cannot be used anymore.
 * Because of that, the merge list is going to be destroyed after calling this function! So do not access it anymore!
 */
void dListIterator_merge( DListIterator *iterator, DList *mergeList );


#ifdef __cplusplus
}
#endif


#endif // DLISTITERATOR

