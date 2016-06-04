/**
 * @file main.cpp 
 *
 * description
 *
 * @author theNerd247 (noah.harvey247 gm)
 * @copyright GNU Public License 2
 */

#include <map>
#include <iostream>

#include "arbitrary/arbitrary.hpp"

INIT_ARBITRARY()

template<typename K,typename T>
std::ostream& operator<<(std::ostream& os, const std::map<K,T>& mp)
{
  
  for(auto& elem : mp)
  {
    os << elem.first << ": " << elem.second << std::endl;
  }

  return os;
}

int main(int argc, char const *argv[])
{
  std::cout << "Int: " << arbitrary::arbitrary<int>() << std::endl;
  std::cout << "float: " << arbitrary::arbitrary<int>() << std::endl;
  std::cout << "map: \n" << arbitrary::sizedContainer<5,std::map<int,float>>() << std::endl;
  std::cout << "\nString: " << arbitrary::arbitrary<std::string>() << std::endl;
  
	return 0;
}
