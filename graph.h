/**
 * @file graph.h
 * @author Philipp Badenhoop
 * @date 6 Jun 2016
 * @brief A graph containing vertices which are connected through edges.
 */


#ifndef GRAPH
#define GRAPH


#include "basic.h"
#include "vertex.h"


/**
 * The graph contains a onedimensional array of vertices and each vertex stores a
 * doubly linked list of edges.
 */
typedef struct
{
    Vertex **   vertices;           /**< A onedimensional array of pointers to vertex-objects. */
    int         vertexCount;        /**< The number of vertices in the vertex array. */
} Graph;


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Allocates and initializes a new graph.
 * @param vertexCount
 * @return The pointer to the new graph.
 */
Graph *graph_new( int vertexCount );


/**
 * @brief Simply frees the poiner to the graph.
 * @param graph
 */
void graph_destroy( Graph *graph );


/**
 * @brief Frees the pointer to the graph and completely destroys the structure
 * of the graph including all vertices and their edges.
 * @param graph
 */
void graph_destroyAll( Graph *graph );


/**
 * @param graph
 * @param vertexNum
 * @return Get the vertex object at a specified vertex number.
 */
Vertex *graph_getVertex( Graph *graph, int vertexNum );


/**
 * @param graph
 * @return The number of vertices in the graph.
 */
int graph_getVertexCount( Graph *graph );


/**
 * @brief This adds a vertex to the graph.
 * @param graph
 *
 * @attention This function can be slow on a graph with a large number of vertices since it
 * creates a new array of the current size + 1 and copies all vertices from the old array into the new one.
 */
void graph_addVertex( Graph *graph );


/**
 * @brief Adds a bidirectional edge betweenn two vertices.
 * @param graph
 * @param vertexNum1
 * @param vertexNum2
 */
void graph_addEdgePair( Graph *graph, int vertexNum1, int vertexNum2 );


/**
 * @brief Removes a bidirectional edge between two vertices.
 * @param graph
 * @param vertexNum1
 * @param vertexNum2
 * @return true, if the given edge was found and removed, else false.
 */
bool graph_removeEdgePair( Graph *graph, int vertexNum1, int vertexNum2 );


/**
 * @param graph
 * @return true, if there is at least one vertex with an edge to another vertex.
 */
bool graph_hasEdges( Graph *graph );


#ifdef __cplusplus
}
#endif


#endif // GRAPH

