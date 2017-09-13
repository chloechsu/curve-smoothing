#include "WillmoreFlow1D.h"
#include "Utility.h"
#include <cmath>
#include <iostream>

using namespace std;

namespace DDG
{
   WillmoreFlow1D :: WillmoreFlow1D( Mesh& _mesh )
      : mesh( _mesh )
   {}

   void WillmoreFlow1D :: integrate( double dt )
   {
      // TODO Compute the gradient (just call the appropriate method!)

      // TODO Integrate the position of the curve, stored in Vertex::position.
   }

   void WillmoreFlow1D :: computeGradient( void )
   {
      // TODO Initialize the gradient at each vertex to zero --
      // the value of the gradient should be stored in Vertex::energyGradient

      // TODO Evaluate the gradient at each vertex.
   }
}
