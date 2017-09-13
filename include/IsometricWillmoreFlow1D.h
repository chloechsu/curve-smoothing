#ifndef DDG_ISOMETRICWILLMOREFLOW1D_H
#define DDG_ISOMETRICWILLMOREFLOW1D_H

#include "Mesh.h"
#include "SparseMatrix.h"
#include "DenseMatrix.h"

namespace DDG
{
   class IsometricWillmoreFlow1D
   {
      public:
         IsometricWillmoreFlow1D( Mesh& mesh );

         void integrate( double dt );

      protected:
         void getCurvature( void );
         void buildMassMatrix( void );
         void buildConstraints( void );
         void orthogonalizeConstraints( void );
         void computeFlowDirection( void );
         void enforceConstraints( void );
         void integrateFlow( double dt );
         void recoverTangents( void );
         void recoverPositions( void );

         Mesh& mesh;
         SparseMatrix<Real> B; // diagonal mass matrix on primal 0-forms
         vector< DenseMatrix<Real> > c; // constraint vectors
         DenseMatrix<Real> kappaDot; // flow direction
   };
}

#endif
