#include <deque>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "helper.h"

int main(int argc, char *argv[]) {

  //Input name of file
  string inputFileName = argv[1];

  //Call function in helper
  prereq_file_check(inputFileName);
  return 0;
}
