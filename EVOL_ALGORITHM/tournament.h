#ifndef _TOURNAMENT_SELECTION__
#define _TOURNAMENT_SELECTION__

#include "ga.h"
#include "chromosome.h"

#define POOL_SIZE     4
using namespace std;

class Tournament : public GA { 
  
 private:
  Chromosome <chromosome_type> *solution;
  Chromosome <chromosome_type> *tmp_solution;
  vector< ValueWithIndex <double> > fitness;
  
 public:
  Tournament ( const int num_alleles_, const int num_bases_per_allele_, const int num_agents_per_group_ );
  ~Tournament( );
  
  /* -------------------------------------------------------------------------------------------------- */
  /*                                             VIRTUAL FUNCTIONS                                      */
  /* -------------------------------------------------------------------------------------------------- */  
  void breeding                                  ( void );
  void assign_fitness                            ( const int ind, const vector <double> & final_fitness);
  void dump_genome_into_file                     ( const char *dir, const char *fileName, const int generation );
  void upload_genome_from_file                   ( const char *dir, const char *fileName, const int generation );
  inline vector <chromosome_type> & get_solution ( const int genotype_id /*, const int ind_id */ ){ return solution[genotype_id].allele; }
  /* -------------------------------------------------------------------------------------------------- */
  
  void select_ind                                ( int *mum );
};

#endif
