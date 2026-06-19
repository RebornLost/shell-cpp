#include <iostream>
#include <string>

using namespace std;

int main() {
  
  cout << std::unitbuf;
  cerr << std::unitbuf;

   string input;
  
  while(true){

    cout << "$" ; 

    getline(cin , input);

    cerr << input << ":command not found" << "\n" ;
  }

  return 0;

}
