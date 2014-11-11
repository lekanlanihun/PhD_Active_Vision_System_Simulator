#include "ir_sensors.h"

/* ---------------------------------------------------------------------------------------- */
IR_Sensors::IR_Sensors( void ){
  // For Infra-red Sensors
  ir_num_sensors          = 0;
  ir_num_turns            = 0;
  ir_num_horizontal_steps = 0;
  ir_step_length          = 0;
  ir_min_distance         = 0;
  max_ir_reading          = 0;
  ir_readings_file_name   = new char[500];
  strcpy ( ir_readings_file_name, "../MISC/round.sam" );
  load_ir_readings( );
}

/* ---------------------------------------------------------------------------------------- */
IR_Sensors::~IR_Sensors( void ){
  delete[] ir_readings_file_name;
}

/* ---------------------------------------------------------------------------------------- */
void IR_Sensors::load_ir_readings ( void ){
  
  if( ir_readings_file_name != NULL ){
    ifstream inFile ( ir_readings_file_name );
    string s;
    int count_lines = 0, count_turns = 0, count_horizontal_steps = 0;
    
    while(getline(inFile, s)){
      istringstream ss(s);
      if(count_lines == 0){
	ss >> ir_num_sensors;
	ss >> ir_num_turns;
	ss >> ir_num_horizontal_steps;
	ss >> ir_min_distance;
	ss >> ir_step_length;
	
	ir_readings.resize(  ir_num_horizontal_steps );
	for(int i = 0; i <  ir_num_horizontal_steps; i++){
	  ir_readings[i].resize( ir_num_turns );
	  for(int j = 0; j < ir_num_turns; j++)
	    ir_readings[i][j].resize(  ir_num_sensors );
	}
	
	count_turns = 0;
	count_horizontal_steps = 0;
      }
      else{
	for(int n = 0; n < ir_num_sensors; n++){
	  ss >> ir_readings[count_horizontal_steps][count_turns][n];
	  if( ir_readings[count_horizontal_steps][count_turns][n] > max_ir_reading )
	    max_ir_reading = ir_readings[count_horizontal_steps][count_turns][n];
	}
	count_turns++;
	if( count_turns  == ir_num_turns ){
	  count_turns = 0;
	  count_horizontal_steps++;
	}
      }
      count_lines++;
    }
  }
  else{
    cerr << " In IR_Sensors::load_ir_readings - File name = " << ir_readings_file_name 
   	 << " NOT FOUND " << endl;
    exit(0);
  }
}

/* ---------------------------------------------------------------------------------------- */

void IR_Sensors::agent_round_obj_readings ( World_Entity *agent, World_Entity *target, vector <double> &readings){
  //Compute distance to target
  double x_offset = (target->get_pos()[0] - agent->get_pos()[0]);
  double y_offset = (target->get_pos()[1] - agent->get_pos()[1]);
  double remove = (agent->get_dim()[0]+target->get_dim()[0]);
  double target_distance = 10.0*(sqrt((x_offset*x_offset)+(y_offset*y_offset))-remove);
  int dist_entry;
  
  if(target_distance < ir_min_distance ) dist_entry = 0;
  else dist_entry = int((target_distance-ir_min_distance)/ir_step_length);
  
  if(dist_entry < ir_num_horizontal_steps){
    //Compute relative orientation Theta
    double theta = atan2( y_offset, x_offset);
    if( theta < 0.0 ) theta += TWO_PI;
    
    theta = agent->get_rot()[2] - theta;
    if(theta < 0.0 ) theta += TWO_PI;
    else if(theta > TWO_PI ) theta -= TWO_PI;
    
    int turn_entry = int(  (((theta*180.0)/PI)*0.5) );
    
    //Copy readings
    for(int i = 0; i < ir_num_sensors; i++){
      if(ir_readings[dist_entry][turn_entry][i]>readings[i]) readings[i] = ir_readings[dist_entry][turn_entry][i];
    }
  }
}

/* ---------------------------------------------------------------------------------------- */

void IR_Sensors::add_noise( vector <double> &readings ){
  
  for(int i = 0; i < ir_num_sensors; i++){
    if( readings[i] ){
      readings[i] += ((rint)(gsl_ran_gaussian(GSL_randon_generator::r_rand, 50)) * World_Entity::noise_level );
      if( readings[i] > max_ir_reading ) readings[i] = max_ir_reading;
      else if ( readings[i] < 0.0 ) readings[i] = 0.0;
      readings[i] /= (double)(max_ir_reading);
    }
    else{
      //just background noise
      readings[i] = (((double)(gsl_rng_uniform_int(GSL_randon_generator::r_rand, 21))) * World_Entity::noise_level );
      readings[i] /= (double)(max_ir_reading);
    }
  }
}
/* ---------------------------------------------------------------------------------------- */
