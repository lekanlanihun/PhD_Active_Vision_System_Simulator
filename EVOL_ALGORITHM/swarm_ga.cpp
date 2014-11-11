#include "swarm_ga.h"

/* -------------------------------------------------------------------------------------- */

Swarm_ga::Swarm_ga ( const int num_alleles_, const int num_bases_per_allele_, const int num_agents_per_group_ ) : GA( num_agents_per_group_) {
  
  num_mates_per_queen = 1;
  
  queens_chromo       = new Chromosome <chromosome_type> [num_tot_solutions];
  tmp_queens_chromo   = new Chromosome <chromosome_type> [num_tot_solutions]; 
  drones_chromo       = new Chromosome <chromosome_type> [num_tot_solutions];
  tmp_drones_chromo   = new Chromosome <chromosome_type> [num_tot_solutions];
  workers_chromo      = new Chromosome <chromosome_type> [1/*num_agents_per_group_*/];

  drones_for_queen.resize(num_tot_solutions);
  
  for(int c=0; c < num_tot_solutions; c++){
    queens_chromo[c].set_diploid ( true );
    queens_chromo[c].init_allele_values ( num_alleles_, num_bases_per_allele_ );
    tmp_queens_chromo[c].set_diploid ( true );
    
    drones_chromo[c].set_diploid ( false );
    drones_chromo[c].init_allele_values ( num_alleles_, num_bases_per_allele_ );
    //drones_chromo[c].create_with_cross_over_and_mutate_operators ( queens_chromo[c], queens_chromo[c], &prob_mutation );
    tmp_drones_chromo[c].set_diploid ( false );

    drones_for_queen[c].resize( num_mates_per_queen );
    
    for(int d = 0; d < num_mates_per_queen; d++){
      /* -------------------------------------------------------------------------------------- */
      /*                                           NEW CODE                                     */
      /* -------------------------------------------------------------------------------------- */
      drones_for_queen[c][d] = c;
      /* -------------------------------------------------------------------------------------- */
      /*                                           END NEW CODE                                 */
      /* -------------------------------------------------------------------------------------- */
      
      //drones_for_queen[c][d] = gsl_rng_uniform_int (GSL_randon_generator::r_rand, num_tot_solutions);
    }
  }
  fitness.clear();
  wheel.assign( num_tot_solutions, 0 );
  
#ifdef _RANK_BASED_
  linear_rank_wheel();
#endif
  
}

/* -------------------------------------------------------------------------------------- */

Swarm_ga::~Swarm_ga ( void  ) { 
  delete[] queens_chromo;
  delete[] tmp_queens_chromo;
  delete[] drones_chromo;
  delete[] tmp_drones_chromo;
  delete[] workers_chromo;
}

/* ------------------------------------------------------------------------------------------ */

vector <chromosome_type> & Swarm_ga::get_solution ( const int queen_id /*, const int agent_id */ ){
  init_workers( queen_id /*, agent_id */ );
  return workers_chromo[0/*agent_id*/].allele;
}

/* ------------------------------------------------------------------------------------------ */

void Swarm_ga::init_workers( const int queen_id /*, const int agent_id */ ){
  double local_prob_mutation = 0.0;
  int selected;
  if( num_mates_per_queen == 1 ) selected = 0;
  else selected = gsl_rng_uniform_int (GSL_randon_generator::r_rand, num_mates_per_queen );
  workers_chromo[0/*agent_id*/].set_diploid ( true );
  workers_chromo[0/*agent_id*/].create_with_cross_over_and_mutate_operators ( queens_chromo[queen_id], 
									 drones_chromo[drones_for_queen[queen_id][selected]], 
									 &local_prob_mutation );
}

/* -------------------------------------------------------------------------------------- */

void Swarm_ga::assign_fitness( const int ind, const vector <double> & final_fitness ){

  if( ind == 0 ){
    fitness.clear();
    wheel[ind] = 0.0;
    fitness.push_back( MakeValueWithIndex(final_fitness[0], ind) );
  }
  else {
    wheel[ind] = 0.0;
    fitness.push_back( MakeValueWithIndex(final_fitness[0], ind) );
    if( ind == (num_tot_solutions-1) ) {
      sort(fitness.begin(), fitness.end());
#ifndef _RANK_BASED_
      compute_wheel();
#endif
    }
  }
}

/* -------------------------------------------------------------------------------------- */

void Swarm_ga::breeding   ( void ){
  for(int counter = 0; counter < num_tot_solutions; counter++){
    int mum, dad;
    select_ind( &mum );
    int selected;
    if( num_mates_per_queen == 1 ) selected = 0;
    else selected = gsl_rng_uniform_int (GSL_randon_generator::r_rand, num_mates_per_queen );
    dad = drones_for_queen[mum][selected];
    
    tmp_queens_chromo[counter].create_with_cross_over_and_mutate_operators ( queens_chromo[mum],
  									     drones_chromo[dad],
  									     &prob_mutation );    

    /* -------------------------------------------------------------------------------------- */
    /*                                           NEW CODE                                     */
    /* -------------------------------------------------------------------------------------- */
    select_ind( &selected );
    if( gsl_rng_uniform(GSL_randon_generator::r_rand) < prob_cross_over ){
      tmp_drones_chromo[counter].create_with_cross_over_and_mutate_operators ( drones_chromo[dad],
									       drones_chromo[selected],
									       &prob_mutation );
    }
    else{
      tmp_drones_chromo[counter].create_with_mutate_operator ( drones_chromo[dad], &prob_mutation );
    }
    /* -------------------------------------------------------------------------------------- */
    /*                                           END NEW CODE                                 */
    /* -------------------------------------------------------------------------------------- */
    
    /*
      tmp_drones_chromo[counter].create_with_cross_over_and_mutate_operators ( queens_chromo[mum],
      queens_chromo[mum],
      &prob_mutation );
    */
  }
  
  for(int c = 0; c < num_tot_solutions; c++){
    queens_chromo[c] = tmp_queens_chromo[c];
    drones_chromo[c] = tmp_drones_chromo[c];
    //for(int d = 0; d < num_mates_per_queen; d++)
    //drones_for_queen[c][d] = gsl_rng_uniform_int (GSL_randon_generator::r_rand, num_tot_solutions);
  }
}

/* -------------------------------------------------------------------------------------- */

void Swarm_ga::select_ind( int * mum ){
  *mum = gsl_rng_uniform_int (GSL_randon_generator::r_rand, num_tot_solutions );
  double reference = gsl_rng_uniform(GSL_randon_generator::r_rand) * wheel[num_tot_solutions-1];
  for (int selected = num_solutions_truncated; selected  < num_tot_solutions; selected++ ){
    if(reference  < wheel[selected] ){
      *mum = fitness[selected].index;
      break;
    }
  }
}

/* --------------------------------------------------------------------------------------- */

void Swarm_ga::select_dad(int *mum, int *dad ){
  *dad = *mum;
  int iter = 0;
  do{
    select_ind( dad );
    iter++;
  }while(*mum == *dad && iter < 5);
}

/* -------------------------------------------------------------------------------------- */

void Swarm_ga::compute_wheel ( void ){
  double tmp_fit;
  for(int i = 0; i < num_tot_solutions; i++){
    if( i < num_solutions_truncated ){
      wheel[i] = 0.0;
      tmp_fit     = fitness[i].value;
    }
    else if ( i == num_solutions_truncated ){
      wheel[i] = fitness[i].value - tmp_fit;
    }
    else{
      wheel[i] += wheel[i-1] + ( fitness[i].value - tmp_fit);
    }
  }
}

/* -------------------------------------------------------------------------------------- */

#ifdef _RANK_BASED_
void Swarm_ga::linear_rank_wheel( void ){
  double selective_pressure = 2.0;
  for (int position = 0; position < num_tot_solutions; position++){
    if (position <= (num_solutions_truncated-1) ) {
      wheel[position] = 0.0;
    }
    else {
      wheel[position] = ((2.0 - selective_pressure) + 
			 2.0*(selective_pressure - 1.0)*((double)(position - num_solutions_truncated )/(double)(num_tot_solutions-1-num_solutions_truncated)) );
    }
  }
}
#endif

/* -------------------------------------------------------------------------------------- */

void Swarm_ga::dump_genome_into_file ( const char *dir, const char *fileName, const int generation ) {
  dump_queens_genome( dir, fileName, generation );
  dump_drones_genome( dir, fileName, generation );
}

/* -------------------------------------------------------------------------------------- */

void Swarm_ga::upload_genome_from_file( const char *dir, const char *fileName, const int generation ) {
  upload_queens_genome_from_file( dir, fileName, generation );
  upload_drones_genome_from_file( dir, fileName, generation );
  fitness.clear();
}

/* -------------------------------------------------------------------------------------- */

void Swarm_ga::dump_queens_genome( const char *dir, const char *fileName, const int generation ) {
  char fname_genome[500];
  sprintf(fname_genome, "%s%s_pop_G%d.queens", dir, fileName, generation );
  ofstream out ( fname_genome );
  out.setf( ios::fixed );

  for(unsigned int ind=0; ind<num_tot_solutions; ind++) {
    int ref = fitness[num_tot_solutions-1-ind].index;
    out << " " << queens_chromo[ref].get_allele_values().size();
    for(int n=0; n<queens_chromo[ref].get_allele_values().size(); n++){
      out << " " << setprecision(15) << queens_chromo[ref].get_allele_values()[n];
    }
    out	<< endl;
  }
  out.close();
}

/* -------------------------------------------------------------------------------------- */

void Swarm_ga::dump_drones_genome( const char *dir, const char *fileName, const int generation ) {
  char fname_genome[500];
  sprintf(fname_genome, "%s%s_pop_G%d.drones", dir, fileName, generation );
  ofstream out ( fname_genome );
  out.setf( ios::fixed );
  
  for(unsigned int ind=0; ind<num_tot_solutions; ind++) {
    out << " " << drones_chromo[ind].get_allele_values().size();
    for(int n=0; n<drones_chromo[ind].get_allele_values().size(); n++){
      out << " " << setprecision(15) << drones_chromo[ind].get_allele_values()[n];
    }
    for(int d = 0; d < num_mates_per_queen; d++)
      out << " " << drones_for_queen[fitness[num_tot_solutions-1-ind].index][d];
    out	<< endl;
  }
  out.close();
}

/* -------------------------------------------------------------------------------------- */

void Swarm_ga::upload_queens_genome_from_file( const char *dir, const char *fileName, const int generation ) {
  char fname_genome[500];
  sprintf(fname_genome, "%s%s_pop_G%d.queens", dir, fileName, generation );
  ifstream inFile (fname_genome );
  inFile.setf(ios::fixed );
  string s;
  int ind = 0;
  vector <chromosome_type> queens_genes;
  int genotype_length;
  
  while(getline(inFile, s) && (ind < num_tot_solutions) ){
    istringstream ss(s);
    ss >> genotype_length;
    queens_genes.assign(genotype_length, 0 );
    for(int q=0; q<queens_genes.size(); q++){
      ss >> queens_genes[q];
    }
    queens_chromo[ind].set_allele_values( queens_genes );
    ind++;
    queens_genes.clear();
  }
}

/* -------------------------------------------------------------------------------------- */

void Swarm_ga::upload_drones_genome_from_file( const char *dir, const char *fileName, const int generation ) {
  char fname_genome[500];
  sprintf(fname_genome, "%s%s_pop_G%d.drones", dir, fileName, generation );
  ifstream inFile (fname_genome );
  inFile.setf(ios::fixed );
  string s;
  int ind = 0;
  vector <chromosome_type> drones_genes;
  int genotype_length;
  
  while(getline(inFile, s) && (ind < num_tot_solutions) ){
    istringstream ss(s);
    
    ss >> genotype_length;
    drones_genes.assign(genotype_length, 0);
    for(int d=0; d<drones_genes.size(); d++){
      ss >> drones_genes[d];
    }
    for(int m = 0; m < num_mates_per_queen; m++) ss >> drones_for_queen[ind][m];
    drones_chromo[ind].set_allele_values( drones_genes );
    drones_genes.clear();
    ind++;
  }
}

/* -------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------- */
