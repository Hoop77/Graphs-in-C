#ifndef GRAPH
#define GRAPH


#include "basic.h"
#include "vertex.h"


typedef struct
{
    Vertex **   vertices;
    int         vertexCount;
} Graph;


#ifdef __cplusplus
extern "C" {
#endif


Graph * graph_new( int );
void    graph_destroy( Graph * );
void    graph_destroyAll( Graph * );
void    graph_setVertex( Graph *, Vertex *, int );
Vertex *graph_getVertex( Graph *, int );
int     graph_getVertexCount( Graph * );
void    graph_addVertex( Graph * );
void    graph_addEdgePair( Graph *, int, int );
bool    graph_removeEdgePair( Graph *, int, int );
bool    graph_hasEdges( Graph * );


#ifdef __cplusplus
}
#endif


#endif // GRAPH

