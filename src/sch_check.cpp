#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <list>
#include <sstream>
#include <vector>
#include <algorithm>

#include "helper.h"

int main(int argc, char* argv[]) {
    //Input file names
    string inputFileName1 = argv[1];
    string inputFileName2 = argv[2];
    //string defaultName = "-Manual Student File-";

    //Call function in helper
    schedule_file_check(inputFileName1, inputFileName2);
    return 0;
} 