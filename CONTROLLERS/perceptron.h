#ifndef _PERCEPTRON_
#define _PERCEPTRON_

#include "controller.h"

class Perceptron : public Controller {
  
 private:
  
  typedef struct {
    double state;
    double *weightsOut;
  } Node;
  vector <Node> biasLayer;
  vector <Node> inputLayer;
  
  double low_bound_Wts;
  double upper_bound_Wts;
  
 public:
  Perceptron(  );
  Perceptron( Perceptron& );
  virtual ~Perceptron();


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
  inline vector <Node> getInputLayer  ( void )  { return inputLayer;  }
  
};

#endif
