#ifndef DDG_WILLMOREFLOW1D_H
#define DDG_WILLMOREFLOW1D_H

#include "Mesh.h"

namespace DDG
{
   class WillmoreFlow1D
   {
      public:
         WillmoreFlow1D( Mesh& mesh );

         void integrate( double dt );

      protected:
         void computeGradient( void );
         double energy( void ) const;
         void checkGradient( void );

         Mesh& mesh;
   };
}

#endif
