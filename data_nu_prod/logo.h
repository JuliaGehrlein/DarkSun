#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void printlogo(void){
ifstream ifs("logo.txt");
if(!ifs)
  {
    cout << "File open failed!\n";
  }

string str;
while(getline(ifs, str))
  {
    cout << str << endl;
  }
};
