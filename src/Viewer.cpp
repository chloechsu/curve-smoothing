#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

#include "Viewer.h"
#include "Image.h"

namespace DDG
{
   // declare static member variables
   Mesh Viewer::mesh;
   Viewer::RenderMode Viewer::mode = renderWireframe;
   GLuint Viewer::surfaceDL = 0;
   int Viewer::windowSize[2] = { 800, 800 };
   Camera Viewer::camera;
   Shader Viewer::shader;

   WillmoreFlow1D* Viewer::willmoreFlow = NULL;
   IsometricWillmoreFlow1D* Viewer::isometricWillmoreFlow = NULL;
   Viewer::FlowType Viewer::flowType = flowWillmore;

   bool Viewer::animate = false;

   double Viewer::timeStep;
   int Viewer::timeStepExponent = -9;
   int Viewer::timeStepMantissa = 1;
   void Viewer :: updateTimeStep( void )
   {
      timeStep = (double) timeStepMantissa *
       pow( 10., (double) timeStepExponent );
   }
   
   void Viewer :: init( void )
   {
      restoreViewerState();
      initGLUT();
      initGL();
      initGLSL();

      willmoreFlow = new WillmoreFlow1D( mesh );
      isometricWillmoreFlow = new IsometricWillmoreFlow1D( mesh );
      updateTimeStep();
   
      updateDisplayList();
   
      glutMainLoop();
   }

   void Viewer :: initGL( void )
   {
      //glClearColor( .5, .5, .5, 1. );
      glClearColor( 1., 1., 1., 1. );
      initLighting();
   }
   
   void Viewer :: initGLUT( void )
   {
      int argc = 0;
      vector< vector<char> > argv(1);
   
      // initialize window
      glutInitWindowSize( windowSize[0], windowSize[1] );
      glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
      glutInit( &argc, (char**)&argv );
      glutCreateWindow( "DDG" );
   
      // specify callbacks
      glutDisplayFunc  ( Viewer::display  );
      glutIdleFunc     ( Viewer::idle     );
      glutKeyboardFunc ( Viewer::keyboard );
      glutSpecialFunc  ( Viewer::special  );
      glutMouseFunc    ( Viewer::mouse    );
      glutMotionFunc   ( Viewer::motion   );
   
      // initialize menus
      int viewMenu = glutCreateMenu( Viewer::view );
      glutSetMenu( viewMenu );
      glutAddMenuEntry( "[s] Smooth Shaded",  menuSmoothShaded );
      glutAddMenuEntry( "[f] Wireframe",      menuWireframe    );
      glutAddMenuEntry( "[↑] Zoom In",    menuZoomIn       );
      glutAddMenuEntry( "[↓] Zoom Out",   menuZoomOut      );

      int flowMenu = glutCreateMenu( Viewer::mFlow );
      glutSetMenu( flowMenu );
      glutAddMenuEntry( "[l] Willmore Flow",                  menuWillmoreFlow               );
      glutAddMenuEntry( "[i] Isometric Willmore Flow",        menuIsometricWillmoreFlow      );

      int mainMenu = glutCreateMenu( Viewer::menu );
      glutSetMenu( mainMenu );
      glutAddMenuEntry( "[space] Toggle Flow", menuProcess    );
      glutAddMenuEntry( "[r] Reset Curve",     menuResetMesh  );
      glutAddMenuEntry( "[w] Write Curve",     menuWriteMesh  );
      glutAddMenuEntry( "[\\] Screenshot",     menuScreenshot );
      glutAddMenuEntry( "[esc] Exit",          menuExit       );
      glutAddSubMenu( "Flow", flowMenu );
      glutAddSubMenu( "View", viewMenu );
      glutAttachMenu( GLUT_RIGHT_BUTTON );
   }

   void Viewer :: initGLSL( void )
   {
      shader.loadVertex( "shaders/vertex.glsl" );
      shader.loadFragment( "shaders/fragment.glsl" );
   }
   
   void Viewer :: initLighting( void )
   {
      GLfloat position[4] = { 20., 30., 40., 0. };
      glLightfv( GL_LIGHT0, GL_POSITION, position );
      glEnable( GL_LIGHT0 );
      glEnable( GL_NORMALIZE );
   }
   
   void Viewer :: menu( int value )
   {
      switch( value )
      {
         case( menuProcess ):
            mProcess();
            break;
         case( menuResetMesh ):
            mResetMesh();
            break;
         case( menuWriteMesh ):
            mWriteMesh();
            break;
         case( menuScreenshot ):
            mScreenshot();
            break;
         case( menuExit ):
            mExit();
            break;
         default:
            break;
      }
   }
   
   void Viewer :: view( int value )
   {
      switch( value )
      {
         case( menuSmoothShaded ):
            mSmoothShaded();
            break;
         case( menuWireframe ):
            mWireframe();
            break;
         case( menuZoomIn ):
            mZoomIn();
            break;
         case( menuZoomOut ):
            mZoomOut();
            break;
         default:
            break;
      }
   }

   void Viewer :: mFlow( int value )
   {
      switch( value )
      {
         case( menuWillmoreFlow ):
            mWillmoreFlow();
            break;
         case( menuIsometricWillmoreFlow ):
            mIsometricWillmoreFlow();
            break;
         default:
            break;
      }
   }
   
   void Viewer :: mProcess( void )
   {
      if( flowType == flowWillmore )
      {
         willmoreFlow->integrate( timeStep );
      }
      else if( flowType == flowIsometricWillmore )
      {
         isometricWillmoreFlow->integrate( timeStep );
      }

      mesh.center();

      updateDisplayList();
   }

   void Viewer :: mResetMesh( void )
   {
      mesh.reload();
      updateDisplayList();
   }
   
   void Viewer :: mWriteMesh( void )
   {
      mesh.write( "out.obj" );
   }
   
   void Viewer :: mExit( void )
   {
      if(          willmoreFlow ) delete willmoreFlow;
      if( isometricWillmoreFlow ) delete isometricWillmoreFlow;

      storeViewerState();
      exit( 0 );
   }
   
   void Viewer :: mSmoothShaded( void )
   {
      mode = renderShaded;
      updateDisplayList();
   }
   
   void Viewer :: mWireframe( void )
   {
      mode = renderWireframe;
      updateDisplayList();
   }

   void Viewer :: mZoomIn( void )
   {
      camera.zoomIn();
   }

   void Viewer :: mZoomOut( void )
   {
      camera.zoomOut();
   }

   void Viewer :: mWillmoreFlow( void )
   {
      flowType = flowWillmore;
   }

   void Viewer :: mIsometricWillmoreFlow( void )
   {
      flowType = flowIsometricWillmore;
   }

   void Viewer :: mToggleAnimate( void )
   {
      animate = !animate;
   }

   void Viewer :: mScreenshot( void )
   {
      static int index = 0;
   
      // get window width and height
      GLint view[4];
      glGetIntegerv( GL_VIEWPORT, view );
      int w = view[2];
      int h = view[3];
   
      // get pixels
      Image image( w, h );
      glReadPixels( 0, 0, w, h, GL_BGR, GL_FLOAT, &image(0,0) );
   
      stringstream filename;
      filename << "frames/viewer" << setw(8) << setfill( '0' ) << index << ".tga";
      image.write( filename.str().c_str() );
   
      index++;
   }
   
   void Viewer :: keyboard( unsigned char c, int x, int y )
   {
      switch( c )
      {
         case 's':
            mSmoothShaded();
            break;
         case 'f':
            mWireframe();
            break;
         case 'w':
            mWriteMesh();
            break;
         case 'r':
            mResetMesh();
            break;
         case '\\':
            mScreenshot();
            break;
         case ' ':
            mToggleAnimate();
            break;
         case '-':
            timeStepExponent--;
            updateTimeStep();
            break;
         case '=':
            timeStepExponent++;
            updateTimeStep();
            break;
         case '_':
            timeStepMantissa--;
            if( timeStepMantissa == 0 )
            {
               timeStepMantissa = 9;
               timeStepExponent--;
            }
            updateTimeStep();
            break;
         case '+':
            timeStepMantissa++;
            if( timeStepMantissa == 10 )
            {
               timeStepMantissa = 1;
               timeStepExponent++;
            }
            updateTimeStep();
            break;
         case 'l':
            mWillmoreFlow();
            break;
         case 'i':
            mIsometricWillmoreFlow();
            break;
         case 27:
            mExit();
            break;
         default:
            break;
      }
   }

   void Viewer :: special( int i, int x, int y )
   {
      switch( i )
      {
         case GLUT_KEY_UP:
            camera.zoomIn();
            break;
         case GLUT_KEY_DOWN:
            camera.zoomOut();
            break;
         case 27:
            mExit();
            break;
         default:
            break;
      }
   }
   
   void Viewer :: display( void )
   {
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

      glMatrixMode( GL_PROJECTION );
      glLoadIdentity();
   
      GLint viewport[4];
      glGetIntegerv( GL_VIEWPORT, viewport );
      double aspect = (double) viewport[2] / (double) viewport[3];
      const double fovy = 50.;
      const double clipNear = .01;
      const double clipFar = 1000.;
      gluPerspective( fovy, aspect, clipNear, clipFar );
   
      glMatrixMode( GL_MODELVIEW );
      glLoadIdentity();

      Quaternion    eye = Vector( 0., 0., 2.5*camera.zoom );
      Quaternion center = Vector( 0., 0., 0. );
      Quaternion     up = Vector( 0., 1., 0. );

      gluLookAt(    eye[1],    eye[2],    eye[3],
                 center[1], center[2], center[3],
                     up[1],     up[2],     up[3] );


      Quaternion r = camera.currentRotation();

      eye = r.conj() * eye * r;
      GLint uniformEye = glGetUniformLocation( shader, "eye" );
      glUniform3f( uniformEye, eye[1], eye[2], eye[3] );

      Quaternion light = Vector( -1., 1., -2. );
      light = r.conj() * light * r;
      GLint uniformLight = glGetUniformLocation( shader, "light" );
      glUniform3f( uniformLight, light[1], light[2], light[3] );

      camera.setView();
   
      drawSurface();
      drawFlowInfo();

      glutSwapBuffers();
   }

   void Viewer :: setMeshMaterial( void )
   {
      GLfloat  diffuse[4] = { .8, .5, .3, 1. };
      GLfloat specular[4] = { .3, .3, .3, 1. };
      GLfloat  ambient[4] = { .2, .2, .5, 1. };
   
      glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE,   diffuse  );
      glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR,  specular );
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT,   ambient  );
      glMaterialf ( GL_FRONT_AND_BACK, GL_SHININESS, 16.      );
   }
   
   void Viewer :: drawSurface( void )
   {
      glPushAttrib( GL_ALL_ATTRIB_BITS );

      glEnable( GL_DEPTH_TEST );
      glEnable( GL_LIGHTING );
   
      glCallList( surfaceDL );
   
      glPopAttrib();
   }
   
   void Viewer :: drawMesh( void )
   {
      glPushAttrib( GL_ALL_ATTRIB_BITS );

      drawWireframe();

      glPopAttrib();
   }

   void Viewer :: drawPolygons( void )
   {
      for( FaceCIter f  = mesh.faces.begin();
                     f != mesh.faces.end();
                     f ++ )
      {
         if( f->isBoundary() ) continue;

         glBegin( GL_POLYGON );
         if( mode == renderWireframe )
         {
            Vector N = f->normal();
            glNormal3dv( &N[0] );
         }

         HalfEdgeCIter he = f->he;
         do
         {
            if( mode != renderWireframe )
            {
               Vector N = he->vertex->normal();
               glNormal3dv( &N[0] );
            }

            glVertex3dv( &he->vertex->position[0] );

            he = he->next;
         }
         while( he != f->he );
         glEnd();
      }
   }

   void Viewer :: drawWireframe( void )
   {
      glPushAttrib( GL_ALL_ATTRIB_BITS );

      glDisable( GL_LIGHTING );
      glEnable( GL_BLEND );
      glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
      glEnable( GL_LINE_SMOOTH );
      glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );

      glDisable( GL_DEPTH_TEST );

      if( mode == renderWireframe )
      {
         glLineWidth( 4. );
      }
      if( mode == renderShaded )
      {
         glLineWidth( 3. );
      }

      //glColor4f( 1., 1., 1., 1. );
      glColor4f( 1., .5, 0., 1. );

      glBegin( GL_LINE_LOOP );
      for( EdgeCIter e  = mesh.edges.begin();
                     e != mesh.edges.end();
                     e ++ )
      {
         glVertex3dv( &e->he->vertex->position[0] );
      }
      glEnd();

      if( mode == renderWireframe )
      {
         glColor4f( 0., 0., 0., 1. );
         glPointSize( 6. );
         glEnable( GL_POINT_SMOOTH );
         glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );
         glBegin( GL_POINTS );
         for( EdgeCIter e  = mesh.edges.begin();
               e != mesh.edges.end();
               e ++ )
         {
            glVertex3dv( &e->he->vertex->position[0] );
         }
         glEnd();
      }

      glPopAttrib();
   }

   void Viewer :: drawIsolatedVertices( void )
   {
      glPushAttrib( GL_ALL_ATTRIB_BITS );

      // draw with big, round, red dots
      glPointSize( 5 );
      glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );
      glEnable( GL_POINT_SMOOTH );
      glEnable( GL_BLEND );
      glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
      glColor4f( 1., 0., 0., 1. ); // red

      glBegin( GL_POINTS );
      for( VertexCIter v  = mesh.vertices.begin();
                       v != mesh.vertices.end();
                       v ++ )
      {
         if( v->isIsolated() )
         {
            glVertex3dv( &v->position[0] );
         }
      }
      glEnd();

      glPopAttrib();
   }
   
   void Viewer :: updateDisplayList( void )
   {
      if( surfaceDL )
      {
         glDeleteLists( surfaceDL, 1 );
         surfaceDL = 0;
      }
   
      surfaceDL = glGenLists( 1 );
   
      setMeshMaterial();
   
      glNewList( surfaceDL, GL_COMPILE );
      drawMesh();
      glEndList();
   }
   
   void Viewer :: mouse( int button, int state, int x, int y )
   {
      //camera.mouse( button, state, x, y );
   }

   void Viewer :: motion( int x, int y )
   {
      //camera.motion( x, y );
   }
   
   void Viewer :: idle( void )
   {
      camera.idle();

      if( animate )
      {
         mProcess();
      }

      glutPostRedisplay();
   }

   void Viewer :: storeViewerState( void )
   {
      ofstream out( ".viewer_state.txt" );

      out << camera.rLast[0] << endl;
      out << camera.rLast[1] << endl;
      out << camera.rLast[2] << endl;
      out << camera.rLast[3] << endl;

      GLint view[4];
      glGetIntegerv( GL_VIEWPORT, view );
      out << view[2] << endl;
      out << view[3] << endl;

      out << (int) mode << endl;
   }

   void Viewer :: restoreViewerState( void )
   {
      ifstream in( ".viewer_state.txt" );

      if( !in.is_open() ) return;

      // in >> camera.rLast[0];
      // in >> camera.rLast[1];
      // in >> camera.rLast[2];
      // in >> camera.rLast[3];

      in >> windowSize[0];
      in >> windowSize[1];

      int m;
      in >> m;
      mode = (RenderMode) m;
   }

   void Viewer::drawFlowInfo( void )
   {
      glPushAttrib( GL_ALL_ATTRIB_BITS );
      glDisable( GL_LIGHTING );
      glDisable( GL_DEPTH_TEST );

      GLint viewport[4];
      glGetIntegerv( GL_VIEWPORT, viewport );
      int W = viewport[2];
      int H = viewport[3];

      glMatrixMode( GL_PROJECTION ); glPushMatrix(); glLoadIdentity();
      glOrtho( 0., W, 0., H, .1, 100. );
      glMatrixMode( GL_MODELVIEW  ); glPushMatrix(); glLoadIdentity();
      glTranslatef( 0., 0., -1. );

      glColor3f( 0., 0., 0. );
      
      // display flow type
      {
         stringstream ss;
         ss << "flow: ";
         switch( flowType )
         {
            case( flowWillmore ):
               ss << "Willmore";
               break;
            case( flowIsometricWillmore ):
               ss << "isometric Willmore";
               break;
            default:
               ss << "(not selected)";
               break;
         }

         string s = ss.str();
         glRasterPos2f( W-9*(s.length()+1), H-16 );
         for( size_t i = 0; i < s.length(); i++ )
         {
            glutBitmapCharacter( GLUT_BITMAP_9_BY_15, s[i] );
         }
      }
      
      // display time step
      {
         stringstream ss;
         ss.precision( 3 );
         ss << "time step: " << scientific << timeStep;
         string s = ss.str();
         glRasterPos2f( W-9*(s.length()+1), H-32 );
         for( size_t i = 0; i < s.length(); i++ )
         {
            glutBitmapCharacter( GLUT_BITMAP_9_BY_15, s[i] );
         }
      }

      glMatrixMode( GL_PROJECTION ); glPopMatrix();
      glMatrixMode( GL_MODELVIEW  ); glPopMatrix();

      glPopAttrib();
   }
}

