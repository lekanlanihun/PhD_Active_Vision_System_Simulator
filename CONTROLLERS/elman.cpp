#include "elman.h"

/* -------------------------------------------------------------------------------------- */

void  Elman::read_from_file ( void ) { 
  ifstream I ("../CONTROLLERS/elman");
  if(!I) {
    cerr << "File with network structure not found" <<endl;
    exit(0);  
  }
  
  num_input               = getInt      ('=', I);
  num_hidden              = getInt      ('=', I);
  num_output              = getInt      ('=', I);
  low_bound_Wts           = getDouble   ('=', I);
  upper_bound_Wts         = getDouble   ('=', I);
  
  int check = getInt('=',I);
  I.close();
  if( check != 999 ){
    cerr << " In ../CONTROLLERS/elman specifications' file --- End check was not 999" << endl;
    exit(0);
  }
}

/* -------------------------------------------------------------------------------------- */

void Elman::compute_genotype_length ( void ){
  genotype_length = 0;
  //weights input-hidden
  for(int i = 0; i < num_input; i++) 
    for(int h = 0; h < num_hidden; h++) 
      genotype_length++;
  
  //weights hidden-hidden
  for(int h = 0; h < num_hidden; h++)
    for(int k = 0; k < num_hidden; k++)
      genotype_length++;

  //weights hidden-output
  for(int h = 0; h < num_hidden; h++) 
    for(int o = 0; o < num_output; o++) 
      genotype_length++;
  
  //weigth bias-hidden 
  for(int h = 0; h < num_hidden; h++) genotype_length++;
  
  //weigths bias-output
  for(int out = 0; out < num_output; out++) genotype_length++;
}

/* -------------------------------------------------------------------------------------- */

Elman::Elman( ) : Controller() {  
  read_from_file ( );
  compute_genotype_length();

  inputLayer.resize       ( num_input );
  hiddenLayer.resize      ( num_hidden );
  hiddenLayerCopy.resize  ( num_hidden );
  outputLayer.resize      ( num_output );
  biasLayer.resize        ( 1 );
  
  for( int i=0; i<num_input; i++) {
    inputLayer[i].weightsOut    = new double[num_hidden];
  }
  for( int i=0; i<num_hidden; i++) {
    hiddenLayerCopy[i].weightsOut   = new double[num_hidden];
  }
  for( int i=0; i<num_hidden; i++) {
    hiddenLayer[i].weightsOut   = new double[num_output];
  }
  for( int i=0; i<num_output; i++){
    outputLayer[i].weightsOut  = NULL;
  }
  biasLayer[0].weightsOut   = new double[num_hidden+num_output];
}

/* -------------------------------------------------------------------------------------- */

Elman::~Elman(){
  for(  int i=0; i<inputLayer.size(); i++) {
    delete[] inputLayer[i].weightsOut;
  }
  for(  int i=0; i<hiddenLayer.size(); i++){
    delete[] hiddenLayerCopy[i].weightsOut;
  }
  for(  int i=0; i<hiddenLayer.size(); i++){
    delete[] hiddenLayer[i].weightsOut;
  }
  for(  int i=0; i<outputLayer.size(); i++) {
    delete[] outputLayer[i].weightsOut;
  }
  delete[] biasLayer[0].weightsOut;
}

/* -------------------------------------------------------------------------------------- */

void Elman::init( const vector <chromosome_type> &genes ){
  int    counter     = 0;
  
  /* ------------------ INPUT_LAYER -------------------- */
  for( int i=0; i<num_input; i++){
    inputLayer[i].state        = 0.0;
    for(int j=0; j< num_hidden ; j++){
      inputLayer[i].weightsOut[j] = get_value(genes, counter)*(upper_bound_Wts - low_bound_Wts) + low_bound_Wts;
      counter++;
    }
  }
  
  /* ------------------ CONTROLLER::HIDDEN_LAYER -------------------- */
  for( int i=0; i<num_hidden; i++){
    hiddenLayerCopy[i].state = 0.0;
    for(int j=0; j<num_hidden; j++){
      hiddenLayerCopy[i].weightsOut[j] = get_value(genes, counter)*(upper_bound_Wts - low_bound_Wts) + low_bound_Wts;
      counter++;
    }
  }
  
  for( int i=0; i<num_hidden; i++){
    hiddenLayer[i].state        = 0.0;
    for(int j=0; j<num_output; j++){
      hiddenLayer[i].weightsOut[j] = get_value(genes, counter)*(upper_bound_Wts - low_bound_Wts) + low_bound_Wts;
      counter++;
    }
  }
  
  /* ------------------ CONTROLLER::OUTPUT_LAYER -------------------- */
  for(int i=0; i<num_output; i++)
    outputLayer[i].state         = 0.0;
  /* --------------------------------------------------------------- */

  /* ------------------ CONTROLLER::BIAS_LAYER -------------------- */
  biasLayer[0].state         = 1.0;
  for(int i=0; i<(num_hidden+num_output); i++){
    biasLayer[0].weightsOut[i] = get_value(genes, counter)*(upper_bound_Wts - low_bound_Wts) + low_bound_Wts;
    counter++;
  }
  /* --------------------------------------------------------------- */

  if( counter != genotype_length ){
    cerr << "'In elamn.cpp init():: the number of genes is wrong"
	 << " " << genotype_length << " " << counter << endl;
    exit(0);
  }
}

/* -------------------------------------------------------------------------------------- */

void Elman::step( const vector <double> &input, vector <double> &output ){
  for( int h=0; h<num_hidden; h++){
    hiddenLayer[h].state = biasLayer[0].weightsOut[h];
    for( int i=0; i<num_input; i++){
      hiddenLayer[h].state += input[i] * inputLayer[i].weightsOut[h];
    }
    for( int k=0; k<num_hidden; k++){
      hiddenLayer[h].state += hiddenLayerCopy[k].state * hiddenLayerCopy[k].weightsOut[h];
    }
    hiddenLayer[h].state = f_sigmoid( hiddenLayer[h].state );
  }
  
  for( int h=0; h<num_hidden; h++)
    hiddenLayerCopy[h].state = hiddenLayer[h].state;
  
  for(int out=0; out<num_output; out++){
    outputLayer[out].state = biasLayer[0].weightsOut[num_hidden+out];
    for( int h=0; h<num_hidden; h++){
      outputLayer[out].state  += hiddenLayer[h].state * hiddenLayer[h].weightsOut[out];
    }
    output[out] = f_sigmoid( outputLayer[out].state );
  }
}

/* -------------------------------------------------------------------------------------- */

void Elman::reset ( void ){
  for(int i=0; i<num_input; i++){
    inputLayer[i].state = 0.0;
  }
  for(int i=0; i<num_hidden; i++){
    hiddenLayer[i].state = 0.0;
    hiddenLayerCopy[i].state = 0.0;
  }
  for(int i=0; i<num_output; i++){
    outputLayer[i].state = 0.0;
  }
}

/* -------------------------------------------------------------------------------------- */
