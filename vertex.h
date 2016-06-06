/**
 * @file vertex.h
 * @author Philipp Badenhoop
 * @date 6 Jun 2016
 * @brief A vertex in a graph.
 */


#ifndef VERTEX
#define VERTEX


#include "basic.h"
#include "dlist.h"
#include "edge.h"


/**
 * @brief A vertex in a graph is just a container of a doubly linked list of edges.
 */
typedef struct
{
    DList *edges;       /**< The list of edges. */
} Vertex;


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Allocates and initializes a new vertex.
 * @return The pointer to the new vertex.
 */
Vertex *vertex_new( void );


/**
 * @brief Simply frees the pointer to the vertex.
 * @param vertex
 *
 * @attention This does not destroy the list of edges!
 */
void vertex_destroy( Vertex *vertex );


/**
 * @brief Frees the pointer to the vertex and destroys the complete list of edges.
 * @param vertex
 */
void vertex_destroyAll( Vertex *vertex );


/**
 * @param vertex
 * @return Get the list of edges.
 */
DList *vertex_getEdges( Vertex *vertex );


/**
 * @param vertex
 * @return Get the number of edges which go out of the vertex.
 */
int vertex_getDegree( Vertex *vertex );


/**
 * @param vertex
 * @return true, if the degree of this vertex is zero.
 */
bool vertex_hasEdges( Vertex *vertex );


#ifdef __cplusplus
}
#endif


#endif // VERTEX

