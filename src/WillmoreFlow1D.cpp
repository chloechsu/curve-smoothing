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
      computeGradient();
      // TODO Integrate the position of the curve, stored in Vertex::position.
      for(VertexIter v = mesh.vertices.begin(); v!=mesh.vertices.end(); v++)
      {
          v->position += dt * v->energyGradient;
      }
   }

   void WillmoreFlow1D :: computeGradient( void )
   {
      // TODO Initialize the gradient at each vertex to zero --
      // the value of the gradient should be stored in Vertex::energyGradient
      FaceIter gamma = mesh.faces.begin();
      HalfEdgeIter he = gamma->he;
      do {
        he->vertex->energyGradient = Vector(0,0,0);
        he = he->next;
      } while( he != gamma->he );

      // TODO Evaluate the gradient at each vertex.

      he = gamma->he;
      do {
        VertexIter a = he->flip->next->next->vertex;
        VertexIter b = he->vertex;
        VertexIter c = he->next->vertex;
        Vector u = b->position - a->position;
        Vector v = c->position - b->position;
        Vector uhat = u/u.norm();
        Vector vhat = v/v.norm();
        Vector vperpu = v - dot(u,v) * uhat;
        Vector uperpv = u - dot(u,v) * vhat;
        double area = 0.5 * cross(u,v).norm();
        double phi = acos(dot(uhat,vhat));
        a->energyGradient += phi / a->dualLength() * (vperpu/area + 0.5 * phi * uhat);
        b->energyGradient -= phi / c->dualLength() * (uperpv/area + 0.5 * phi * vhat);
        c->energyGradient += phi / b->dualLength() * ((uperpv-vperpu)/area + 0.5 * phi * (vhat - uhat));
        he = he->next;
      } while( he != gamma->he );
   }
}
