#ifndef _SIMPLE_OBJECTS_
#define _SIMPLE_OBJECTS_

#include "world_entity.h"

using namespace std;

class SIMPLE_Objects : public World_Entity {
  
 protected:
  bool visible;
  vector <double> pos;
  vector <double> dim;
  static const int robot_slices    = 20;
  
 private:
  friend class IR_Sensors;
  friend class AL_Sensors;
  friend class Linear_Camera;
  
 public:
  SIMPLE_Objects( );
  virtual ~SIMPLE_Objects();
  
  inline bool is_visible                   ( void )      { return visible; }
  inline void set_visible                  ( bool flag ) { visible = flag; }

  virtual const int are_you_this_object    ( char *str ) = 0;
  inline void  set_pos                     ( const vector <double> &_pos ) {
    pos[0] = _pos[0];
    pos[1] = _pos[1];
    pos[2] = _pos[2];
  }
  inline const vector <double> get_pos     ( void ){ return pos;}
  inline const vector <double> get_tmp_pos     ( void ){ return pos;}
  inline const vector <double> get_dim     ( void ){ return dim;}
  inline const vector <double> get_colour  ( void ){ return colour;}
  inline void set_colour ( const vector <double> c ) {
    colour[0] = c[0];
    colour[1] = c[1];
    colour[2] = c[2];
  }
  virtual inline const vector <double> get_rot     ( void ){ vector <double> rot; return rot;}

#ifdef _GRAPHICS_
  virtual void    render                   ( void ) = 0;
#endif 
  
};

/* ------------------------------------------------------------ */

class SIMPLE_Brick : public SIMPLE_Objects {
  
 public:
  SIMPLE_Brick( double *data );
  inline  const int are_you_this_object ( char *str ){return strcmp("brick", str);}
#ifdef _GRAPHICS_
  void    render                   ( void );
#endif
};

/* ------------------------------------------------------------ */

class SIMPLE_Cylinder : public SIMPLE_Objects {
  
 public:
  SIMPLE_Cylinder( double *data );
  inline  const int are_you_this_object ( char *str ){return strcmp("cylinder", str);}
#ifdef _GRAPHICS_
  void    render                   ( void );
#endif
};

/* ------------------------------------------------------------ */

class SIMPLE_Sphere : public SIMPLE_Objects {
  
 public:
  SIMPLE_Sphere( double *data );
  inline  const int are_you_this_object ( char *str ){return strcmp("sphere", str);}
#ifdef _GRAPHICS_
  void    render                     ( void );
#endif
};

/* ------------------------------------------------------------ */

class SIMPLE_Light : public SIMPLE_Objects {
  
 public:
  SIMPLE_Light( double *data );
  inline  const int are_you_this_object ( char *str ){return strcmp("light", str);}
#ifdef _GRAPHICS_
  void    render                     ( void );
#endif
};

/* ------------------------------------------------------------ */

#endif
