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

   void IsometricWillmoreFlow1D :: integrate( double dt, double lambda, double mu, int filterPolynomialPower )
   {
      getCurvature();
      buildMassMatrix();
      buildLaplacian();
      buildConstraints();
      orthogonalizeConstraints();
      computeFlowDirection();
      applyFilter( lambda, mu, filterPolynomialPower );
      enforceConstraints();
      integrateFlow( dt );
      recoverTangents();
      recoverPositions();
   }

   void IsometricWillmoreFlow1D :: getCurvature( void )
   {
      // TODO Evaluate Vertex::curvature() for each vertex and
      // store the result in Vertex::kappa.
      for(VertexIter v=mesh.vertices.begin();v!=mesh.vertices.end();v++)
          v->kappa = v->curvature();
   }

   void IsometricWillmoreFlow1D :: buildMassMatrix( void )
   {
      // TODO This class contains a member "B" that will hold the
      // mass matrix, but it has not yet been initialized -- initialize
      // it (with the appropriate size) here.
      B = SparseMatrix<Real>(mesh.vertices.size(), mesh.vertices.size());

      // TODO Set the diagonal entries of B -- as in your last homework,
      // each vertex already has a unique index stored in Vertex::index.
      
      for(VertexIter v=mesh.vertices.begin(); v!=mesh.vertices.end(); v++)
        B(v->index,v->index) = v->dualLength();
   }

   void IsometricWillmoreFlow1D :: buildConstraints( void )
   {
      // TODO The constraints will be stored as an STL vector of
      // DenseMatrices -- see IsometricWillmoreFlow1D.h.  Since
      // you have three constraints, you will need to resize this
      // vector, and will need to allocate each of the DenseMatrices
      // it contains to the appropriate size.  (If you are unfamiliar
      // with STL vectors, Google for "stl vector".)
      c.resize(3);
      c[0] = DenseMatrix<Real>(mesh.vertices.size(),1);
      c[1] = DenseMatrix<Real>(mesh.vertices.size(),1);
      c[2] = DenseMatrix<Real>(mesh.vertices.size(),1);
      // TODO At each vertex, set the values of the three constraint vectors.
      for(VertexIter v=mesh.vertices.begin();v!=mesh.vertices.end();v++)
      {
        c[0](v->index,0) = 1;
        c[1](v->index,0) = v->position.x;
        c[2](v->index,0) = v->position.y;
      }
   }

   void IsometricWillmoreFlow1D :: orthogonalizeConstraints( void )
   {
      // TODO Orthogonalize the vectors in IsometricWillmoreFlow1D::c
      // with respect to the L2 inner product on primal 0-forms, using
      // the Gram-Schmidt process.  Do not assume that there are only
      // three constraints!  Your code should work for a vector c of
      // any size.
      for(int i=0;i<c.size();i++)
      {
        for(int j=0;j<i;j++)
        {
            c[i] -= inner(c[j],B,c[i]) * c[j];
        }
        c[i] /= sqrt(inner(c[i],B,c[i]));
      }
   }

   void IsometricWillmoreFlow1D :: computeFlowDirection( void )
   {
      // TODO Allocate the vector IsometricWillmoreFlow1D::kappaDot, which
      // stores the direction of the gradient flow per vertex.
      kappaDot = DenseMatrix<Real>(mesh.vertices.size(), 1); 

      // TODO Compute the direction of the gradient flow at each vertex (without
      // worrying about constraints for the moment) and store it in kappaDot.
      // Remember that each vertex already has a unique integer index.
      for(VertexIter v=mesh.vertices.begin();v!=mesh.vertices.end();v++)
        kappaDot(v->index,0) = - 2 * v->kappa;
   }

   void IsometricWillmoreFlow1D :: applyFilter( double lambda, double mu, int filterPolynomialPower )
   {
      // lambda < mu <= 0.5
      // apply filter (1 - lambda (2-K))(1 + mu (2-K)) ^ filterPolynomialPower
      SparseMatrix<Real> id = SparseMatrix<Real>::identity(mesh.vertices.size());
      SparseMatrix<Real> K = laplacian;
      int N = filterPolynomialPower;
      while(N--)
        kappaDot = (id - Real(lambda) * (Real(2) * id - K)) * (id + Real(mu) * (Real(2) * id - K)) * kappaDot;
   }
   
   void IsometricWillmoreFlow1D :: enforceConstraints( void )
   {
      // TODO Remove the constraint components from the flow direction
      // kappaDot, using the orthonormal basis stored in IsometricWillmoreFlow1D::c.
      // Make sure to use the correct inner product.  (This routine should be
      // extremely short -- in particular, you should *not* need to explicitly build
      // the linear combination of constraint bases as suggested by the notation in
      // the homework!)
      for(int i=0;i<c.size();i++)
          kappaDot -= inner(kappaDot,B,c[i]) * c[i];
   }

   void IsometricWillmoreFlow1D :: integrateFlow( double dt )
   {
      // TODO Integrate Vertex::kappa at each vertex by flowing
      // in the direction stored in IsometricWillmoreFlow1D::kappaDot.
      // Use forward Euler time integration.
      for(VertexIter v=mesh.vertices.begin();v!=mesh.vertices.end();v++)
          v->kappa += kappaDot(v->index,0) * dt;
   }

   void IsometricWillmoreFlow1D :: recoverTangents( void )
   {
      // TODO Pick an initial tangent direction theta.  (To prevent
      // the curve from jumping around, you may want to use the
      // direction of the initial tangent in the current curve.)
      FaceIter gamma = mesh.faces.begin();
      HalfEdgeIter he = gamma->he;
      Vector initialTangent = he->next->vertex->position - he->vertex->position;
      double theta = acos(initialTangent.x / initialTangent.norm());
      if (sin(theta) * initialTangent.y < 0)
          theta = -theta;
      theta -= he->vertex->kappa * B(he->vertex->index,he->vertex->index);

      // TODO Walk along the curve, integrating theta and constructing
      // new tangents as you go.  Make sure you give tangents the
      // correct length!
      do {
          theta += he->vertex->kappa * B(he->vertex->index,he->vertex->index);
          double L = (he->next->vertex->position - he->vertex->position).norm();
          he->tangent.x = cos(theta) * L;
          he->tangent.y = sin(theta) * L;
          he = he->next;
      }
      while(he != gamma->he);
   }

   void IsometricWillmoreFlow1D :: recoverPositions( void )
   {
      // TODO Pick an initial vertex position gamma.  (This position
      // does not really matter since the curve will automatically
      // be centered around the origin by the Viewer.)

      // TODO Add up tangents end-to-end to reconstruct the new curve.
      FaceIter gamma = mesh.faces.begin();
      HalfEdgeIter he = gamma->he;
      Vector pos = he->vertex->position;
      do {
          he->vertex->position = pos;
          pos += he->tangent;
          he = he->next;
      }
      while(he != gamma->he);
   }

   void IsometricWillmoreFlow1D :: buildLaplacian( void )
   {
      laplacian = SparseMatrix<Real>(mesh.vertices.size(), mesh.vertices.size());
      FaceIter gamma = mesh.faces.begin();
      HalfEdgeIter he = gamma->he;
      do {
          VertexIter a = he->flip->next->next->vertex;
          VertexIter b = he->vertex;
          VertexIter c = he->next->vertex;
          laplacian(b->index, a->index) = -0.5;
          laplacian(b->index, c->index) = -0.5;
          laplacian(b->index, b->index) = 1;
          he = he->next;
      }
      while(he != gamma->he);
   }
}
