#ifndef _PARAMETERS_
#define _PARAMETERS_

#include "general.h"

#include "../EVOL_ALGORITHM/roulette_wheel.h"
#include "../EVOL_ALGORITHM/tournament.h"
#include "../EVOL_ALGORITHM/swarm_ga.h"

#include "../CONTROLLERS/controller.h"
#include "../CONTROLLERS/ctrnn3Layers.h"
#include "../CONTROLLERS/ctrnnFullyRecurrent.h"
#include "../CONTROLLERS/perceptron.h"
#include "../CONTROLLERS/simple3Layers.h"
#include "../CONTROLLERS/elman.h"

#include "../ROB_SENSORS_OBJS/world_entity.h"
#include "../ROB_SENSORS_OBJS/simple_agents.h"
#include "../ROB_SENSORS_OBJS/simple_objects.h"
#include "../ROB_SENSORS_OBJS/ir_sensors.h"
#include "../ROB_SENSORS_OBJS/al_sensors.h"
#include "../ROB_SENSORS_OBJS/linear_camera.h"

#include "../ROB_SENSORS_OBJS/Artificial_Eye.h"


class Parameters {
  
 private:
  
 public:
  Parameters ( const char *run_name );
  ~Parameters( );
  void read_run_parameter_file( const char *run_name );
  void init_random_generator( void );
  void init_controllers( void );
  void init_ga( const int num_allele_per_solution, const int num_bases_per_allele );
  
  void init_artificial_eye(int,int);
  void init_eye_position(int,int);
  void set_eye_movement(int,int);
  void init_objects( void );
  void init_agents( void );
  void init_ir_sensor( void );
  void init_al_sensor( void );
  void init_linear_camera( void );
  
  void dump_simulation_seed( void );
  void reset_seed( void );
  
  inline void set_random_root_seed( unsigned long _seed ){ root_seed = _seed; }
  inline int get_random_root_seed( void ){ return root_seed; }
  inline char* get_run_name( void ){return run_name;}

  char *run_name;
  
  Artificial_Eye *eye;

  GA *ga;
  Controller **nets;  
  SIMPLE_Agents **agent;
  SIMPLE_Objects **object;
  IR_Sensors *ir_sensor;
  AL_Sensors *al_sensor;
  Linear_Camera *camera;
  
  unsigned long root_seed;
  
  int     num_generations;
  int     num_evaluations;
  int     num_iterations;
  int     num_agents;
  int     dump_stats_every;
  int     dump_genome_every;
  
  char    statsFileDir[400];
  char    statsFileName[400];
  char    genomeFileDir[400];
  char    genomeFileName[400];
  char    rootSeedFileDir[400];
  char    rootSeedFileName[400];
  char    type_of_ga[400];
  char    type_of_controller[400];
  
  double simulation_time_step;
  
  int num_bricks;
  int num_cylinders;
  int num_spheres;
  int num_objects;
  int num_lights;
  double** objects_data;


  
};
#endif
