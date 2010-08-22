#include "polybori.h"


#include <iostream>


void print_env();

using namespace polybori;
int main() {

  std::cout << "BooleEnv::ring "<<(BooleEnv::ring) <<std::endl;


  std::cout << "&BooleEnv::ring() "<< &(BooleEnv::ring()) <<std::endl;

  print_env();
  return 0;
}
