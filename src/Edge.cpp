#include "Edge.h"
#include "Mesh.h"

namespace DDG
{
   double Edge :: length( void ) const
   // returns the edge length
   {
      return (he->vertex->position - he->flip->vertex->position).norm();
   }
}

