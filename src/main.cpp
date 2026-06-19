#include <iostream>
#include <string>

using namespace std;

int main() {
  
  cout << std::unitbuf;
  cerr << std::unitbuf;

  string input;
  
  start:

  cout << "$ " ; 
   
  getline(cin , input);

  cerr << input << ": command not found" << "\n" ;
  
  goto start;

  return 0;

}
