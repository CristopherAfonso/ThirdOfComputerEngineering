#include <string>
#include <fstream>
#include <iostream>

int main(int argc, char** argv) {
  if (argc == 3) {
    std::ifstream file_in(argv[1], std::ios_base::in);
    std::ofstream file_out(argv[2], std::ios_base::trunc);
    if (file_in.is_open() && file_out.is_open()) {
      std::string val = "", line_res = "";
      while (std::getline(file_in, val)) {
        size_t last_sp_pos = val.find_last_of(' ');
        line_res += (val.substr(last_sp_pos != std::string::npos ? last_sp_pos+1 : 0)) + ".0 ";
      }
      file_out << line_res.substr(0, line_res.size()-1);
      return 0;
    }
  }
  
  return -1;
}

