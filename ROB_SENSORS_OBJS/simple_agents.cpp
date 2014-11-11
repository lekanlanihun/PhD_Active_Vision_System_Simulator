#include "simple_agents.h"

/* ----------------------------------------------------------------------- */
/*                              CONSTRUCTOR                                */
/* ----------------------------------------------------------------------- */
SIMPLE_Agents::SIMPLE_Agents( double simulation_time_step ) : World_Entity ( ) {
  pos.assign(3, 0.0);
  dim.assign(3, 0.0);
  previous_pos.assign(3, 0.0);
  tmp_pos.assign(3, 0.0);
  rot.assign(3, 0.0);
  previous_rot.assign(3, 0.0);
  vel.assign(2, 0.0);
  long_term.assign(3, 0.0);
  medium_term.assign(3, 0.0);
  colour.assign(3, 0.0);
  updates_per_second   = 1.0/simulation_time_step;
  frozen       = false;
  
  dim[0] = robot_radius;
  dim[1] = robot_height;
  dim[2] = 0.0;
}

/* ----------------------------------------------------------------------- */
/*                             END CONSTRUCTOR                             */
/* ----------------------------------------------------------------------- */
SIMPLE_Agents::~SIMPLE_Agents( ){ }


/* ----------------------------------------------------------------------- */
/*                             SET AGENT POSITION                          */
/* ----------------------------------------------------------------------- */
void SIMPLE_Agents::set_pos( const vector <double> &_pos ){
  for (int i = 0; i < _pos.size(); i++) {
    previous_pos[i] = _pos[i];
    pos[i]          = _pos[i];
  }
}

/* ----------------------------------------------------------------------- */
/*                             SET AGENT ROTATION                          */
/* ----------------------------------------------------------------------- */
void SIMPLE_Agents::set_rot( const vector <double> &_rot ){
  for (int i = 0; i < _rot.size(); i++) {
    previous_rot[i] = _rot[i];
    rot[i]          = _rot[i];
  }
}

/* ----------------------------------------------------------------------- */
/*                            SET AGENT VELOCITY                           */
/* ----------------------------------------------------------------------- */
void SIMPLE_Agents::set_vel( const vector <double> &outputs ){
  right_out = (outputs[0]-outputs[1]);
  left_out  = (outputs[2]-outputs[3]);
  
  vel[0] = (right_out*max_vel) + (((2.0*gsl_rng_uniform_pos (GSL_randon_generator::r_rand)*vel_noise) - vel_noise)  * World_Entity::noise_level );
  if( vel[0] > max_vel ) vel[0] = max_vel;
  else if( vel[0] < min_vel ) vel[0] = min_vel;
  
  vel[1] = (left_out*max_vel) + (((2.0*gsl_rng_uniform_pos (GSL_randon_generator::r_rand)*vel_noise) - vel_noise)  * World_Entity::noise_level );
  if( vel[1] > max_vel ) vel[1] = max_vel;
  else if( vel[1] < min_vel ) vel[1] = min_vel;

}

/* ----------------------------------------------------------------------- */
/*                     UPDATE POSITION ROTATION                            */
/* ----------------------------------------------------------------------- */
void SIMPLE_Agents::update_pos_rot( void ){

  //store previous pos and rot
  for (int i = 0; i < pos.size(); i++) previous_pos[i] = pos[i];
  for (int i = 0; i < rot.size(); i++) previous_rot[i] = rot[i];
  
  //update rotation considering noise
  rot[2] += (vel[0] - vel[1])*cm_per_pulse/(updates_per_second*wheels_distance);
  rot[2] += ((pinkq(2)*2.0*rot_noise-rot_noise)  * World_Entity::noise_level );
  if(rot[2] > (TWO_PI) ) rot[2] -= TWO_PI;
  else if( rot[2] < 0.0 ) rot[2] += TWO_PI;

  //update position considering noise
  double av_speed = (vel[0] + vel[1])*0.5;
  int look_up_angle = (int)(rot[2]/TWO_PI*35.0+0.5);
  
  pos[0] += cos(rot[2])*av_speed*cm_per_pulse/updates_per_second;
  pos[0] += ((pinkq(0)*2.0*pos_noise-pos_noise)  * World_Entity::noise_level);
  pos[1] += sin(rot[2])*av_speed*cm_per_pulse/updates_per_second;
  pos[1] += ((pinkq(1)*2.0*pos_noise-pos_noise)  * World_Entity::noise_level);
  
  clear_var_before_collision_check( );
}

/* ----------------------------------------------------------------------- */
/*                              CHECK COLLISIONS                           */
/* ----------------------------------------------------------------------- */

void SIMPLE_Agents::clear_var_before_collision_check( void ){
  //This is for collision detection and repositioning
  for (int i = 0; i < pos.size(); i++) tmp_pos[i] = pos[i];
  crashed                   = false;
  centreDisplacement[0]     = 0.0;
  centreDisplacement[1]     = 0.0;
  displacement              = 0.0;
  direction_of_displacement = 0.0;
}

/* ----------------------------------------------------------------------- */

bool SIMPLE_Agents::check_collision_with_round_object ( World_Entity  *target ){
  double x_offset        = (target->get_tmp_pos()[0] - tmp_pos[0]);
  double x_offset_square = (x_offset*x_offset); 
  double y_offset        = (target->get_tmp_pos()[1] - tmp_pos[1]);
  double y_offset_square = (y_offset*y_offset); 
  double sumRadii        = robot_radius + target->get_dim()[0];
  double sumRadii_square = (sumRadii+0.2) * (sumRadii+0.2);

  if( (x_offset_square + y_offset_square) < sumRadii_square ){
    crashed = true;
    double theta = atan2(y_offset, x_offset);
    if( theta < -EPSILON ) theta += TWO_PI;
    else if ( theta < EPSILON ) theta = 0.0;

    double dist = sqrt(x_offset_square + y_offset_square);
    centreDisplacement[0] += dist * cos( theta );
    centreDisplacement[1] += dist * sin( theta );
  }
  return crashed;
}

/* ----------------------------------------------------------------------- */

void SIMPLE_Agents::update_direction_of_displacement( void ){
  
  direction_of_displacement = atan2(centreDisplacement[1], centreDisplacement[0]);
  direction_of_displacement -= PI;
  if( direction_of_displacement < -EPSILON ) direction_of_displacement += TWO_PI;
  else if ( direction_of_displacement < EPSILON ) direction_of_displacement = 0.0;
  
  displacement = sqrt( (centreDisplacement[1]*centreDisplacement[1]) + (centreDisplacement[0]*centreDisplacement[0]) );
  if( displacement > half_robot_radius ) displacement = half_robot_radius;

}

/* ----------------------------------------------------------------------- */

void SIMPLE_Agents::update_displacement_round_obj( World_Entity  *target ){
  
  double x_offset        = (target->get_tmp_pos()[0] - tmp_pos[0]);
  double x_offset_square = (x_offset*x_offset); 
  double y_offset        = (target->get_tmp_pos()[1] - tmp_pos[1]);
  double y_offset_square = (y_offset*y_offset); 
  double sumRadii        = robot_radius + target->get_dim()[0];
  
  double dist_to_robot = sqrt(x_offset_square + y_offset_square) - sumRadii;
  if( dist_to_robot < displacement && dist_to_robot > 0.0 ) {
    double theta = atan2(y_offset, x_offset);
    if( theta < 0.0 ) theta += TWO_PI;
    if( theta < (direction_of_displacement + (PI*0.5)) && theta > (direction_of_displacement - (PI*0.5)) ) {
      if( dist_to_robot < displacement ) displacement = dist_to_robot;
    }
  }
}

/* ----------------------------------------------------------------------- */

void SIMPLE_Agents::update_tmp_position_after_collisions(){
  centreDisplacement[0] = 0.0;
  centreDisplacement[1] = 0.0;
  tmp_pos[0] += displacement * cos (direction_of_displacement );
  tmp_pos[1] += displacement * sin (direction_of_displacement );
  crashed = false;
}

/* ----------------------------------------------------------------------- */

void SIMPLE_Agents::update_position_after_collisions(){
  for (int i = 0; i < pos.size(); i++) pos[i] = tmp_pos[i];
}

/* ----------------------------------------------------------------------- */

void SIMPLE_Agents::return_to_previous_position( void ){
  
  for (int i = 0; i < pos.size(); i++)
    pos[i] = previous_pos[i];
  for (int i = 0; i < rot.size(); i++)
    rot[i] = previous_rot[i];

}

/* ----------------------------------------------------------------------- */
/*                                PINK NOISE                               */
/* ----------------------------------------------------------------------- */
double SIMPLE_Agents::pinkq( int index ){
  if(medium_term[index] == 0.0 ) 
    medium_term[index] = gsl_rng_uniform_pos(GSL_randon_generator::r_rand);
  else if( gsl_rng_uniform_pos(GSL_randon_generator::r_rand) < prob_medium_term_change )
    medium_term[index] = gsl_rng_uniform_pos(GSL_randon_generator::r_rand);
  
  if(long_term[index] == 0.0 ) 
    long_term[index] = gsl_rng_uniform_pos(GSL_randon_generator::r_rand);
  else if( gsl_rng_uniform_pos(GSL_randon_generator::r_rand) < prob_long_term_change )
    long_term[index] = gsl_rng_uniform_pos(GSL_randon_generator::r_rand);
  
  return (long_term[index] + medium_term[index] + gsl_rng_uniform_pos(GSL_randon_generator::r_rand) )/3.0;
}

#ifdef _GRAPHICS_
/* ----------------------------------------------------------------------- */
/*                    openGL RENDERING FUNCTIONS                           */
/* ----------------------------------------------------------------------- */
void SIMPLE_Agents::render( void ){
  glPushMatrix();
  glTranslated(pos[0], pos[1], pos[2]);
  glRotatef((rot[2]*360.0)/6.28, 0, 0, 1);
  GLUquadric* params_quadric = gluNewQuadric();
  glColor3f(colour[0], colour[1], colour[2] );
  gluCylinder(params_quadric, robot_radius, robot_radius, robot_height, robot_slices, 1);
  gluDisk(params_quadric, 0, robot_radius, robot_slices, 1);
  glTranslated(0,0,robot_height);
  gluDisk(params_quadric, 0, robot_radius, robot_slices, 1);
  gluDeleteQuadric(params_quadric);

  glTranslated(1.3,0,0);
  vector <double> lengths;
  lengths.assign(3, 0.0);
  lengths[0] = 1.0;
  lengths[1] = 0.5;
  lengths[2] = 1.0;
  glColor3f(0.0, 0.0, 0.0);
  glScalef(lengths[0], lengths[1], lengths[2]);
  glutSolidCube(1.0f);
  glPopMatrix();
}
#endif
