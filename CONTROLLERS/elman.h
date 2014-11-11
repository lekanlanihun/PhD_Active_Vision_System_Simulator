#ifndef _ELMAN_
#define _ELMAN_

#include "controller.h"

class Elman : public Controller {
  
 private:
  int num_hidden;
  
  typedef struct {
    double state;
    double *weightsOut;
  } Node;
  vector <Node> biasLayer;
  vector <Node> inputLayer;
  vector <Node> hiddenLayer;
  vector <Node> hiddenLayerCopy;
  vector <Node> outputLayer;
  
  double low_bound_Wts;
  double upper_bound_Wts;
  
 public:
  Elman(  );
  Elman( Elman& );
  virtual ~Elman();

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

  inline vector <Node> getInputLayer  ( void )  { return inputLayer;  }
  inline vector <Node> getHiddenLayer ( void )  { return hiddenLayer; }
  inline vector <Node> getOutputLayer ( void )  { return outputLayer; }
};

#endif
