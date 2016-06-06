/**
 * @file pathelement.h
 * @author Philipp Badenhoop
 * @date 6 Jun 2016
 * @brief An element of a path of a graph.
 */

#ifndef PATHELEMENT
#define PATHELEMENT


#include "basic.h"
#include "path.h"


/**
 * A path element contains simply a vertex number.
 */
typedef struct
{
    int vertexNum;      /**< The vertex number this element represents. */
} PathElement;


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Allocates and initializes a new path element.
 * @param vertexNum
 * @return The pointer to the new path element.
 */
PathElement *pathElement_new( int vertexNum );


/**
 * @brief Simply frees the pointer to the element.
 * @param element
 */
void pathElement_destroy( PathElement *element );


/**
 * @param element
 * @return The vertex number this element represents.
 */
int pathElement_getVertexNum( PathElement *element );


/**
 * @brief Compares two path elements.
 * @param element1
 * @param element2
 * @return true, if the vertex number of the elements are equal.
 */
bool pathElement_equals( PathElement *element1, PathElement *element2 );


#ifdef __cplusplus
}
#endif


#endif // PATHELEMENT

