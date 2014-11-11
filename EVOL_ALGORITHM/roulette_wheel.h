#ifndef _ROULETTE_WHEEL_
#define _ROULETTE_WHEEL_

#include "ga.h"
#include "chromosome.h"

//#define _RANK_BASED_

using namespace std;

class Roulette_wheel : public GA { 
  
 private:
  vector <int> rank;
  vector <double> wheel;
  multimap<double, int> fitness;

  Chromosome <chromosome_type> *solution;
  Chromosome <chromosome_type> *tmp_solution;
  
 public:
  Roulette_wheel ( const int num_alleles_, const int num_bases_per_allele_, const int num_agents_per_group_ );
  ~Roulette_wheel( );

  /* -------------------------------------------------------------------------------------------------- */
  /*                                             VIRTUAL FUNCTIONS                                      */
  /* -------------------------------------------------------------------------------------------------- */  
  void breeding                                  ( void );
  void assign_fitness                            ( const int ind, const vector <double> & final_fitness );
  void dump_genome_into_file                     ( const char *dir, const char *fileName, const int generation );
  void upload_genome_from_file                   ( const char *dir, const char *fileName, const int generation );
  inline vector <chromosome_type> & get_solution ( const int genotype_id /*, const int ind_id */ ){ return solution[genotype_id].allele; }
  /* -------------------------------------------------------------------------------------------------- */
  
  void rank_and_wheel                   ( void );
  void linear_rank_wheel                ( void );
  void select_ind                       ( int *mum );
  void select_dad                       ( const int *mum, int *dad );  
  
};

#endif
