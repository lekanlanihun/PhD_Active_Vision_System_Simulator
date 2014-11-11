#include "ctrnn3Layers.h"

/* -------------------------------------------------------------------------------------- */

void  Ctrnn3Layers::read_from_file ( void ) { 
  ifstream I ("../CONTROLLERS/ctrnn3Layers");
  if(!I) {
    cerr << "File with network structure not found" <<endl;
    exit(0);  
  }
  
  delta_t                 = getDouble   ('=', I);
  num_input               = getInt      ('=', I);
  num_hidden              = getInt      ('=', I);
  num_output              = getInt      ('=', I);
  
  low_bound_inputWts      = getDouble   ('=', I);
  upper_bound_inputWts    = getDouble   ('=', I);
  low_bound_inputBias     = getDouble   ('=', I);
  upper_bound_inputBias   = getDouble   ('=', I);
  
  low_bound_hiddenWts     = getDouble   ('=', I);
  upper_bound_hiddenWts   = getDouble   ('=', I);
  low_bound_hiddenTau     = getDouble   ('=', I);
  upper_bound_hiddenTau   = getDouble   ('=', I);
  low_bound_hiddenBias  = getDouble   ('=', I);
  upper_bound_hiddenBias  = getDouble   ('=', I);
  
  low_bound_outputBias    = getDouble   ('=', I);
  upper_bound_outputBias  = getDouble   ('=', I);
  
  low_bound_sensorsGain   = getDouble   ('=', I);
  upper_bound_sensorsGain = getDouble   ('=', I);
  
  int check = getInt('=',I);
  I.close();
  if( check != 999 ){
    cerr << " In ../CONTROLLERS/ctrnn3Layers specifications' file --- End check was not 999" << endl;
    exit(0);
  } 
}

/* -------------------------------------------------------------------------------------- */

void Ctrnn3Layers::compute_genotype_length ( void ){
    genotype_length = 0;
    //weights input-hidden
    for(int i = 0; i < num_input; i++){
        if(i<25){
            for(int o= 0; o <2; o++)
               genotype_length++;
        }
      for(int h = 0; h <num_hidden; h++){
              genotype_length++;
      }

  }

    //weights hidden-hidden
    for(int h = 0; h < num_hidden; h++)
      for(int hh = 0; hh < num_hidden; hh++)
        genotype_length++;

    //weights hidden-output
    for(int h = 0; h < num_hidden; h++)
      for(int o = 0; o < num_output; o++)
        genotype_length++;

    //tau - it only applies to hidden nodes
    for(int h = 0; h < num_hidden; h++) genotype_length++;

    //bias - it applies to hidden nodes plus a single bias for all input
    //nodes, and a single bias for all output nodes
    for(int h = 0; h < num_hidden+2; h++) genotype_length++;

    //sensor gain - there is only one single gain for all input nodes
    genotype_length++;
}

/* -------------------------------------------------------------------------------------- */

Ctrnn3Layers::Ctrnn3Layers( ) : Controller() {  
    // cout<<"Enter cttrn3layer Constructor"<<endl;
        read_from_file ( );
        compute_genotype_length ( );

        inputLayer.resize   ( num_input );
        hiddenLayer.resize  ( num_hidden );
        outputLayer.resize  ( num_output );

        for( int i=0; i<num_input; i++) {
            if(i<25){
                inputLayer[i].weightsOut    = new double[2];
            }
              inputLayer[i].weightsOut    = new double[num_hidden];
              inputLayer[i].weightsSelf   = NULL;
        }
        for( int i=0; i<num_hidden; i++) {
          hiddenLayer[i].weightsOut   = new double[num_output];
          hiddenLayer[i].weightsSelf  = new double[num_hidden];
        }
        for( int i=0; i<num_output; i++){
          outputLayer[i].weightsSelf  = NULL;
          outputLayer[i].weightsOut   = NULL;
        }
        sensorsGain           = new double[num_input];

       // cout<<"leave cttrn3layer Constructor"<<endl;
}

/* -------------------------------------------------------------------------------------- */

Ctrnn3Layers::~Ctrnn3Layers(){
  for(  int i=0; i<inputLayer.size(); i++) {
    delete[] inputLayer[i].weightsOut;
    delete[] inputLayer[i].weightsSelf;
  }
  for(  int i=0; i<hiddenLayer.size(); i++){
    delete[] hiddenLayer[i].weightsOut;
    delete[] hiddenLayer[i].weightsSelf;
  }
  for(  int i=0; i<outputLayer.size(); i++) {
    delete[] outputLayer[i].weightsOut;
    delete[] outputLayer[i].weightsSelf;
  }
  
  inputLayer.clear();
  hiddenLayer.clear();
  outputLayer.clear();

  delete[] sensorsGain;
}

/* -------------------------------------------------------------------------------------- */

void Ctrnn3Layers::init ( const vector <chromosome_type> &genes ){
    //cout<<"Enter Init cttrn3layer "<<endl;
      int    counter     = 0;
      double single_tau  = 0.0;
      double single_gain = 0.0;
      double single_bias = 0.0;

      /* ------------------ INPUT_LAYER -------------------- */
      //SINGLE TAU EQUAL TO DELTA_T
      single_tau = delta_t;

      // SINGLE BIAS FOR ALL INPUT
      single_bias = get_value(genes, counter)*(upper_bound_inputBias - low_bound_inputBias) + low_bound_inputBias;
      counter++;

      //SINGLE GAIN FOR ALL INPUT
      single_gain = get_value(genes, counter)*(upper_bound_sensorsGain - low_bound_sensorsGain) + low_bound_sensorsGain;
      counter++;

      for( int i=0; i<num_input; i++){
          if(i<25){
              for(int j=0;j<2;j++){
                  inputLayer[i].weightsOut[j] =  get_value(genes, counter)*(upper_bound_inputWts - low_bound_inputWts) + low_bound_inputWts;
                  counter++;
              }
          }
        for(int j=0; j< num_hidden; j++){
          inputLayer[i].weightsOut[j] =  get_value(genes, counter)*(upper_bound_inputWts - low_bound_inputWts) + low_bound_inputWts;
          counter++;
          }
        inputLayer[i].tau  = single_tau;
        inputLayer[i].bias = single_bias;
        sensorsGain[i]     = single_gain;
      }
      /* --------------------------------------------------------------- */

      /* ------------------ CONTROLLER::HIDDEN_LAYER -------------------- */
      for( int i=0; i<num_hidden; i++){
        for(int j=0; j<num_output; j++){
          hiddenLayer[i].weightsOut[j] = get_value(genes, counter)*(upper_bound_hiddenWts - low_bound_hiddenWts) + low_bound_hiddenWts;
          counter++;
        }
        hiddenLayer[i].tau         = pow(10, (low_bound_hiddenTau + (upper_bound_hiddenTau * get_value(genes, counter) )));
        counter++;
        hiddenLayer[i].bias        = get_value(genes, counter)*(upper_bound_hiddenBias - low_bound_hiddenBias) + low_bound_hiddenBias;
        counter++;
        for( int j=0; j<num_hidden; j++){
          hiddenLayer[i].weightsSelf[j] = get_value(genes, counter)*(upper_bound_hiddenWts - low_bound_hiddenWts) + low_bound_hiddenWts;
          counter++;
        }
      }
      /* ---------------------------------------------------------------- */

      /* ------------------ CONTROLLER::OUTPUT_LAYER -------------------- */
      //SINGLE TAU EQUAL TO DELTA_T
      single_tau           = delta_t;

      // SINGLE BIAS FOR ALL OUTPUT
      single_bias = get_value(genes, counter)*(upper_bound_outputBias - low_bound_outputBias) + low_bound_outputBias;
      counter++;

      for(int i=0; i<num_output; i++){
        outputLayer[i].tau  =  single_tau;
        outputLayer[i].bias =  single_bias;
      }
      /* --------------------------------------------------------------- */

      if( counter != genotype_length ){
        cerr << "'In ctrnn3Layers.cpp init():: the number of genes is wrong"
         << " " << genotype_length << " " << counter << endl;
        exit(0);
      }

}

/* -------------------------------------------------------------------------------------- */

void Ctrnn3Layers::step ( const vector <double> &inputs, vector <double> &outputs ){
    // cout<<"Enter Step in cttrn3layer "<<endl;
       for( int i=0; i < num_input; i++) {
         inputLayer[i].state = (inputs[i] * sensorsGain[i] );
       }

       update_hidden_layer();
       update_output_layer( );

       /* here we set the actuators state */
       for( int i = 0; i < num_output; i++ ){
         outputs[i] = f_sigmoid( (outputLayer[i].state + outputLayer[i].bias) );
       }
        //cout<<"leave Step in cttrn3layer "<<endl;
}

/* -------------------------------------------------------------------------------------- */

/* This function compute the activation of each hidden neurons */
void Ctrnn3Layers::update_hidden_layer( ){
    // cout<<"Enter Update_hidden_layer in cttrn3layer "<<endl;
        for(int i=0; i < num_hidden; i++) {
          hiddenLayer[i].s = -hiddenLayer[i].state;
          int from_afferent_node = 0;
          int to_afferent_node = num_input;//50;
          hiddenLayer[i].s += update_StimesW  ( i,from_afferent_node,to_afferent_node, inputLayer );
          hiddenLayer[i].s += update_StimesWself  ( i, hiddenLayer );
        }
        for(int i=0; i < num_hidden; i++)
          hiddenLayer[i].state += (hiddenLayer[i].s * (delta_t/hiddenLayer[i].tau));
        //cout<<"leave Update_hidden_layer in cttrn3layer "<<endl;

}

/* -------------------------------------------------------------------------------------- */

/* This function compute the activation of each output neurons taking
   into account the hidden layer */
void Ctrnn3Layers::update_output_layer ( ){
    //cout<<"Enter Update_output_layer in cttrn3layer "<<endl;

         for(int i=0; i < num_output; i++) {
           outputLayer[i].s = -outputLayer[i].state;
           int from_afferent_node_input = 0;// This determines the range of the periphery input values
           int to_afferent_node_input = 25;// from 0 to 236 etc.
           int from_afferent_node_hidden = 0;// This determines the range of the
           int to_afferent_node_hidden = num_hidden;//periphery hidden values
           //outputLayer[i].s += update_StimesW  ( i, from_afferent_node, to_afferent__node, inputLayer );
           if(i<2){
                outputLayer[i].s += update_StimesW( i,from_afferent_node_input,to_afferent_node_input ,inputLayer );
           }
                outputLayer[i].s += update_StimesW( i,from_afferent_node_hidden,to_afferent_node_hidden,hiddenLayer );
         }

         for(int i=0; i < num_output; i++)
           outputLayer[i].state += (outputLayer[i].s * (delta_t/outputLayer[i].tau));
          //cout<<"leave Update_output_layer in cttrn3layer "<<endl;
}

/* -------------------------------------------------------------------------------------- */



/* This function is used to multiply the firig rate 
   of the neurons (layer) with a connection to neuron j */
double Ctrnn3Layers::update_StimesW ( int j,int from_afferent_node,int to_afferent_node, vector<Node> &layer){
  double sum = 0.0;
  for( int i = from_afferent_node; i < to_afferent_node; i++ ) {
      double z = f_sigmoid ( ( layer[i].state + layer[i].bias ) );
      sum  +=  layer[i].weightsOut[j] * z;
  }
  return sum;
}

/* -------------------------------------------------------------------------------------- */

/* This function is used to multiply the firig rate 
   of the neurons (layer) with the self-connections to neuron j */
double Ctrnn3Layers::update_StimesWself ( int j,vector<Node> &layer ){
  double sum = 0.0;
  for( int i = 0; i < layer.size(); i++ ) {
    double z = f_sigmoid ( ( layer[i].state + layer[i].bias ) );
    sum  +=  layer[i].weightsSelf[j] * z;
  }
  return sum;
}

/* -------------------------------------------------------------------------------------- */

void Ctrnn3Layers::reset ( void ){
  for(int i=0; i<num_input; i++){
    inputLayer[i].state = 0.0;
    inputLayer[i].s = 0.0;
  }
  for(int i=0; i<num_hidden; i++){
    hiddenLayer[i].state = 0.0;
    hiddenLayer[i].s = 0.0;
  }
  for(int i=0; i<num_output; i++){
    outputLayer[i].state = 0.0;
    outputLayer[i].s = 0.0;
  }
}

/* -------------------------------------------------------------------------------------- */
