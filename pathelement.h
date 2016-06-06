#ifndef PATHELEMENT
#define PATHELEMENT


#include "basic.h"
#include "path.h"


typedef struct
{
    int vertexNum;
} PathElement;


#ifdef __cplusplus
extern "C" {
#endif


PathElement *pathElement_new( int );
void         pathElement_destroy( PathElement * );
void         pathElement_setVertexNum( PathElement *, int );
int          pathElement_getVertexNum( PathElement * );
bool         pathElement_equals( PathElement *, PathElement * );


#ifdef __cplusplus
}
#endif


#endif // PATHELEMENT

