#ifndef _EXP_
#define _EXP_

#include "../MISC/parameters.h"
//#include "../CONTROLLERS/controller.h"
#include <math.h>

#define MAX_STEPS_OUT_OF_IMAGES 3
#define ITER_START_FITNESS_COUNT 50

#ifdef _GRAPHICS_
#include "../EXP/RENDERING/engine.h"

#include <vector>

class EXP_Class : public Engine {
#endif
  
#ifndef _GRAPHICS_
  class EXP_Class {
#endif

 private:
  /* --------------- FIXED MEMBERS --------------- */
  int  generation;
  int  genotype;
  int  eval;
  int  iter;
 int num_trials;
  int nl;
// int bigRect=50;
  //int smallRect = 5;
  double top_final_fitness;
  double  top_trial_final_fitness;
  bool evolution;
  bool viewing;
  bool re_evaluation;

  double generational_avg_fitness;
  double generational_max_fitness;
  double generational_min_fitness;
  double final_fitness;
  vector<double> inputs;
//double *inputs;
  vector<double> outputs;
//double *outputs;
  vector<int>rank_index;
  vector<int>image_index;
//vector<string>image_path;
  string *image_path;
  double single_fitness;
  vector<double>sum_single_fitness;
vector<double>fitness;
vector<double>total_fitness;
  vector<double>genotypes_final_fitness;

  int counter_for_out_of_image_view;
  /* --------------------------------------------- */
  
 public:
  /* --------------- FIXED FUNCTIONS --------------- */
  EXP_Class ( const char *run_name, bool _evolution, bool _viewing, bool _re_evaluation, unsigned long _seed );
  ~EXP_Class( );
  
  Parameters *param;

  void init_local_variables( void );
  void init_evolutionary_run( void );
  void init_genotype_loop( );
  void init_evaluations_loop( void );
  void init_single_evaluation( void );
  void from_genome_to_controllers( const char *str_source, const int which_genotype );
  void adv ( void );
  void assign_fitness_to_genotype( void );

  void finalise_single_evaluation( void );
  void finalise_evaluations_loop( void );
  void finalise_genotypes_loop( void );
  void finalise_evolutionary_run( void );
  
  bool stop_iterations_loop( void );
  bool stop_evaluations_loop( void ); 
  bool stop_genotypes_loop( void ); 
  bool stop_generations_loop( void );
  
  void update_sensors( void );
  void update_controllers( void );
  void update_world( void );
  void manage_collisions ( void );
  void compute_fitness ( void ); 
  void dump_statistics              ( const char *locationOfFileTodump, const char *fileName, int generation );
  void init_eye_position();
  void set_eye_movement();
  void get_eye_movement();
  void track_image();
  /* ----------------------------------------------- */
  
};
#endif
