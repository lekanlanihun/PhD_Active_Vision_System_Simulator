#ifndef _SIMPLE_AGENTS_
#define _SIMPLE_AGENTS_

#include "world_entity.h"

#define EPSILON (double)(exp(-3.0))

class SIMPLE_Agents : public World_Entity {
  
 protected:
  vector <double> pos;
  vector <double> dim;
  vector <double> previous_pos;
  vector <double> tmp_pos;

  vector <double> rot;
  vector <double> previous_rot;
  vector <double> vel;
  vector <double> long_term;
  vector <double> medium_term;

  double right_out;
  double left_out;

  bool   crashed;
  double centreDisplacement[2];
  double displacement;
  double direction_of_displacement;
  bool frozen;
  double updates_per_second;
  
  static const double robot_radius        = 2.7;
  static const double half_robot_radius   = 1.35;
  static const double robot_height        = 5.0;
  static const int robot_slices           = 20;
  static const double wheels_distance     = 5.2;
  static const double cm_per_pulse        = 0.8;
  static const double pos_noise           = 0.01;
  static const double rot_noise           = 0.0087;
  static const double vel_noise           = 0.0;
  static const double max_vel             = 8.0;
  static const double min_vel             = -8.0;
  static const double prob_long_term_change = 0.02;
  static const double prob_medium_term_change = 0.1;
  
  friend class IR_Sensors;
  friend class AL_Sensors;
  friend class Linear_Camera;
  
 public:
  SIMPLE_Agents();
  SIMPLE_Agents( double simulation_time_step );
  ~SIMPLE_Agents();
  void   set_pos( const vector <double> &_pos );
  void   set_rot( const vector <double> &_rot );
  void   set_vel( const vector <double> &_vel );
  
  inline const vector <double> get_pos( void ){return pos;}
  inline const vector <double> get_tmp_pos( void ){return tmp_pos;}
  inline const vector <double> get_dim( void ){ return dim;}
  inline const vector <double> get_colour  ( void ){ return colour;}
  inline void set_colour ( const vector <double> c ) {
    colour[0] = c[0];
    colour[1] = c[1];
    colour[2] = c[2];
  }
  inline const vector <double> get_rot( void ){return rot;}
  inline const vector <double> get_vel( void ){return vel;}
  inline static const double   get_max_vel( void ) {return max_vel;}
  inline static const double   get_min_vel( void ) {return min_vel;}
  inline static const double get_radius( void ){return robot_radius;}
  
  inline const double get_right_out    ( void ) { return right_out;}
  inline const double get_left_out     ( void ) { return left_out;}
  inline void         set_frozen       ( bool flag ) { frozen = flag;}
  inline const bool   is_frozen        ( void ) { return frozen;}
  inline const bool   is_crashed       ( void ) { return crashed;}

  void   update_pos_rot ( void );
  double pinkq          ( int gen );
  bool   check_collision_with_round_object ( World_Entity *target );
  void   return_to_previous_position( void );
  
  void clear_var_before_collision_check( void );
  void update_direction_of_displacement( void );
  void update_displacement_round_obj(  World_Entity  *target );
  void update_tmp_position_after_collisions( void );
  void update_position_after_collisions( void );


#ifdef _GRAPHICS_
  void   render         ( void );
#endif
  
};

#endif
