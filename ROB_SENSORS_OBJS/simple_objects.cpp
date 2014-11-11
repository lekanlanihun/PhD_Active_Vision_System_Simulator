#include "simple_objects.h"

/* ------------------------------------------------------------ */

SIMPLE_Objects::SIMPLE_Objects( void ) : World_Entity ( )
{
  pos.assign    (3, 0.0);
  dim.assign    (3, 0.0);
  colour.assign (3, 0.0);
  visible       = true;
}

/* ------------------------------------------------------------ */

SIMPLE_Objects::~SIMPLE_Objects(){

}

/* ------------------------------------------------------------ */
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------- */
/* BRICK   BRICK   BRICK   BRICK   BRICK   BRICK   BRICK   BRICK */
/* ------------------------------------------------------------- */

SIMPLE_Brick::SIMPLE_Brick( double *data ) : SIMPLE_Objects( )
{
  pos[0] = data[0]; // X
  pos[1] = data[1]; // Y
  pos[2] = data[2]; // Z
  
  dim[0] = data[3]; //Lenght
  dim[1] = data[4]; //Width
  dim[2] = data[5]; //Height
  
  colour[0] = data[6]; //Red
  colour[1] = data[7]; //Green
  colour[2] = data[8]; //Blue
}

/* ------------------------------------------------------------ */

#ifdef _GRAPHICS_
void SIMPLE_Brick::render ( void ){
  glColor3f(colour[0], colour[1], colour[2]);
  float m[16];
  m[ 0] = 1.0;
  m[ 1] = 0.0;
  m[ 2] = 0.0;
  m[ 3] = 0;
  m[ 4] = 0.0;
  m[ 5] = 1.0;
  m[ 6] = 0.0;
  m[ 7] = 0;
  m[ 8] = 0.0;
  m[ 9] = 0.0;
  m[10] = 1.0;
  m[11] = 0;
  m[12] = pos[0];
  m[13] = pos[1];
  m[14] = pos[2];
  m[15] = 1;
  glPushMatrix();
  glMultMatrixf(m);
  glScalef(dim[0],dim[1],dim[2]);
  glutSolidCube(1.0f);
  glPopMatrix();
}
#endif
/* ------------------------------------------------------------ */
/* CYLINDER    CYLINDER    CYLINDER    CYLINDER    CYLINDER     */
/* ------------------------------------------------------------ */

SIMPLE_Cylinder::SIMPLE_Cylinder( double *data ) : SIMPLE_Objects(  )
{
  pos[0] = data[0]; //X
  pos[1] = data[1]; //Y
  pos[2] = data[2]; //Z
  
  dim[0] = data[3]; //dim[0] = radius
  dim[1] = data[4]; //dim[1] = length
  dim[2] = data[5];
  
  colour[0] = data[6]; //Red
  colour[1] = data[7]; //Green
  colour[2] = data[8]; //Blue
}

/* ------------------------------------------------------------ */

#ifdef _GRAPHICS_
void SIMPLE_Cylinder::render ( void ){
  glColor3f(colour[0], colour[1], colour[2]);
  /*float m[16];
  m[ 0] = 1.0;
  m[ 1] = 0.0;
  m[ 2] = 0.0;
  m[ 3] = 0;
  m[ 4] = 0.0;
  m[ 5] = 1.0;
  m[ 6] = 0.0;
  m[ 7] = 0;
  m[ 8] = 0.0;
  m[ 9] = 0.0;
  m[10] = 1.0;
  m[11] = 0;
  m[12] = pos[0];
  m[13] = pos[1];
  m[14] = pos[2];
  m[15] = 1;*/
  
  glPushMatrix();
  //glMultMatrixf(m);
  glTranslatef(pos[0], pos[1], pos[2]);
  GLUquadric* params_quadric = gluNewQuadric();
  glColor3f(colour[0], colour[1], colour[2] );
  gluCylinder(params_quadric, dim[0], dim[0], dim[1], robot_slices, 1);
  gluDisk(params_quadric, 0, dim[0], robot_slices, 1);
  glTranslated(0,0,dim[1]);
  gluDisk(params_quadric, 0, dim[0], robot_slices, 1);
  gluDeleteQuadric(params_quadric);
  /*glBegin(GL_TRIANGLES);
  for (int i = 0; i<100; i++)
    {
      float n = i*(M_PI/50);
      //glColor3f((n/2),1-(n/2),sin(n));
      glVertex3f((dim[0]*sin(n)),(dim[0]*cos(n)), 0);
      n = (1+i)*(M_PI/50);
      glVertex3f((dim[0]*sin(n)),(dim[0]*cos(n)), 0);
      glVertex3f(0, 0, 0);
    }
  glEnd();
  GLUquadric* quad = gluNewQuadric();
  gluCylinder( quad, dim[0], dim[0], dim[1], 30, 30 );
  gluDeleteQuadric( quad );
  glTranslatef( 0, 0, dim[1] );
  glBegin(GL_TRIANGLES);
  for (int i = 0; i<100; i++)
    {
      float n = i*(M_PI/50);
      //glColor3f((n/2),1-(n/2),sin(n));
      glVertex3f((dim[0]*sin(n)),(dim[0]*cos(n)), 0);
      n = (1+i)*(M_PI/50);
      glVertex3f((dim[0]*sin(n)),(dim[0]*cos(n)), 0);
      glVertex3f(0, 0, 0);
    }
  glEnd();*/
  glPopMatrix();
}
#endif

/* ------------------------------------------------------------ */
/* SPHERE    SPHERE    SPHERE    SPHERE    SPHERE     SPHERE    */
/* ------------------------------------------------------------ */

SIMPLE_Sphere::SIMPLE_Sphere( double *data ) : SIMPLE_Objects( ) 
{
  
  pos[0] = data[0]; //X
  pos[1] = data[1]; //Y
  pos[2] = data[2]; //Z
  
  dim[0] = data[3]; //Lenght
  dim[1] = data[4]; //Width
  dim[2] = data[5]; //Height
  
  colour[0] = data[6]; //Red
  colour[1] = data[7]; //Green
  colour[2] = data[8]; //Blue
  
}

/* ------------------------------------------------------------ */

#ifdef _GRAPHICS_
void SIMPLE_Sphere::render ( void ){
  float m[16];
  m[ 0] = 1.0;
  m[ 1] = 0.0;
  m[ 2] = 0.0;
  m[ 3] = 0;
  m[ 4] = 0.0;
  m[ 5] = 1.0;
  m[ 6] = 0.0;
  m[ 7] = 0;
  m[ 8] = 0.0;
  m[ 9] = 0.0;
  m[10] = 1.0;
  m[11] = 0;
  m[12] = pos[0];
  m[13] = pos[1];
  m[14] = pos[2];
  m[15] = 1;

  glColor3f(colour[0], colour[1], colour[2]);
  glPushMatrix();
  glMultMatrixf(m);
  glutSolidSphere(dim[0],100,100);
  glPopMatrix();
}
#endif
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*    LIGHT    LIGHT    LIGHT    LIGHT    LIGHT     LIGHT       */
/* ------------------------------------------------------------ */

SIMPLE_Light::SIMPLE_Light( double *data ) : SIMPLE_Objects( ) 
{
  
  pos[0] = data[0]; //X
  pos[1] = data[1]; //Y
  pos[2] = data[2]; //Z
  
  dim[0] = data[3]; //Lenght
  dim[1] = data[4]; //Width
  dim[2] = data[5]; //Height
  
  colour[0] = data[6]; //Red
  colour[1] = data[7]; //Green
  colour[2] = data[8]; //Blue
  
}

/* ------------------------------------------------------------ */

#ifdef _GRAPHICS_
void SIMPLE_Light::render ( void ){
  float m[16];
 m[ 0] = 1.0;
  m[ 1] = 0.0;
  m[ 2] = 0.0;
  m[ 3] = 0;
  m[ 4] = 0.0;
  m[ 5] = 1.0;
  m[ 6] = 0.0;
  m[ 7] = 0;
  m[ 8] = 0.0;
  m[ 9] = 0.0;
  m[10] = 1.0;
  m[11] = 0;
  m[12] = pos[0];
  m[13] = pos[1];
  m[14] = pos[2];
  m[15] = 1;
  
  glColor3f(colour[0], colour[1], colour[2]);
  glPushMatrix();
  glMultMatrixf(m);
  glutSolidSphere(dim[0],100,100);
  glPopMatrix();
}
#endif

/* ------------------------------------------------------------ */
/* ------------------------------------------------------------ */
