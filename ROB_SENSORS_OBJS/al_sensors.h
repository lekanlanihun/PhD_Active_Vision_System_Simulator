#ifndef _AL_SENSORS_
#define _AL_SENSORS_

#include <iomanip>
#include <fstream>
#include <sstream>

#include "world_entity.h"
#include "simple_agents.h"
#include "simple_objects.h"

using namespace std;

class AL_Sensors {
  
 private:
  int al_num_sensors;
  int al_num_turns;
  int al_num_horizontal_steps;
  int al_step_length;
  int al_min_distance;
  char *al_readings_file_name;
  vector <vector <vector <double> > > al_readings;
  int max_al_reading;
  
 public:
  AL_Sensors();
  ~AL_Sensors();
  
  void load_al_readings     ( void );
  void agent_light_readings ( World_Entity *agent, World_Entity *light_pos, vector <double> &readings );
  void add_noise( vector <double> &readings );
  inline int    get_al_num_sensors( void ) { return al_num_sensors;}  

};

#endif

