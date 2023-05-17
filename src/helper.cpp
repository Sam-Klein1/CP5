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
#include <regex>
#include <algorithm>
#include <cstring>

#include "helper.h"


bool cycle_Check(string node, unordered_map<string, vector<string> > &graph, set<string> &visited, vector<string> &path, bool& returnBool) {
  bool cycle_found = false;
  string prereqString;
  visited.insert(node);
  path.push_back(node);

  for (vector<string>::iterator preCourse = graph[node].begin(); preCourse != graph[node].end(); preCourse++) // For each prereq of the current vertex
  {
    char prereqTag = (*preCourse)[0];
    (*preCourse) = (*preCourse).substr(1); //remove tag from preReq
    if (visited.find(*preCourse) == visited.end()) { // If prereq hasnt been visited
      cycle_Check(*preCourse, graph, visited, path, returnBool); // Run DFS on that prereq
    }

    // Check if the prereq is already in the path
    vector<string>::iterator cycleCheck = path.begin();
    while (cycleCheck != path.end() && (*cycleCheck != *preCourse)) {
      cycleCheck++;
    }

    vector<string>::iterator originCourse = cycleCheck; // Set iterator at possible cycle start (just for print)
    if (cycleCheck != path.end()) // If prereq exists in path, a cycle is found
    {
      returnBool = true;
      cout << "Not Viable, cycle found in prereqs for " << *originCourse << ": ";
      while (cycleCheck != path.end()) {
        cout << *cycleCheck << " -> ";
        cycleCheck++;
      }
      cout << *originCourse << endl;
      cycle_found = true;
    }
    (*preCourse) = prereqTag + (*preCourse); //Add the tag back to the prereq
  }
  path.pop_back();
  return cycle_found;
  
}

void prereq_DFS(unordered_map<string, vector<string> > &graph, string course, int &counter, set<string> &visited_set) 
{
  string prereqCourse;
  vector<string>::iterator prereq_itr = graph[course].begin(); // Prepare iterator to check each prereq of current course
  visited_set.insert(course); // Set current course as visited
  char ORtag = '+'; // Plan is here--------------------------------------------------------------------------------------------------------

  while (prereq_itr != graph[course].end()) {
    prereqCourse = (*prereq_itr);
    prereqCourse = prereqCourse.substr(1); // Take course name without tag for recursive call

    if (visited_set.find(prereqCourse) == visited_set.end()) // If current course isn't found in visted set, visit it
    {
      if(ORtag != (*prereq_itr)[0])
      {
        counter++; // Increment counter then visit the course
        ORtag = (*prereq_itr)[0];
      }
      prereq_DFS(graph, prereqCourse, counter, visited_set); // Now visit THIS courses prereqs
    }
    prereq_itr++;
  }
}

int prereq_check(unordered_map<string, vector<string> > &adjlist, string course) // call from main, recursimvely calls other for DFS
{
  int prereq_count = 0;
  set<string> visitedNodes;
  // Start recursion loop
  if (!adjlist[course].empty()) {
    prereq_DFS(adjlist, course, prereq_count, visitedNodes);
  }

  return prereq_count;
}

bool prereq_file_check(string inputFile)
{
  string line;
  ifstream infile(inputFile);

  unordered_map<string, vector<string> > adjlist;

  string preReq;
  string course;
  int tag_number = 0;
  
  while (getline(infile, line)) { // read each line, create the graph

    stringstream ss(line);
    ss >> course; // extract vertex
    vector<string> preReqs;

    
    int count = 0;
    while (ss >> preReq) // make list
    {
      preReq = (char)tag_number + preReq; //tag the prereq
      adjlist[course].push_back(preReq);
    }
    tag_number++;
  }

  set<string> visitedNodes;
  vector<string> pathNodes;
  bool pre_cycle_not_viable = false, cycle_return = false;
  unordered_map<string, vector<string> >::iterator course_itr = adjlist.begin();

  while (course_itr != adjlist.end()) {
    cycle_Check((*course_itr).first, adjlist, visitedNodes, pathNodes, cycle_return);
    if (cycle_return)
    {
      // If any cycle is found, our final output will state there is a cycle
      pre_cycle_not_viable = true;
    }
    course_itr++;
  }

  visitedNodes.clear();
  course_itr = adjlist.begin();
  int prereq_count = 0;
  bool pre_seq_not_viable = false;
  while (course_itr != adjlist.end()) 
  {
    prereq_count = prereq_check(adjlist, (*course_itr).first);
    if (prereq_count > 6) 
    {
      pre_seq_not_viable = true;
      cout << "Not Viable, course " << (*course_itr).first << " has too many prereqs (" << prereq_count << ")" << endl;
    }
    course_itr++;
  }

  if((!pre_seq_not_viable) && (!pre_cycle_not_viable))
  {
    cout << "Viable: No errors found for prerequisites" << endl;
    return true;
  }
  return false;
  
}


//sorting by year and semester(spring then fall)
bool compareSemesters(string s1, string s2){
    int year1 = stoi(s1.substr(0, 4));
    int year2 = stoi(s2.substr(0, 4));
    string season1;
    string season2;
    if(s1.find("Fall")!= string::npos){
        season1 = s1.substr(4,4);
    }else if(s1.find("Spring")!= string::npos){
        season1 = s1.substr(4,6);
    }else if(s1.find("Summer")!= string::npos){
        season1 = s1.substr(4,6);
    }
    if(s2.find("Fall")!= string::npos){
        season2 = s2.substr(4,4);
    }else if(s2.find("Spring")!= string::npos){
        season2 = s2.substr(4,6);
    }else if(s2.find("Summer")!= string::npos){
        season2 = s2.substr(4,6);
    }

    if(year1 < year2){
        return true;
    }else if(year1 == year2 && season1 == "Spring" && season2 == "Fall"){
        return true;
    }else if(year1 == year2 && season1 == "Spring" && season2 == "Summer"){
        return true;
    }else if(year1 == year2 && season1 == "Summer" && season2 == "Fall"){
        return true;
    }
    return false;
}

bool schedule_file_check(string input1, string input2){
  // Open the two files for reading
  ifstream file1(input1); //Prereq file
  ifstream file2(input2); //File holding schedule
  string preReq;
  string course;
  map<string, list<string> > adjList;

  /*Read each line from the prereq file and input them into a graph to access later*/
  string line;
  int tag_number = 1000;
  int count = 0;
  string tag_string;
  while (getline(file1, line) ) {
    stringstream ss(line);
    
    ss >> course; 
    list<string> preReqs;
    while (ss >> preReq){
        tag_string = to_string(tag_number);
        preReq = tag_string + preReq; //tag the prereq
        adjList[course].push_back(preReq);
    }
    tag_number++;
  }
  
  //sorting schedule file into year/semester order
  vector<string> courses;

  while(getline(file2, line)){
    string currSemester;
    string currCourse;
    
    stringstream ss(line);

    ss >> currCourse;
    ss >> currSemester;

    if(currCourse != "" && currSemester != ""){
        string semester = currSemester+currCourse;
        courses.push_back(semester);

        sort(courses.begin(), courses.end(), compareSemesters);
    }
      
      
  }

  map<string, int> numPerSemester;
  vector<string> coursesTaken;

  for(auto i=courses.begin(); i<courses.end(); i++){
    string course = *i;
    string currSemester;
    string currCourse;

    //seperates line's semester and course depending on whether its fall or spring
    if(course.find("Fall")!= string::npos){
      currSemester = course.substr(0,8);
      currCourse = course.substr(8);
    }else if(course.find("Spring")!= string::npos){
      currSemester = course.substr(0,10);
      currCourse = course.substr(10);
    }else if(course.find("Summer")!= string::npos){
      currSemester = course.substr(0,10);
      currCourse = course.substr(10);
    }

    //check for if schedule includes the same course more than once
    for (auto i : coursesTaken) {
      if(i == currCourse){ 
          cout << "Error: Duplicate course for "<< currCourse << endl;
          file1.close();
          file2.close();

          return false;
      }
    }
    
    coursesTaken.push_back(currCourse);

    //check for schedule proposes too many courses (more than 3) in a semester

    //if new semester then add one too that course in the numPerSemester map
    //else existing semester add one to count
    if(numPerSemester[currSemester] == 0){
        numPerSemester[currSemester] = 1;
        
    }else{
      int count = numPerSemester[currSemester];
      count++;
      numPerSemester[currSemester] = count;
    }

    //if count in one semester is over 3 throw error

    if(numPerSemester[currSemester] > 3){
      cout << "Error: More than 3 classes in " << currSemester << " semester" << endl;
      file1.close();
      file2.close();

      return false;
    }

    //check for schedule proposes to take a course before its prerequisite
    //add prereqs to a list and remove each time its found in the course already taken
    //if list is not empty then there are preqreqs that were not taken already and trhow an error
    list<string> preReqs = adjList[currCourse];

    for (string i : coursesTaken) {
      string currTag;
      bool removed = false;
      
      for (string j : adjList[currCourse]) {
        //check the tag number
        //if there is i loop through the list to find all other members with the tag
        string tagChar = j.substr(0,4);
        list<string> taggedCourses;
        if(tagChar != currTag){
          currTag = tagChar;
        }
        
        string prereqnochar = j.substr(4);
        if(i == prereqnochar){
          
          preReqs.remove(j);
          for(string k : adjList[currCourse]){
            if(k.substr(0,4) == currTag)
              preReqs.remove(k);
          }
        }
      }
    }

    if(preReqs.size() != 0){
      cout << "Error: Prereqs not done for " << currCourse << endl;
      file1.close();
      file2.close();

      return false;
    }
  }

  // Close the files
  cout << "Viable: No errors found in student schedule plan" << endl;
  file1.close();
  file2.close();

  return true;
}
