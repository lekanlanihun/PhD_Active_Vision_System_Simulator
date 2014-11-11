#ifndef _IR_SENSORS_
#define _IR_SENSORS_

#include "../MISC/general.h"
#include "world_entity.h"
#include "simple_agents.h"
#include "simple_objects.h"

using namespace std;

class IR_Sensors {
  
 private:
  int ir_num_sensors;
  int ir_num_turns;
  int ir_num_horizontal_steps;
  int ir_step_length;
  int ir_min_distance;
  char *ir_readings_file_name;
  vector <vector <vector <double> > > ir_readings;
  int max_ir_reading;
  
 public:
  IR_Sensors();
  ~IR_Sensors();
  
  void load_ir_readings           ( void );
  void agent_round_obj_readings   ( World_Entity *agent, World_Entity *target, vector <double> &readings);
  void add_noise                  ( vector <double> &readings );
  inline int get_ir_num_sensors( void ) { return ir_num_sensors;}

};

#endif

