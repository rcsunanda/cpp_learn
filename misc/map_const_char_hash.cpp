//g++ -std=c++11 map_const_char_hash.cpp -o map_const_char_hash

// unordered_map::begin/end example
#include <iostream>
#include <unordered_map>
#include <map>
#include <cstring>


//*************************************************************************************************
struct cmp_str
{
   bool operator()(char const *a, char const *b)
   {
      return std::strcmp(a, b) < 0;
   }
};


//*************************************************************************************************
int main (int argc, char** argv)
{
  if (argc < 2)
  {
	  std::cout << "Enter BBB as argument" << std::endl;
	  return 1;
  }
  
  std::map<const char*, int, cmp_str> mymap;
  
  mymap["AAA"] = 1;
  mymap["BBB"] = 2;	//must enter as argument 
  mymap["CCC"] = 3;	//must enter as argument

  //look up const char created here
  const char* x = "AAA";
  auto ite = mymap.find(x);
  if (ite != mymap.end())
    std::cout << "FOUND x" << std::endl;

   //look up const char given as arg
  const char* arg = argv[1];
  //std::cout << "arg = " << arg << std::endl;	//to check
  
  auto ite2 = mymap.find(arg);
  
  if (ite2 != mymap.end())
    std::cout << "FOUND arg" << std::endl;

  //look up const char from std::string
  std::string ccc = "CCC";
  auto ite3 = mymap.find(ccc.c_str());

  if (ite3 != mymap.end())
    std::cout << "FOUND ccc" << std::endl;
  
  return 0;
}