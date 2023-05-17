#ifndef HELPER_H
#define HELPER_H

#include <deque>
#include <fstream>
#include <iostream>
#include <set>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <list>
#include <algorithm>
#include <cstring>

using namespace std;

//Prereq checker functions
bool cycle_Check(string node, unordered_map<string, vector<string> > &graph, set<string> &visited, vector<string> &path, bool& returnBool);

void prereq_DFS(unordered_map<string, vector<string> > &graph, string course, int &counter, set<string> &visited_set); 

int prereq_check(unordered_map<string, vector<string> > &adjlist, string course); // call from main, recursimvely calls other for DFS

bool prereq_file_check(string inputFile);


//Schedule checker functions
bool compareSemesters(string s1, string s2);

bool schedule_file_check(string input1, string input2);

#endif