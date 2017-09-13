#include "HalfEdge.h"
#include "Mesh.h"

namespace DDG
{
   double HalfEdge :: length( void ) const
   // returns the edge length
   {
      return ( vertex->position - flip->vertex->position ).norm();
   }
}

