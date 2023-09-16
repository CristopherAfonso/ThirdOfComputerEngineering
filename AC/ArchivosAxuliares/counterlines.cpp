#include <string>
#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
  if (argc == 2) {
    std::ifstream prog(argv[1], std::ios_base::in);
    int instr = -1;
    std::string l = "";

    while (std::getline(prog, l)) {
      if (!l.empty() && l[0] != '/' && l[l.size()-1] != ':') instr++;
    }

    std::cout << instr << std::endl;

    return 0;
  }

  return 1;  
}