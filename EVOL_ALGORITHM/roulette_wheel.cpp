#include "roulette_wheel.h"

/* -------------------------------------------------------------------------------------- */

Roulette_wheel::Roulette_wheel ( const int num_alleles_, const int num_bases_per_allele_, const int num_agents_per_group_ ) : GA( num_agents_per_group_) {
  
  solution          = new Chromosome <chromosome_type> [num_tot_solutions];
  tmp_solution      = new Chromosome <chromosome_type> [num_tot_solutions]; 
  
  for(int c=0; c < num_tot_solutions; c++){
    solution[c].set_diploid ( DIPLOID_FLAG );
    solution[c].init_allele_values ( num_alleles_, num_bases_per_allele_ );
    tmp_solution[c].set_diploid ( DIPLOID_FLAG );
  }
  
  rank.assign  (num_tot_solutions, 0);
  wheel.assign (num_tot_solutions, 0);
  fitness.clear();  
  
#ifdef _RANK_BASED_
  linear_rank_wheel();
#endif
  
}

/* -------------------------------------------------------------------------------------- */

Roulette_wheel::~Roulette_wheel ( void ){
  delete[] solution;
  delete[] tmp_solution;
}

/* -------------------------------------------------------------------------------------- */

void Roulette_wheel::assign_fitness( const int ind, const vector <double> & final_fitness ){
  if( ind == 0 ){
    fitness.clear();
    fitness.insert( make_pair( final_fitness[0], ind) );
    wheel[ind] = 0.0;
    rank[ind]  = ind;
  }
  else {
    fitness.insert( make_pair( final_fitness[0], ind) );
    wheel[ind] = 0.0;
    rank[ind]  = ind;
    if ( ind == (num_tot_solutions-1) ) rank_and_wheel ( );
  }
}

/* -------------------------------------------------------------------------------------- */

void Roulette_wheel::breeding   ( void ){
  
  for(int counter = 0; counter < num_tot_solutions; counter++){
    if( counter < num_elite ){
      tmp_solution[counter] = solution[rank[num_tot_solutions-1-counter]];
    }
    else{
      int mum, dad;
      select_ind( &mum );
      if( gsl_rng_uniform(GSL_randon_generator::r_rand)  < prob_cross_over ) {
	select_dad( &mum, &dad );
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

void Roulette_wheel::select_ind( int *mum ){
  *mum = gsl_rng_uniform_int (GSL_randon_generator::r_rand, num_tot_solutions );
  double reference = gsl_rng_uniform(GSL_randon_generator::r_rand) * wheel[num_tot_solutions-1];
  for (int selected = num_solutions_truncated; selected  < num_tot_solutions; selected++ ){
    if(reference  < wheel[selected] ){
      *mum = rank[selected];
      break;
    }
  }
}

/* --------------------------------------------------------------------------------------- */

void Roulette_wheel::select_dad(const int *mum, int *dad ){
  *dad = *mum;
  int iter = 0;
  do{
    select_ind( dad );
    iter++;
  }while(*mum == *dad && iter < 5);
}

/* -------------------------------------------------------------------------------------- */

void Roulette_wheel::rank_and_wheel ( void ){
  int iter = 0;
  double tmp_fit = 0.0;
  /* iterate from worst to best fitness value 
     rank[0] : ID of the worst solution
     rank[num_tot_solutions-1] : ID of the best solution 
     wheel[0] : is the worst
     wheel[num_tot_solutions-1] : is the maximum 
  */
  for (multimap<double, int>::iterator it = fitness.begin(); it != fitness.end(); it++){ 
    rank[iter]  = (*it).second;
#ifndef _RANK_BASED_
    if( iter < num_solutions_truncated ){
      wheel[iter] = 0.0;
      tmp_fit     = (*it).first;
    }
    else if ( iter == num_solutions_truncated ){
      wheel[iter] = (*it).first - tmp_fit;
    }
    else{
      wheel[iter] += wheel[iter-1] + ( (*it).first - tmp_fit);
    }
#endif
    iter++;
  }

  // iter = num_tot_solutions-1;
  // for (multimap<double, int>::reverse_iterator it = fitness.rbegin(); it != fitness.rend(); it++){ 
  //   cerr << " FIT = " << (*it).first << " ID = " << (*it).second 
  // 	 << " wheel["<<iter<<"]= " << wheel[iter]
  // 	 << endl;
  //   iter--;
  //   if( !(iter%1000) )
  //     getchar();
  // }

}

/* -------------------------------------------------------------------------------------- */

#ifdef _RANK_BASED_
void Roulette_wheel::linear_rank_wheel( void ){
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

void Roulette_wheel::dump_genome_into_file( const char *dir, const char *fileName, const int generation ) {
  char fname_genome[500];
  sprintf(fname_genome, "%s%s_pop_G%d.geno", dir, fileName, generation );
  ofstream out ( fname_genome );
  out.setf( ios::fixed );
  for(unsigned int ind=0; ind<num_tot_solutions; ind++) {
    out << solution[rank[num_tot_solutions-1-ind]].get_allele_values().size();
    for(int n=0; n<solution[rank[num_tot_solutions-1-ind]].get_allele_values().size(); n++){
      out << " " << solution[rank[num_tot_solutions-1-ind]].get_allele_values()[n]; //<< setprecision(15)
    }
    out	<< endl;
  }
  out.close();
}

/* -------------------------------------------------------------------------------------- */

void Roulette_wheel::upload_genome_from_file( const char *dir, const char *fileName, const int generation ) {
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

