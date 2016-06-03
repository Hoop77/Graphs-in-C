#include "comparator.h"


Comparator *comparator_new( Data data, CompareFunction compareFunction )
{
    Comparator *comparator = (Comparator *) malloc( sizeof( Comparator ));
    assert( comparator != NULL );

    comparator->data = data;
    comparator->compareFunction = compareFunction;

    return comparator;
}


void comparator_destroy( Comparator *comparator )
{
    free( comparator );
    return;
}


bool comparator_compare( Comparator *comparator, Data compareData )
{
    return comparator->compareFunction( comparator->data, compareData );
}
