#ifndef VERTEX
#define VERTEX


#include "basic.h"
#include "dlist.h"
#include "edge.h"


typedef struct
{
    DList *edges;
} Vertex;


#ifdef __cplusplus
extern "C" {
#endif


Vertex *vertex_new( void );
void    vertex_destroy( Vertex * );
void    vertex_destroyAll( Vertex * );
DList * vertex_getEdges( Vertex * );
int     vertex_getDegree( Vertex * );


#ifdef __cplusplus
}
#endif


#endif // VERTEX

