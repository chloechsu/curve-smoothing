#ifndef DDG_DIFFERENTIALOPERATORS_H
#define DDG_DIFFERENTIALOPERATORS_H

#include "SparseMatrix.h"

namespace DDG
{
   template<class T>
   class DifferentialOperators
   {
      public:
         static SparseMatrix<T> Laplacian1D( const Mesh& mesh );
         // returns weak 1D piecewise linear Laplace-Beltrami matrix
         // corresponding to the first polygon in "mesh" viewed as
         // a curve in the xy-plane

         static SparseMatrix<T> vertexMass1D( const Mesh& mesh );
         // returns a diagonal mass matrix containing the dual lengths
         // associated with each vertex of the first polygon in "mesh"
         // viewed as a curve in the xy-plane
   };
}

#include "DifferentialOperators.inl"

#endif
