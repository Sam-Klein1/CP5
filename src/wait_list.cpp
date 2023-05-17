#include <deque>
#include <fstream>
#include <iostream>
#include <list>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

bool prereq_file_check(string inputFile);
bool schedule_file_check(string input1, string input2);

class Student{
  public:
  int priorityPoints;
  string Bnumber;
  vector<int> waitLists;

  Student(int points, string Bnumber){
    this->priorityPoints = points;
    this->Bnumber = Bnumber;
  }
  void addList(int courseNum){
    waitLists.push_back(courseNum);
  }
  void removeList(int courseNum){
    auto wait_itr = waitLists.begin();
    while((*wait_itr != courseNum) && (wait_itr != waitLists.end())){
      wait_itr++;
    }
    if(wait_itr != waitLists.end()){
      waitLists.erase(wait_itr);
    }
  }
  void printStudent(){
    cout << Bnumber << " with " << priorityPoints << " points." << endl;
  }
};

void maxHeapify(vector<Student>& StudentList, int size, int index) {
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;
  
    if (left < size && StudentList[left].priorityPoints > StudentList[largest].priorityPoints)
        largest = left;
  
    if (right < size && StudentList[right].priorityPoints > StudentList[largest].priorityPoints)
        largest = right;
  
    if (largest != index) {
        swap(StudentList[index], StudentList[largest]);
        maxHeapify(StudentList, size, largest);
    }
}
  
void buildMaxHeap(vector<Student>& StudentList) {
    for (int index = StudentList.size() / 2 - 1; index >= 0; index--)
        maxHeapify(StudentList, StudentList.size(), index);
}

Student extractMax(vector<Student>& StudentList){
  
  if(StudentList.empty())
  {
    //cout << "trying to enroll course containg no Students in wait list" << endl;
    return Student(-10, "bNULL"); // return arbitrary Student object
  }

  Student temp = StudentList[0]; //"extract max"
  StudentList[0] = StudentList[StudentList.size()-1];//replace root w last element
  StudentList.erase(StudentList.begin() + (StudentList.size() - 1));
  maxHeapify(StudentList, StudentList.size(), 0);//maxheapify

  return temp;

  
}
int main(int argc, char *argv[]) {

  if(argc != 5){
    cout << "Argument Error: Format should be  './waitlist <semester> <prereqfile> <schedulefile> <enrollmentfile>'" << endl;
    return 0;
  }

  //Check prereqs and schedule, halt if its not valid
  if(!prereq_file_check(argv[2])){
    cout << "Can't run waitlist, prereq input file not viable" << endl;
    return 0;
  }

  ifstream schedulesFile(argv[3]);
  string semester = argv[1];
  
  string bNumber;
  string line;
  string scheduleFile;

  while (getline(schedulesFile, line) ) {
        stringstream ss(line);
        
        ss >> bNumber; 
        ss >> scheduleFile;
        scheduleFile = "input_files/" + scheduleFile;
        
        if(!schedule_file_check(argv[2], scheduleFile)){
          cout << "Can't run waitlist, schedule input files not viable" << endl;
          return 0;
        }
  }

  ifstream enrollFile(argv[4]);
  string line2, command, courseName; //bNumber declared above, just write over it
  int priority; 
  vector<Student> StudentList;

  unordered_map<string, vector<Student>> course_heaps;
  
  while (getline(enrollFile, line2))
  {

    stringstream ss(line2);

    ss >> command;
    if(command == "newlist"){
        //Create new MAX_heap with course name; "newlist CS_310"
        ss >> courseName;
        course_heaps[courseName];
    }
    else if(command == "add"){
        //Add Student to existing course with priority points; "add B00000001 CS_310 10"
        ss >> bNumber >> courseName >> priority;
        Student newStudent(priority, bNumber); //Create Student to put into heap
        course_heaps[courseName].push_back(newStudent); //Put Student in existing heap
        buildMaxHeap(course_heaps[courseName]); //Heapify course with new Student
    }
    else if (command == "promote"){
        //Add priority points to existing Students in existing course; "promote B00000001 CS_310 15"
        ss >> bNumber >> courseName >> priority;
        auto map_iter = course_heaps[courseName].begin();
        while((*map_iter).Bnumber != bNumber && map_iter != course_heaps[courseName].end())
          map_iter++;
        if(map_iter == course_heaps[courseName].end())
          cout << "Student " << bNumber << " doesn't exist in course" << courseName << endl;
        else{
          (*map_iter).priorityPoints += priority;
          buildMaxHeap(course_heaps[courseName]);
        }

        
    }
       else if (command == "enroll"){
        //Enrolls Student Student at the top of the MAX_heap in the existing course, setting his points to 0 everywhere else; "enroll CS_310"
        ss >> courseName;
        //Find heap, extract MAX
        vector<Student> maxHeap = course_heaps[courseName]; // Can't do this, need to either pass course_heaps in or reassign that heap after the function
        Student student = extractMax(maxHeap);
        course_heaps[courseName] = maxHeap;

        //Find Student's schedule in schedules.txt,
        string bNumber;
        string line;
        string scheduleFile;
        while (getline(schedulesFile, line) ) {
          stringstream ss(line);
        
          ss >> bNumber; 
          if(bNumber == student.Bnumber)
            ss >> scheduleFile;
        } 
        //If he meets prereq requirments, ouput to console their enrollment; 
        if(student.Bnumber != "bNULL")
          cout << "Enrolling Student " << student.Bnumber << " in course " << courseName << endl;

        //Set Students priority points in other courses to 0
        auto map_itr = course_heaps.begin(); //Iterator to all course heaps
        while(map_itr != course_heaps.end())
        {
          for(int i = 0; i < (*map_itr).second.size(); i++)
          {
            if((*map_itr).second.at(i).Bnumber == student.Bnumber) //If student is found in the course
            {
              (*map_itr).second.at(i).priorityPoints = 0; //Set his points to 0
              buildMaxHeap(course_heaps[(*map_itr).first]);//Heapify the course to reflect new points
              break; //Exit for loop to next class
            }
          }
          map_itr++;
        }
    }
    else{
        cout << "unrecognizable command" << endl;
    }
  }

  return 0;
}