/**
 * @file dlist.h
 * @author Philipp Badenhoop
 * @date 4 Jun 2016
 * @brief A generic doubly linked list.
 */

#ifndef DLIST
#define DLIST


#include "basic.h"
#include "dlistnode.h"


/**
 * @brief A pointer to a function which destroys the generic data.
 */
typedef void (*DestroyFunction)( Data );


/**
  * @brief The data structure of a generic doubly linked list.
  *
  * The DList is made up of at least two nodes:
  * The begin and the end node of the list.
  * These nodes will be created when the list is created and will never be
  * removed from the list until the list is destroyed.
  * Thus, the begin and end nodes will have a NULL pointer to their data. They're empty objects.
  * Having discrete nodes for the begin and end of the list will result in fewer checks
  * compared to lists without them.
  *
  * Some defintions:
  * @code
  * An 'empty' list:
  *     A list which has no nodes other than the discrete begin and end nodes.
  *
  * The 'first' node of the list:
  *     IF the list is not empty    -> The next node of the discrete begin node
  *     ELSE                        -> A 'first' node does not exist / is undefined.
  *
  * The 'last' node of the list:
  *     IF the list is not empty    -> The previous node of the discrete end node
  *     ELSE                        -> A 'last' node does not exist / is undefined.
  * @endcode
  */
typedef struct
{
    int             dataSize;               /**< The size of the data stored in each node. */
    int             listSize;               /**< The number of nodes in the list excluding the begin and end nodes. */
    DListNode *     begin;                  /**< The discrete begin node of the list. */
    DListNode *     end;                    /**< The discrete end node of the list. */
    DestroyFunction destroyFunction;        /**< A function which destroys the data of a node. */
} DList;


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Allocates and initializes a new list.
 * It also creates the discrete begin and end node.
 * @param elementSize The size of the data stored in each node.
 * @param DestroyFunction A function which destroys the data of a node.
 * @return A pointer to the created list.
 */
DList *dList_new( int elementSize, DestroyFunction destroyFunction );


/**
 * @brief Simply frees the pointer to the list.
 * @param list
 */
void dList_destroy( DList *list );


/**
 * @brief Completely destroys and frees the list, including its nodes and their data.
 * @param list
 */
void dList_destroyAll( DList *list );


/**
 * @param list
 * @return The number of nodes in the list excluding the discrete begin and end nodes.
 */
int dList_getSize( DList *list );


/**
 * @param list
 * @return true, if there're no nodes in the list except the discrete begin and end nodes.
 */
bool dList_isEmpty( DList *list );


/**
 * @brief Inserts a new node whichs stores given data after the last node of the list.
 * @param list
 * @param data The data which the new node will be storing.
 */
void dList_append( DList *list, Data data );


/**
 * @param list
 * @param i
 * @return The data of the i-th node of the list (start counting at the first node).
 */
Data dList_get( DList *list, int i );


#ifdef __cplusplus
}
#endif


#endif // DLIST

