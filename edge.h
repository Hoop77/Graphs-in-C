/**
 * @file edge.h
 * @author Philipp Badenhoop
 * @date 6 Jun 2016
 * @brief The edge between vertices in a graph.
 */


#ifndef EDGE
#define EDGE


#include "basic.h"
#include "dlistiterator.h"


/**
 * @brief The edge between vertices in a graph.
 * Next to the vertex number of the vertex which it goes to,
 * it also stores an iterator to find its corresponding edge,
 * meaning the edge in a bidirectional graph.
 * This is important because when we remove this edge, we must also remove
 * the corresponding edge and we want that to happen fast! Using this approach, we don't have to search it
 * incrementally in a doubly linked list but rather destroy it immediately.
 */
typedef struct
{
    int             toVertexNum;                    /**< The vertex number of the vertex this edge goes to. */
    DListIterator * correspondingEdgeIterator;      /**< An iterator to  */
} Edge;


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Allocates and initializes a new edge.
 * @param toVertexNum
 * @return The pointer to the new edge.
 */
Edge *edge_new( int toVertexNum );


/**
 * @brief Simply frees the pointer to the edge.
 * @param edge
 */
void edge_destroy( Edge *edge );


/**
 * @param edge
 * @return The vertex number this edge goes to.
 */
int edge_getToVertexNum( Edge *edge );


/**
 * @brief Sets the iterator to the corresponding edge.
 * @param edge
 * @param correspondingEdgeIterator
 */
void edge_setCorrespondingEdgeIterator( Edge *edge, DListIterator *correspondingEdgeIterator );


/**
 * @param edge
 * @return Gets the iterator to teh corresponding edge.
 */
DListIterator *edge_getCorrespondingEdgeIterator( Edge *edge );


/**
 * @brief Compares two edges.
 * @param edge1
 * @param edge2
 * @return true, if the vertex number the edges go to are equal.
 */
bool edge_equals( Edge *edge1, Edge *edge2 );


#ifdef __cplusplus
}
#endif


#endif // EDGE

