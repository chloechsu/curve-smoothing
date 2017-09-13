namespace DDG
{
   template<class T>
   SparseMatrix<T> DifferentialOperators<T> :: Laplacian1D( const Mesh& mesh )
   {
      int nV = mesh.vertices.size();
      SparseMatrix<T> L( nV, nV );

      FaceCIter f = mesh.faces.begin();
      HalfEdgeCIter he = f->he;
      do
      {
         VertexIter va = he->vertex;
         VertexIter vb = he->next->vertex;
         VertexIter vc = he->next->next->vertex;

         Vector a = va->position;
         Vector b = vb->position;
         Vector c = vc->position;

         int ia = va->index;
         int ib = vb->index;
         int ic = vc->index;

         double wba = 1./(b-a).norm();
         double wbc = 1./(c-b).norm();

         L( ib, ia ) = -wba;
         L( ib, ib ) = wba+wbc;
         L( ib, ic ) = -wbc;

         he = he->next;
      }
      while( he != f->he );

      return L;
   }

   template<class T>
   SparseMatrix<T> DifferentialOperators<T> :: vertexMass1D( const Mesh& mesh )
   {
      int nV = mesh.vertices.size();
      SparseMatrix<T> M( nV, nV );

      FaceCIter f = mesh.faces.begin();
      HalfEdgeCIter he = f->he;
      do
      {
         VertexIter va = he->vertex;
         VertexIter vb = he->next->vertex;
         VertexIter vc = he->next->next->vertex;

         Vector a = va->position;
         Vector b = vb->position;
         Vector c = vc->position;

         int i = vb->index;

         double lba = (b-a).norm();
         double lbc = (c-b).norm();

         M( i, i ) = (lba+lbc)/2.;

         he = he->next;
      }
      while( he != f->he );

      return M;
   }
}
