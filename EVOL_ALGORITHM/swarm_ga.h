#ifndef _SWARM_GA_
#define _SWARM_GA_

#include "ga.h"
#include "chromosome.h"

//#define _RANK_BASED_

using namespace std;

class Swarm_ga : public GA { 
  
 private:
  vector <double> wheel;
  vector< ValueWithIndex <double> > fitness;
  
  int num_mates_per_queen;
  vector < vector <int> > drones_for_queen;
  
  Chromosome <chromosome_type> *queens_chromo;
  Chromosome <chromosome_type> *tmp_queens_chromo;
  Chromosome <chromosome_type> *drones_chromo;
  Chromosome <chromosome_type> *tmp_drones_chromo;
  Chromosome <chromosome_type> *workers_chromo;
  
 public:
  Swarm_ga ( const int num_alleles_, const int num_bases_per_allele_,  const int num_agents_per_group_ );
  ~Swarm_ga( );

  /* -------------------------------------------------------------------------------------------------- */
  /*                                             VIRTUAL FUNCTIONS                                      */
  /* -------------------------------------------------------------------------------------------------- */  
  void assign_fitness                     ( const int ind, const vector <double> & final_fitness );
  void breeding                           ( void );
  void dump_genome_into_file              ( const char *dir, const char *fileName, const int generation );
  void upload_genome_from_file            ( const char *dir, const char *fileName, const int generation );
  vector <chromosome_type> & get_solution ( const int queen_id /*, const int agent_id */ );
  /* -------------------------------------------------------------------------------------------------- */
  /* -------------------------------------------------------------------------------------------------- */
  
  void init_workers                   ( const int queen_id /*, const int agent_id */);
  void compute_wheel                  ( void );
  void linear_rank_wheel              ( void );
  void select_ind                     ( int *mum );
  void select_dad                     ( int *mum, int *dad );
  void dump_queens_genome             ( const char *dir, const char *fileName, const int generation );
  void dump_drones_genome             ( const char *dir, const char *fileName, const int generation );
  void upload_queens_genome_from_file ( const char *dir, const char *fileName, const int generation );
  void upload_drones_genome_from_file ( const char *dir, const char *fileName, const int generation );
  
};

#endif
