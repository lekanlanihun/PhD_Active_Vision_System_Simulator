#ifdef _GRAPHICS_
#include "../EXP/RENDERING/viewer.h"
#include "../EXP/RENDERING/interface.h"
#endif

#include "../EXP/experiment.h"

bool evolution     = false;
bool viewing       = false;
bool re_evaluation = false;
char *run_name  = new char[100];
unsigned long root_seed = 0;
int target_g = 0;

/* ------------------------------------------------------------ */
/* ------------------------------------------------------------ */

void usage ( )
{
  cerr << "Bad input format - Options are required \n" 
       << "-e evolutionary mode, the parameter -n and -s has to be specified  \n "
       << "-n requires a evolutionary run name \n "
       << "-r re-evaluation mode. It requires a evolutionary run name \n "
       << "-s requires a seed number \n "
       << "-v viewing mode - with graphical rendering  - no file name needed \n "
       << endl;
  exit(-1);
}

/* ------------------------------------------------------------ */
/* ------------------------------------------------------------ */

void  parse_command_line( int argc, char** argv ) {
  //  cout << "parsing command line" << endl;

if ( argc < 2 )
	usage( );
  for(int i=1; i<argc; i++) {
    if(argv[i][0] != '-') usage( );
    switch(argv[i][1]) {
    case 'r': 
      if ( i==argc-1 ) { // if last arg
	cerr << "-r re-evaluation mode, the parameter requires a run name "<<endl;
	exit(-1);
      }
      ++i;
      re_evaluation = true;
      sprintf(run_name, "%s", argv[i]); 

      ++i;
      target_g = atoi(argv[i]); 
      break;
    case 'e': 
      if ( i==argc-1 ) { // if last arg
	cerr << "-e evolutionary mode, the parameter -n and -s have to be specified "<<endl;
	exit(-1);
      }
      ++i;
      if(argv[i][0] == '-' && argv[i][1] == 'n' ) {  // looks like the correct flag
	if ( i==argc-1 ) { // if last arg
 	  cerr << "-n requires a name for the evolutionary run "<<endl;
 	  exit(-1);
	}
	++i;
	evolution = true;
	sprintf(run_name, "%s", argv[i]); 
	/* ------------ root_seed  ------------ */
	if ( i==argc-1 ) { // if last arg
	  cerr << "-e evolutionary mode, the parameter -s (followed by a long int as root seed) has to be specified for the evolutionary run "<<endl;
	  exit(-1);
	}
	else{
	  ++i;
	  if(argv[i][0] == '-' && argv[i][1] == 's' ) {  // looks like the correct flag
	    if ( i==argc-1 ) { // if last arg
	      cerr << "-s requires a long int as root seed for the evolutionary run "<<endl;
	      exit(-1);
	    }
	    ++i;
	    root_seed = atoi(argv[i]);
	  }
	  else{
	    cerr << "-e evolutionary mode, the parameter -n with a file name has to be specified "<<endl;
	    exit(-1);
	  }
	}
	/* ------------------------------------ */
      }
      else{
	cerr << "-e evolutionary mode, the parameter -n with a file name has to be specified "<<endl;
	exit(-1);
      }
      break;
    case 'v':
      viewing   = true; 
      break;  
    case 'm':
      cerr << " I running the algorithm to build visuo-motor maps " << endl;
      break;     
    default:
      usage();
    }
  }
}

/* ------------------------------------------------------------ */
/* ------------------------------------------------------------ */

int main(int argc, char** argv)
{
  //parse the command line
  parse_command_line( argc, argv );
  
 //cout << "Creating experiment" << endl;
  //This is where you create the experiment
  EXP_Class *exp = new EXP_Class( run_name, evolution, viewing, re_evaluation, root_seed );
  
//cout << "Created experiment" << endl;

  if( evolution ){
    exp->init_evolutionary_run( );
    do{//This is the loop for the generations
    //  cout << "enter into the loop for generation" << endl;
      exp->init_genotype_loop( );
      do{//This is the loop for the solutions (genotypes)
      //cout << "enter into the loop for genetopes" << endl;
	exp->init_evaluations_loop( );
	do{//Loop for evaluations
      //cout << "enter into the loop for evaluations" << endl;
	  exp->init_single_evaluation( );
	  do{//This is the loop for the iterations
        //cout << "enter into the loop for iterations" << endl;
        exp->adv();
	  }while( exp->stop_iterations_loop( ) );
	}while( exp->stop_evaluations_loop( ) );//Until the last evalustion
      }while( exp->stop_genotypes_loop( ) ); //Until the last solution (genotype)
    }while( exp->stop_generations_loop() );//Until the end of the world
  }
  else {
    if( viewing ) {
#ifdef _GRAPHICS_
      QApplication application(argc, argv);
      //Video     *video  = new Video();
      Viewer    *viewer = new Viewer( 0, exp, argc, argv/*, video*/);
      Interface *render = new Interface( 0, exp, viewer/*, video */);
      render->show();
      return application.exec();
      delete viewer;
      delete render;
#endif
    }
  }
//  cout << "Deleting exp" << endl;
  delete exp;
  //cout << "Deleting run name" << endl;
  delete[] run_name;
 // cout << "Finished" << endl;
}
