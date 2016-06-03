#include "pathelement.h"


PathElement *pathElement_new( int vertexNum )
{
    PathElement *element = (PathElement *) malloc( sizeof( PathElement ));
    assert( element != NULL );

    element->vertexNum = vertexNum;

    return element;
}


void pathElement_destroy( PathElement *element )
{
    free( element );
    return;
}


int pathElement_getVertexNum( PathElement *element )
{
    return element->vertexNum;
}


bool pathElement_equals( PathElement *element1, PathElement *element2 )
{
    return element1->vertexNum == element2->vertexNum;
}
