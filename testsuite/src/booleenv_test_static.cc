#include "polybori.h"


#include <iostream>



using namespace polybori;

void print_env() {
  std::cout << "static: BooleEnv::ring "<<(BooleEnv::ring) <<std::endl;


  std::cout << "static: &BooleEnv::ring() "<< &(BooleEnv::ring()) <<std::endl;
}
