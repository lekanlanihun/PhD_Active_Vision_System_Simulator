#include "tournament.h"

/* -------------------------------------------------------------------------------------- */

Tournament::Tournament ( const int num_alleles_, const int num_bases_per_allele_, const int num_agents_per_group_ ) : GA( num_agents_per_group_) {

  solution          = new Chromosome <chromosome_type> [num_tot_solutions];
  tmp_solution      = new Chromosome <chromosome_type> [num_tot_solutions]; 
  
  for(int c=0; c < num_tot_solutions; c++){
    solution[c].set_diploid ( DIPLOID_FLAG );
    solution[c].init_allele_values ( num_alleles_, num_bases_per_allele_ );
    tmp_solution[c].set_diploid ( DIPLOID_FLAG );
  }
  fitness.clear();
}

/* -------------------------------------------------------------------------------------- */

Tournament::~Tournament ( void ){
  delete[] solution;
  delete[] tmp_solution;
}

/* -------------------------------------------------------------------------------------- */

void Tournament::assign_fitness( const int ind, const vector <double> & final_fitness ){
  if( ind == 0 ){
    fitness.clear();
    fitness.push_back( MakeValueWithIndex(final_fitness[0], ind) );
  }
  else {
    fitness.push_back( MakeValueWithIndex(final_fitness[0], ind) );
    if( ind == (num_tot_solutions-1) ) sort(fitness.begin(), fitness.end());
  }
}

/* -------------------------------------------------------------------------------------- */

void Tournament::breeding   ( void ){
  for(int counter = 0; counter < num_tot_solutions; counter++){
    if( counter < num_elite ){
      tmp_solution[counter] = solution[fitness[num_tot_solutions-1-counter].index];
    }
    else{
      int mum, dad;
      select_ind( &mum );
      if( gsl_rng_uniform(GSL_randon_generator::r_rand)  < prob_cross_over ) {
	dad = mum;
	do{
	  select_ind( &dad );
	}while(mum == dad );
	tmp_solution[counter].create_with_cross_over_and_mutate_operators ( solution[mum], solution[dad], &prob_mutation );
      }
      else{
	tmp_solution[counter].create_with_mutate_operator ( solution[mum], &prob_mutation );
      }
    }
  }
  
  for(int c = 0; c < num_tot_solutions; c++){
    solution[c] = tmp_solution[c];
  }
}

/* -------------------------------------------------------------------------------------- */

void Tournament::select_ind( int *ind ){
  int p[POOL_SIZE];
  int counter = 0;
  double max_value = 0.0;
  bool equal_ind   = false;
  do{
    do{
      equal_ind = false;
      p[counter] = gsl_rng_uniform_int (GSL_randon_generator::r_rand, num_tot_solutions );
      for (int j = 0; j < counter; j++){
	if( p[counter] == p[j] ) {
	  equal_ind = true;
	  break;
	}
      }
    }while( equal_ind );
    if( fitness[p[counter]].value >= max_value ){
      max_value = fitness[p[counter]].value;
      *ind = fitness[p[counter]].index;
    }
    counter++;
  }while(counter != POOL_SIZE);
}

/* -------------------------------------------------------------------------------------- */

void Tournament::dump_genome_into_file( const char *dir, const char *fileName, const int generation ) {
  char fname_genome[500];
  sprintf(fname_genome, "%s%s_pop_G%d.geno", dir, fileName, generation );
  ofstream out ( fname_genome );
  out.setf( ios::fixed );
  for(unsigned int ind=0; ind<num_tot_solutions; ind++) {
    int ref = fitness[num_tot_solutions-1-ind].index;
    out << solution[ref].get_allele_values().size();
    for(int n=0; n<solution[ref].get_allele_values().size(); n++){
      out << " " << setprecision(15) << solution[ref].get_allele_values()[n];
    }
    out	<< endl;
  }
  out.close();
}

/* -------------------------------------------------------------------------------------- */

void Tournament::upload_genome_from_file( const char *dir, const char *fileName, const int generation ) {
  char fname_genome[500];
  sprintf(fname_genome, "%s%s_pop_G%d.geno", dir, fileName, generation );
  
  ifstream inFile (fname_genome );
  inFile.setf(ios::fixed );
  string s;
  int ind = 0;
  int genotype_lenght;
  vector <chromosome_type> genes;
  genes.assign(solution[0].get_allele_values().size(), 0);
  
  while(getline(inFile, s) && (ind < num_tot_solutions) ){
    istringstream ss(s);
    ss >> genotype_lenght;
    for(int q=0; q<genes.size(); q++){
      ss >> genes[q];
    }
    solution[ind].set_allele_values( genes );
    ind++;
  }
  fitness.clear();
}

/* -------------------------------------------------------------------------------------- */
//                                       END
/* -------------------------------------------------------------------------------------- */

