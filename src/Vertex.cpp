#include <vector>
#include <cmath>
using namespace std;

#include "Vertex.h"
#include "Mesh.h"
#include "HalfEdge.h"
#include "Utility.h"

namespace DDG
{
   double Vertex :: dualLength( void ) const
   // assuming this vertex is a vertex of a closed polygon in the x-y plane,
   // returns the length of the barycentric dual edge associated with this vertex
   {
      // TODO compute the dual length -- you may as well call Edge::length()!

      return 0; // <---- not correct!
   }

   double Vertex :: curvature( void ) const
   // assuming this vertex is a vertex of a closed polygon in the x-y plane,
   // returns the (pointwise) curvature associated with this vertex
   {
      // TODO get the three relevant vertex coordinates a, b, and c

      // TODO compute the edge vectors u and v

      // TODO compute the exterior angle theta between u and v
      // (be careful about sign!)

      // TODO compute the dual length L

      // TODO return the pointwise curvature

      return 0; // <---- not correct!
   }

   Vector Vertex::normal( void ) const
   // returns the vertex normal
   {
      Vector N;

      HalfEdgeCIter h = he;
      do
      {
         N += h->face->normal();
         h = h->flip->next;
      }
      while( h != he );

      return N.unit();
   }

   vector<HalfEdge> isolated; // all isolated vertices point to isolated.begin()

   bool Vertex::isIsolated( void ) const
   // returns true if the vertex is not contained in any face or edge; false otherwise
   {
      return he == isolated.begin();
   }

   int Vertex :: valence( void ) const
   // returns the number of incident faces
   {
      int n = 0;

      HalfEdgeCIter h = he;
      do
      {
         n++;
         h = h->flip->next;
      }
      while( h != he );

      return n;
   }
}

