/**
 * @file dlistnode.h
 * @author Philipp Badenhoop
 * @date 4 Jun 2016
 * @brief A node in a generic doubly linked list.
 */

#ifndef DLISTNODE
#define DLISTNODE


#include "basic.h"


typedef struct DListNode DListNode;

/**
 * @brief The atomic data structure of a generic doubly linked list. (DList)
 */
struct DListNode
{
    Data        data;   /**< The generic data. */
    DListNode * prev;   /**< The previous node. */
    DListNode * next;   /**< The next node. */
};


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Allocates and initializes a node.
 * @param data The generic data the node stores.
 * @param prev The previous node.
 * @param next The next node.
 * @return A pointer to the created node.
 */
DListNode *dListNode_new( Data data, DListNode *prev, DListNode *next );


/**
 * @brief Frees the memory of the node.
 * @param node The node to be destroyed.
 */
void dListNode_destroy( DListNode *node );


#ifdef __cplusplus
}
#endif


#endif // DLISTNODE

