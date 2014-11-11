#ifndef SIMPLE_3_LAYERS_H
#define SIMPLE_3_LAYERS_H

#include "controller.h"

class Simple3Layers : public Controller {
  
 private:
  int num_hidden;
  
  typedef struct {
    double state;
    double *weightsOut;
  } Node;

  vector <Node> biasLayer;
  vector <Node> inputLayer;
  vector <Node> hiddenLayer;
  vector <Node> outputLayer;
  
  double low_bound_Wts;
  double upper_bound_Wts;
  
 public:
  Simple3Layers(  );
  Simple3Layers( Simple3Layers& );
  virtual ~Simple3Layers();

  /* -------------------------------------------------------------------------------------------------- */
  /*                                             VIRTUAL FUNCTIONS                                      */
  /* -------------------------------------------------------------------------------------------------- */   
  void init                    ( const vector <chromosome_type>    &base );
  void step                    ( const vector <double> &input_array, vector <double> &output_array );
  void reset                   ( void );
  void read_from_file          ( void ); 
  void compute_genotype_length ( void );
  /* -------------------------------------------------------------------------------------------------- */   
  /* -------------------------------------------------------------------------------------------------- */   
    
};

#endif
