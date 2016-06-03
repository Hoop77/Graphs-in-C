#include "path.h"


Path *path_new( void )
{
    Path *path = (Path *) malloc( sizeof( Path ));
    assert( path != NULL );

    path->elements = dList_new( sizeof( PathElement ), (DestroyFunction) pathElement_destroy );

    return path;
}


void path_destroy( Path *path )
{
    free( path );
    return;
}


void path_destroyAll( Path *path )
{
    dList_destroyAll( path->elements );
    free( path );
    return;
}


DList *path_getElements( Path *path )
{
    return path->elements;
}


void path_append( Path *path, int vertexNum )
{
    PathElement *newElement = pathElement_new( vertexNum );
    dList_append( path->elements, newElement );
    return;
}
