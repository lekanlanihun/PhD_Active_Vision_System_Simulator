#ifndef CTRNN_3_LAYERS_H
#define CTRNN_3_LAYERS_H

#include "controller.h"

class Ctrnn3Layers : public Controller {
 
 private:
  int num_hidden;

  typedef struct {
    double state;
    double *weightsOut;
    double *weightsSelf;
    double s;
    double tau;
    double bias;
    double gain;
  } Node;

  vector <Node> inputLayer;
  vector <Node> hiddenLayer;
  vector <Node> outputLayer;
 
  double low_bound_inputWts;
  double upper_bound_inputWts;
  double low_bound_inputBias;
  double upper_bound_inputBias;
  
  double low_bound_hiddenWts;
  double upper_bound_hiddenWts;
  double low_bound_hiddenTau;
  double upper_bound_hiddenTau;
  double low_bound_hiddenBias;
  double upper_bound_hiddenBias;
  
  double low_bound_outputBias;
  double upper_bound_outputBias;
  
  double low_bound_sensorsGain;
  double upper_bound_sensorsGain;
  double *sensorsGain;
  
  void   update_hidden_layer                 ( void );
  void   update_output_layer     ( void );
  double update_StimesW                      ( int, int, int, vector<Node>&);
  double update_StimesWself                  ( int, vector<Node>& );

 public:
  Ctrnn3Layers(  );
  Ctrnn3Layers( Ctrnn3Layers& );
  virtual ~Ctrnn3Layers();
  
  /* -------------------------------------------------------------------------------------------------- */
  /*                                             VIRTUAL FUNCTIONS                                      */
  /* -------------------------------------------------------------------------------------------------- */  
  void init                          ( const vector <chromosome_type> &genes );
  void step                          ( const vector <double> &input_array, vector <double> &output_array);
  void reset                         ( void );
  void read_from_file                ( void ); 
  void compute_genotype_length       ( void );
  /* -------------------------------------------------------------------------------------------------- */  
  /* -------------------------------------------------------------------------------------------------- */  
  
};

#endif
