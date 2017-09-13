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
       return 0.5 * he->edge->length() + 0.5 * he->flip->next->edge->length();
   }

   double Vertex :: curvature( void ) const
   // assuming this vertex is a vertex of a closed polygon in the x-y plane,
   // returns the (pointwise) curvature associated with this vertex
   {
      // TODO get the three relevant vertex coordinates a, b, and c
      Vector a = he->flip->next->next->vertex->position;
      Vector b = position;
      Vector c = he->next->vertex->position;
      // TODO compute the edge vectors u and v
      Vector u = b - a;
      Vector v = c - b;
      Vector uhat = u/u.norm();
      Vector vhat = v/v.norm();
      // TODO compute the exterior angle theta between u and v
      // (be careful about sign!)
      double theta = asin(cross(uhat,vhat).z);
      if(dot(uhat,vhat) < 0)
      {
          if(theta > 0)
            theta = acos(0)*2 - theta;
          if(theta < 0)
            theta = -acos(0)*2 - theta;
      }
      // TODO compute the dual length L
      double L = dualLength();
      // TODO return the pointwise curvature
      return theta/L;
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

