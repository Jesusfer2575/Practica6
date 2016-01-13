#include <random>
#include <cstdlib>
#include <ctime>
#include "mirandom.h"
#define C11
using namespace std;


#ifndef C11
void init(){
  
}

int next(){
	srand(time(NULL));
	return rand();
}	
#endif
