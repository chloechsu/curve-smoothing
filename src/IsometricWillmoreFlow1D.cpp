#include "IsometricWillmoreFlow1D.h"
#include "Utility.h"
#include <cmath>
#include <iostream>

using namespace std;

namespace DDG
{
   IsometricWillmoreFlow1D :: IsometricWillmoreFlow1D( Mesh& _mesh )
      : mesh( _mesh )
   {}

   void IsometricWillmoreFlow1D :: integrate( double dt )
   {
      getCurvature();
      buildMassMatrix();
      buildConstraints();
      orthogonalizeConstraints();
      computeFlowDirection();
      enforceConstraints();
      integrateFlow( dt );
      recoverTangents();
      recoverPositions();
   }

   void IsometricWillmoreFlow1D :: getCurvature( void )
   {
      // TODO Evaluate Vertex::curvature() for each vertex and
      // store the result in Vertex::kappa.
   }

   void IsometricWillmoreFlow1D :: buildMassMatrix( void )
   {
      // TODO This class contains a member "B" that will hold the
      // mass matrix, but it has not yet been initialized -- initialize
      // it (with the appropriate size) here.

      // TODO Set the diagonal entries of B -- as in your last homework,
      // each vertex already has a unique index stored in Vertex::index.
   }

   void IsometricWillmoreFlow1D :: buildConstraints( void )
   {
      // TODO The constraints will be stored as an STL vector of
      // DenseMatrices -- see IsometricWillmoreFlow1D.h.  Since
      // you have three constraints, you will need to resize this
      // vector, and will need to allocate each of the DenseMatrices
      // it contains to the appropriate size.  (If you are unfamiliar
      // with STL vectors, Google for "stl vector".)

      // TODO At each vertex, set the values of the three constraint vectors.
   }

   void IsometricWillmoreFlow1D :: orthogonalizeConstraints( void )
   {
      // TODO Orthogonalize the vectors in IsometricWillmoreFlow1D::c
      // with respect to the L2 inner product on primal 0-forms, using
      // the Gram-Schmidt process.  Do not assume that there are only
      // three constraints!  Your code should work for a vector c of
      // any size.
   }

   void IsometricWillmoreFlow1D :: computeFlowDirection( void )
   {
      // TODO Allocate the vector IsometricWillmoreFlow1D::kappaDot, which
      // stores the direction of the gradient flow per vertex.

      // TODO Compute the direction of the gradient flow at each vertex (without
      // worrying about constraints for the moment) and store it in kappaDot.
      // Remember that each vertex already has a unique integer index.
   }
   
   void IsometricWillmoreFlow1D :: enforceConstraints( void )
   {
      // TODO Remove the constraint components from the flow direction
      // kappaDot, using the orthonormal basis stored in IsometricWillmoreFlow1D::c.
      // Make sure to use the correct inner product.  (This routine should be
      // extremely short -- in particular, you should *not* need to explicitly build
      // the linear combination of constraint bases as suggested by the notation in
      // the homework!)
   }

   void IsometricWillmoreFlow1D :: integrateFlow( double dt )
   {
      // TODO Integrate Vertex::kappa at each vertex by flowing
      // in the direction stored in IsometricWillmoreFlow1D::kappaDot.
      // Use forward Euler time integration.
   }

   void IsometricWillmoreFlow1D :: recoverTangents( void )
   {
      // TODO Pick an initial tangent direction theta.  (To prevent
      // the curve from jumping around, you may want to use the
      // direction of the initial tangent in the current curve.)

      // TODO Walk along the curve, integrating theta and constructing
      // new tangents as you go.  Make sure you give tangents the
      // correct length!
   }

   void IsometricWillmoreFlow1D :: recoverPositions( void )
   {
      // TODO Pick an initial vertex position gamma.  (This position
      // does not really matter since the curve will automatically
      // be centered around the origin by the Viewer.)

      // TODO Add up tangents end-to-end to reconstruct the new curve.
   }
}
