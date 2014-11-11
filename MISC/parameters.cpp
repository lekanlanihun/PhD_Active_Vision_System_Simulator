#include "parameters.h"

/* ---------------------------------------- */

void Parameters::read_run_parameter_file( const char *run_name ){
  
  ifstream I ("../MISC/init_run.txt");
  if(!I) {
    cerr << "File for Run Parameters not found" <<endl;
    exit(0);
  }
  
  /* ------------------------------- */
  //Load parameters from init_run.txt
  /* ------------------------------- */
  root_seed         = getInt('=', I);
  num_generations   = getInt('=', I);
  num_evaluations   = getInt('=', I);
  num_iterations    = getInt('=', I);
  num_agents        = getInt('=', I);
  
  dump_stats_every  = getInt('=', I);
  dump_genome_every = getInt('=', I);
  
  getStr('=', I, statsFileDir );
  getStr('=', I, genomeFileDir );
  getStr('=', I, type_of_controller );
  getStr('=', I, type_of_ga );
  
  simulation_time_step = getDouble('=', I);

  num_bricks        = getInt('=', I);
  num_cylinders     = getInt('=', I); 
  num_spheres       = getInt('=', I);
  num_lights        = getInt('=', I);
  num_objects       = num_bricks + num_cylinders +  num_spheres + num_lights;

  int num_obj_properties = getInt('=', I);

  if ( num_objects ){
    objects_data = new double*[num_objects];
    
    for(int b=0; b < num_objects; b++){
      objects_data[b] = new double[num_obj_properties];
    }
    
    for(int c=0; c < num_obj_properties; c++) {
      for(int b=0; b < num_objects; b++) {
  	objects_data[b][c] = getDouble('=', I);
      }
    }
  }
  /* ------------------------------- */
  //End of loading parameters
  /* ------------------------------- */
  
  strcpy(statsFileName, run_name);
  strcpy(rootSeedFileDir, statsFileDir );
  strcpy(rootSeedFileName, run_name );
  strcpy(genomeFileName, run_name);  
}


/* ---------------------------------------- */

Parameters::Parameters( const char *_run_name_ ){
  root_seed = 0;
  run_name = new char[500];
  sprintf(run_name, "%s", _run_name_); 
  read_run_parameter_file( run_name );
}

/* ---------------------------------------- */

Parameters::~Parameters(){
    //cout << "Deleting parameters 1" << endl;
    //cout.flush();
  //Delete run_name array
  delete[] run_name;
  
//cout << "Deleting parameters 2" << endl;
//	cout.flush();
  //Delete network objects
  for(int r=0; r < num_agents; r++)
    delete nets[r];

//cout << "Deleting parameters 3" << endl;
    //cout.flush();
  delete[] nets;
  
//cout << "Deleting parameters 4" << endl;
    //cout.flush();
  //Delete ga object
  delete ga;
  
//cout << "Deleting parameters 5" << endl;
    //cout.flush();
/*  //Delete agents objects
  for (int i = 0; i < num_agents; i++){
    delete agent[i];
  }
cout << "Deleting parameters 6" << endl;
	cout.flush();
  delete[] agent;
  
	cout << "Deleting parameters 6" << endl;
cout.flush();
  //Delete ir sensor object  
  delete ir_sensor;

  //Delete ambient light sensor object
  delete al_sensor;
  
  //Delete visul camera object
  delete camera;
  */
//cout << "Deleting parameters 6" << endl;
//cout.flush();
  //Delete object_data
  for(int b=0; b < num_objects; b++) 
    delete[] objects_data[b];
  delete[] objects_data;
  
/*
cout << "Deleting parameters 7" << endl;
cout.flush();
  //Delete object object
  for (int i = 0; i < num_objects; i++)
    delete object[i];
  delete[] object;
*/
    //cout << "Deleting parameters 8" << endl;
//cout.flush();

  
  GSL_randon_generator::free_generator( );
  
}

/* ---------------------------------------- */

void Parameters::init_random_generator( void ){
  GSL_randon_generator::init_generator( root_seed );
}

/* ---------------------------------------- */

void Parameters::init_objects( ){
  if( num_objects ){
    object = new SIMPLE_Objects*[num_objects];
    
    int count = 0, ob = 0;
    while( count < num_bricks && ob < num_objects ) {
      object[ob] = new SIMPLE_Brick( objects_data[ob] );
      ob++;
      count++;
    }
    count = 0;
    while( count < num_cylinders && ob < num_objects ) {
      object[ob] = new SIMPLE_Cylinder ( objects_data[ob] );
      ob++;
      count++;
    }
    count = 0;
    while( count < num_spheres && ob < num_objects ) {
      object[ob] = new SIMPLE_Sphere ( objects_data[ob] );
      ob++;
      count++;
    }
    count = 0;
    while( count < num_lights && ob < num_objects ) {
      object[ob] = new SIMPLE_Light ( objects_data[ob] );
      ob++;
      count++;
    }
  }
}
/* ---------------------------------------- */

void Parameters::init_artificial_eye(int bigRect,int smallRect){

  //image = imread("/home/oal/Snapshot_20130913_6.JPG", CV_LOAD_IMAGE_COLOR);
  eye = new Artificial_Eye (bigRect, smallRect );

}

/* ---------------------------------------- */



/* ---------------------------------------- */

/*--------------------------------------------*/
void Parameters::init_agents( void ){
  vector <double> colour;
  colour.assign(3, 1.0);
  agent = new SIMPLE_Agents*[num_agents];
  for (int r = 0; r < num_agents; r++){
    agent[r] = new SIMPLE_Agents( simulation_time_step );
    colour[0] = 0.5;
    colour[1] = 0.5;
    colour[2] = 1.0;
    agent[r]->set_colour( colour );
  }
  
  init_ir_sensor( );
  init_al_sensor( );
  init_linear_camera( );

}

/* ---------------------------------------- */

void Parameters::init_ir_sensor( void ){
  ir_sensor = new IR_Sensors();
}

/* ---------------------------------------- */

void Parameters::init_al_sensor( void ){
  al_sensor = new AL_Sensors();
}

/* ---------------------------------------- */

void Parameters::init_linear_camera( void ){
                          //depth, num_pixel, pixel dimension, num_objects
  camera = new Linear_Camera(150.0, 3, (PI/18.0)*2.0, num_agents, num_objects);
}

/* ---------------------------------------- */

void Parameters::init_controllers( void ){
  nets = new Controller* [num_agents];
  if ( !strcmp ( type_of_controller, "ctrnn_three_layers" ) ){
    for(int r=0; r < num_agents; r++) nets[r] = new Ctrnn3Layers(  );
  }
  else if ( !strcmp ( type_of_controller, "simple_three_layers" ) ){ 
    for(int r=0; r < num_agents; r++) nets[r] = new Simple3Layers(  );
  }
  else if ( !strcmp ( type_of_controller, "perceptron" ) ){ 
    for(int r=0; r < num_agents; r++) nets[r] = new Perceptron(  );
  }
  else if ( !strcmp ( type_of_controller, "elman" ) ){ 
    for(int r=0; r < num_agents; r++) nets[r] = new Elman(  );
  }
  else if ( !strcmp ( type_of_controller, "ctrnn_fully_recurrent" ) ){ 
    for(int r=0; r < num_agents; r++) nets[r] = new CtrnnFullyRecurrent (  );
  }
  else{
    cerr << " in Parameters::init_controllers - Controller type not found " << endl;
    exit(0);
  }
}

/* ---------------------------------------- */

void Parameters::init_ga( const int num_allele_per_solution, const int num_bases_per_allele ){  

  if ( !strcmp ( type_of_ga, "roulette_wheel" ) ){
    ga = new Roulette_wheel ( num_allele_per_solution, num_bases_per_allele, num_agents );
    /*   
	 char file_dir[500];
	 sprintf(file_dir, "/home/elt7/Simulations/task_switch/EXP/GENOME/"); 
	 char file_name[500];
	 sprintf(file_name, "H"); 
	 ga->upload_genome_from_file( file_dir, file_name, 0 );
    */

  }
  else if ( !strcmp ( type_of_ga, "tournament" ) ){
    ga = new Tournament ( num_allele_per_solution, num_bases_per_allele, num_agents );
  }
  else if ( !strcmp ( type_of_ga, "swarm_ga" ) ){
    ga = new Swarm_ga ( num_allele_per_solution, num_bases_per_allele, num_agents );
  }
  else{
    cerr << " in Parameters::init_controllers - GA type not found " << endl;
    exit(0);
  }
}

/* ---------------------------------------- */

void Parameters::dump_simulation_seed( void ){
  char fileName[500];
  sprintf(fileName, "%s%s.seed", rootSeedFileDir, rootSeedFileName);
  ofstream outfile ( fileName, ios::app);
  outfile.setf(ios::fixed);
  outfile << "ROOT_SEED=" << root_seed << endl; 
  outfile.close();
}

/* ---------------------------------------- */

void Parameters::reset_seed( void ){
  GSL_randon_generator::reset_seed( root_seed );
}

/* ---------------------------------------- */
