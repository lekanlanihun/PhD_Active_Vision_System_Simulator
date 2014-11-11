
#include "experiment.h"

/* ---------------------------------------------------------------------------------------- */

EXP_Class::EXP_Class( const char *run_name, bool _evolution, bool _viewing, bool _re_evaluation, unsigned long _seed ){
  int output_nodes;
  evolution     = _evolution;
  viewing       = _viewing;
  re_evaluation = _re_evaluation;
  
  param     = new Parameters ( run_name );
  if( evolution ) param->set_random_root_seed( _seed );
  param->init_random_generator( );

// cout << "Initialising local variables" << endl;
  init_local_variables();
  //image_path=new string[param->nets[0]->get_num_output()];
}

/* ---------------------------------------------------------------------------------------- */

EXP_Class::~EXP_Class( ){
      //  cout << "Deallocating exp";
       // cout.flush();
  delete param;
    //cout << "Deallocated exp";
    //cout.flush();
}

/* ---------------------------------------------------------------------------------------- */
// The eye is initialized here
void EXP_Class::init_local_variables( void ){
  //In this function you have to create your objects like agents,
  //sensors, obstables world characteristics etc.
  eval       = 0; //This is the counter for the evaluation
  iter       = 0; //This is the counter for the iterations
  genotype   = 0; //This is the counter for the solutions of genotype
 int bigRect = 50;
    int smallRect = 5;
  if( evolution ) param->dump_simulation_seed ( );//This is to dump the seed number in EXP/DATA
  
// cout << "Initialising eye" << endl;

  //This is the code to init the eye
  param->init_artificial_eye(bigRect,smallRect);

  //cout << "Initialising controllers" << endl;

  //This is to init the network controller
  param->init_controllers( );
  
  //inputs.resize(OUIPUT_SIZE);
  outputs.assign( param->nets[0]->get_num_output(), 0.0 );
  fitness.assign(param->num_evaluations,0.0);
  total_fitness.assign(1,0.0);
  param->eye->load_image( 0 );
  param->eye->Init_Eye_Position( );
  
}

/* ---------------------------------------------------------------------------------------- */

void EXP_Class::init_evolutionary_run( void ){
  generation = 0; //This is the counter for the generation
  //Parameters are: number of genes fopr each solution (genotype), and number of bases per gene
  //which is normally 1
  param->init_ga( param->nets[0]->get_genotype_length(), 1 );

  //cout<<"Parameter net[0] returns genotype length="<< param->nets[0]->get_genotype_length()<<endl; //testing line
  //getchar();
}

/* ---------------------------------------------------------------------------------------- */

void EXP_Class::init_genotype_loop( ){
  genotype   = 0;//This is the counter for the solutions (genotype)
  generational_avg_fitness     = 0.0;
  generational_max_fitness     = 0.0;
  generational_min_fitness     = 0.0;

}

/* ---------------------------------------------------------------------------------------- */

void EXP_Class::init_evaluations_loop( ){
  //int x=0;int y=0;
   num_trials=0;
  eval = 0;//This is the counter for the evalutions
  //param->reset_seed( );
  total_fitness[0]=0.0;
  //final_fitness=0.0;
  //There are various thing you want to do here
  // eg: resettng the fitness to zero (const vector <double> & FINAL_FITNESS)
  // init the agent position, etc

  if( evolution ) {
    vector <chromosome_type> genes = param->ga->get_solution( genotype );
    //for(int r=0; r < param->num_agents; r++) {
    param->nets[0]->init(  genes );//This is where we create a network from solution num: genotype
    //}
  }
}

/* ---------------------------------------------------------------------------------------- */

void EXP_Class::init_single_evaluation( void ){
   //cout<<"begin single evaluation"<<endl;
    //num_trials=0;
  fitness[eval]=0.0;
  
  param->eye->load_image( eval ); 
  param->eye->Init_Eye_Position( );
  
  counter_for_out_of_image_view = 0;

  param->nets[0]->reset();
  inputs.erase(inputs.begin(), inputs.end() );
  for(int i = 0; i < outputs.size(); i++) outputs[i] = 0.0;
  //cout<<"end single eval"<<endl;
}

/* ---------------------------------------------------------------------------------------- */

void EXP_Class::from_genome_to_controllers( const char *str_source, const int which_genotype ){
  int gen_length = 0;
  vector <chromosome_type> genes;
  vector <chromosome_type> copy_genes;
  
  upload_parameters_from_file <chromosome_type> ( str_source, which_genotype, genes, &gen_length);
  copy_genes.assign( gen_length/param->num_agents, 0.0);
  
  for(int r=0; r < param->num_agents; r++) {
    param->nets[r]->set_genotype_length ( copy_genes.size() );
    param->nets[r]->init( genes );
  }
}

/* ---------------------------------------------------------------------------------------- */

void EXP_Class::adv ( void ){
 //cout << "enter advance" << endl;
  if( viewing ) stop_iterations_loop( );
  
  update_sensors( );//here you compute the input vector
  
  update_controllers ( ); //here you update the controllers and you generate a new output vector
  
  if( viewing ) param->eye->view_eye_movement();
  
  update_world(); //Here you call the set eye fucntion to upadte the eye position given the output vector

  //manage_collisions ( );

  compute_fitness();
  
  iter++;
  //cout << "leave advance" << endl;
}


/* ---------------------------------------------------------------------------------------- */

void EXP_Class::update_sensors( void ){
  //cout << "enter update sensor" << endl;
  //HERE YOU COMPUTE THE INPUT VECTOR [from 0 to 49]
  //HERE YOU COPY THE OUTPUTS INTO THE LAST SEVENTH CELLS OF THE INPUT VECTOR []from 50 to 56]
  //Befire leaving the fucbntionyou set the outputs to zero.

 //param->eye->Peripheral_Features_Descriptor.erase(param->eye->Peripheral_Features_Descriptor.begin(),param->eye->Peripheral_Features_Descriptor.end() );
 inputs.erase(inputs.begin(), inputs.end() );
 //double * histogram=param->eye->Histogram_Oriented_Gradient();
  //double** fovea = param->eye->Get_Fovea();
  //double** peripheral = param->eye->Get_Peripheral();

 double ** grayscale_peripheral=param->eye->Gray_Scale_Processing();
 //int** segmented_peripherals=param->eye->Get_Peripheral_Segmented();
 int counter=0;
  //from input 0 to input 25 are the periphery
  for(int i=0;i<5;i++){
  for(int j=0;j<5;j++){
  //for(int i=0;i<4;i++){
 //for(int j=0;j<59;j++){
     //inputs.push_back(histogram[i]);
     inputs.push_back(grayscale_peripheral[i][j]);
          //cout<<"Input="<<""<<inputs[counter++]<<endl;
        //  getchar();
//}

 //}

    }
 }
//
  //from 25 to inout 49 are the fovea
   /*for(int i=0;i<5;i++){
    for(int j=0;j<5;j++){
       inputs.push_back(fovea[i][j]);
     }
   }*/

  //from input 50 to input 56 are copy of previous network output.

  for(int i=0;i<outputs.size();i++){
    inputs.push_back( outputs[i] );  // input the efference copy of movement and categorization unit into the network
  }


  for(int i = 0; i < outputs.size(); i++) outputs[i] = 0.0;
  
  
  //Check if the periphery is out of image
  counter_for_out_of_image_view++;
  //for(int i = 0; i <50; i++){
   // for(int j=0;j<50;j++){
     for(int i=0;i<25;i++){
          //cout<<"segmented_peripherals["<<i<<"]["<<j<<"]"<<segmented_peripherals[i][j]<<endl;
         //if(segmented_peripherals[i][j]!=0){
     if( inputs[i] > 0.05 ){//if any input is different from white then ....
      //if( inputs[i]!=1 ){
      counter_for_out_of_image_view = 0;
      break;
        }
  }
          
// }
        //}
    // }
     // cout<<" "<<endl;
  // }
  //}
 // cout<< "counter_for_out_of_image_view="<< counter_for_out_of_image_view<<endl;
  if( counter_for_out_of_image_view == MAX_STEPS_OUT_OF_IMAGES ){
    iter = param->num_iterations;
      //cout<< "iteration in if cond="<<iter<<endl;
    counter_for_out_of_image_view = 0;
  }
  // cout<< "iteration in out if cond="<<iter<<endl;

   // cout << "leave update sensor" << endl;
}

/* ---------------------------------------------------------------------------------------- */

void EXP_Class::update_controllers(void){
    int temp_eval;
    int hold_eval=0;
        int count=0;
//  cout << "enter update_controllers" << endl;
  param->nets[0]->step(inputs, outputs); // assign input values into the network and gives out output
  /*for(int i=0;i<inputs.size();i++){
      cout<<"input"<<i<<""<<"="<<inputs[i]<<endl;
  }
cout<<" "<<endl;
  for(int i=0;i<outputs.size();i++){
      cout<<"output"<<i<<""<<"="<<outputs[i]<<endl;
  }*/


  if(viewing){
      for(int i=0;i<1;i++){
      if(eval>8 && iter==0){
          num_trials=num_trials+10;
          break;
      }

        }

       cout<<"total number of trials="<<" "<<num_trials<<endl;

      int category;
           //temp_eval=eval%5;
            category=eval%5;
           // cout<<"category="<<" "<<category<<endl;
          if((iter>=50) && (iter <100)){
    for(int i=2;i<outputs.size();i++){
      cout<<"iterate="<<""<<iter<<" "<<"category"<<category<<" "<<"output["<<i<<"]= " <<" "<<outputs[i]<<endl;
    }
    cout<<" "<<endl;

          }

  }

 // cout << "leave update_controllers" << endl;


}

/* ---------------------------------------------------------------------------------------- */
//Set the eye movement at every iteration

void EXP_Class::update_world(){
  //cout << "enter update_world" << endl;
  param->eye->Set_Eye_Movement(outputs);
 // cout << "leave update_world" << endl;

}

/* ---------------------------------------------------------------------------------------- */

void EXP_Class::manage_collisions (void ){
  
}

/* ---------------------------------------------------------------------------------------- */


void EXP_Class::compute_fitness( void ){
//cout << "enter compute fitness" << endl;

  if((iter>=ITER_START_FITNESS_COUNT) && (iter<100)){
    int rank=0;    
    vector<double> outputs_cpy;
    vector<double> rank_index;
    
    outputs_cpy.assign(5,0);
    rank_index.assign(5,0);
    
    for(int i=2; i<outputs.size();i++){
      rank_index[i-2]=(i-2);
      outputs_cpy[i-2]=outputs[i];
    }
    
    double temp_value;
    for(int i=0;i<outputs_cpy.size();i++){
      for(int j=0;j<outputs_cpy.size()-1;j++){
	if(outputs_cpy[j] > outputs_cpy[j+1]){
	  temp_value=outputs_cpy[j];
	  outputs_cpy[j]=outputs_cpy[j+1];
	  outputs_cpy[j+1]=temp_value;
	  temp_value=rank_index[j];
	  rank_index[j]=rank_index[j+1];
	  rank_index[j+1]=temp_value;
	}
      }
    }
    
    for(int i=0;i<rank_index.size();i++){
      if(rank_index[i]==(eval%5)){
	rank=(4-i);
	break;
      }
    }

    double fit_temp=0.5*(1.0/pow(2,rank));
    double sum_out=0.0;

    vector<double> outputs_cpy_2;
    outputs_cpy_2.assign(5,0);
       for(int i=2; i<outputs.size();i++){
         outputs_cpy_2[i-2]=outputs[i];
       }

    for(int i=0;i<5;i++){
      if(i!=(eval%5)){
    sum_out+=(1.0-outputs_cpy_2[i])*0.5/4.0;
      }
    }
    //sum_out*=(0.5/4.0);
    
    fit_temp+= ((sum_out + (outputs_cpy_2[(eval%5)]*0.5)) *0.5);

    
    fitness[eval]+=fit_temp;

    //cout<<"Fitness= "<<fitness<<endl;
  }
//cout << "leave compute fitness" << endl;

}

/* ---------------------------------------------------------------------------------------- */

void EXP_Class::finalise_single_evaluation ( void ){
  
  total_fitness[0]+=fitness[eval]/(double)(param->num_iterations-ITER_START_FITNESS_COUNT);

 // getchar();
  
}

/* ---------------------------------------------------------------------------------------- */

void EXP_Class::finalise_evaluations_loop( void ){

  total_fitness[0]/=(double)(param->num_evaluations);
  generational_avg_fitness += total_fitness[0]/*FINAL_FITNESS*/;
  if( genotype == 0 ){
    generational_max_fitness     = total_fitness[0]/*FINAL_FITNESS*/;
    generational_min_fitness     = total_fitness[0]/*FINAL_FITNESS*/;
  }
  else{
    if( total_fitness[0] > generational_max_fitness ){
      generational_max_fitness     = total_fitness[0] /*FINAL_FITNESS*/;
    }
    else if(total_fitness[0] < generational_min_fitness )
      generational_min_fitness = total_fitness[0] /*FINAL_FITNESS*/;
  }

  //for(int i=0;i<total_fitness.size();i++){
     // cout<<"total_fitness"<<i<<""<<"="<<total_fitness[i]<<endl;
  //}

  
}

/* ---------------------------------------------------------------------------------------- */

void EXP_Class::assign_fitness_to_genotype( void ){
  
  param->ga->assign_fitness( genotype,total_fitness /* const vector <double> & FINAL_FITNESS */ );
  
}

/* ---------------------------------------------------------------------------------------- */

void EXP_Class::finalise_genotypes_loop( void ){

  if( !(generation%param->dump_stats_every) )
    dump_statistics( param->statsFileDir, param->statsFileName, generation);
  if( !(generation%param->dump_genome_every) )
    param->ga->dump_genome_into_file( param->genomeFileDir,  param->genomeFileName, generation );
  
  //This is the function that create s a new generation of solutions (genotypes)
  param->ga->breeding();
}

/* ---------------------------------------------------------------------------------------- */

void EXP_Class::finalise_evolutionary_run( void ){}

/* ---------------------------------------------------------------------------------------- */

bool EXP_Class::stop_iterations_loop( void ){
  //This function should not be changed

  if(iter >= param->num_iterations){
    finalise_single_evaluation( );
    iter = 0;
    if( viewing ) stop_evaluations_loop( );
    return false;
  }
  else {
    return true;
  }

}

/* ---------------------------------------------------------------------------------------- */

bool EXP_Class::stop_evaluations_loop( void ){

  eval++;
  if( evolution ){ 
    if( eval >= param->num_evaluations ) {
      eval = 0;
      finalise_evaluations_loop( );
      assign_fitness_to_genotype();
      return false;
    }
    else{
      return true;
    }
  }
  else{
    if( viewing ) {
      if( eval >= param->num_evaluations ) {
	eval = 0;
	finalise_evaluations_loop( );
      }
      init_single_evaluation( );
      return true;
    }
    /*
      else if( re_evaluation ){
      if( eval >= param->num_evaluations ) {
      eval = 0;	
      finalise_evaluations_loop( );
      return false;
      }
      else{
      return true;
      }
      }
    */
  }
}

/* ---------------------------------------------------------------------------------------- */

bool EXP_Class::stop_genotypes_loop( void ){

  genotype++;
  if( genotype >= param->ga->get_num_tot_solutions() ) {
    finalise_genotypes_loop( );
    genotype = 0;
    return false;
  }
  else
    return true;
}

/* ---------------------------------------------------------------------------------------- */

bool EXP_Class::stop_generations_loop( void ){

  generation++;
  if( generation >= param->num_generations ) {
    finalise_evolutionary_run();
    generation = 0;

    return false;
  }
  else {

    return true;
  }
}

/* ---------------------------------------------------------------------------------------- */

void EXP_Class::dump_statistics( const char *locationOfFileTodump, 
				      const char *fileName, int generation ) {
  char fname_statistics[500];
  sprintf(fname_statistics, "%s%s.data", locationOfFileTodump, fileName ); 
  ofstream outfile;
  outfile.open (fname_statistics, ios::app);
  outfile.setf(ios::fixed);
  
  outfile << " " << generation
   	  << " " << generational_max_fitness                                       //the fitness of the best
   	  << " " << (generational_avg_fitness)/(double)(param->ga->get_num_tot_solutions() )  // the mean fitness
	  << " " << generational_min_fitness                                       //the fitness of the worst
      	  << endl;
  outfile.close();
}

/* ---------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------ */
/* ---------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------- */




