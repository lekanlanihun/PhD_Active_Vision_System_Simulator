#ifndef _LINEAR_CAMERA_
#define _LINEAR_CAMERA_

#include "world_entity.h"
#include "simple_agents.h"
#include "simple_objects.h"

using namespace std;

class Linear_Camera {
  
 private:
  double depth;
  int    num_pixel;
  double pixel_dimension;
  int num_agents;
  int num_objects;
  vector <vector <double> > camera_pixels;
  int count_obj;
  double distance_nearest_obj;
  
  multimap<double, int> obj_dist;
  double ***pixels;
  bool    **pixels_touched;
  

 public:
  Linear_Camera(double _depth, int num_pix, double pix_dim, int num_agents, int num_objects);
  ~Linear_Camera();
  void update_pixel_rgb_values(  World_Entity *agent, World_Entity *target);
  void reset_camera( void );
  inline const int get_num_pixel ( void ) { return num_pixel;}
  void get_pixel_rgb ( vector <double> &pixels, int p );
  void merge_camera_pixels_view( void );
  inline double get_distance_nearest_obj ( void ) {return distance_nearest_obj;}
  inline double get_depth ( void ) {return depth;}
  
#ifdef _GRAPHICS_
  void render( World_Entity *agent );
  void renderCylinder(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions);
#endif
};

#endif

