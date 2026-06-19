#include <iostream>
#include <string>
#include<cstring>

using namespace std;

int main() {
  
  cout << std::unitbuf;
  cerr << std::unitbuf;
  
  while(true){

    cout << "$ " ; 

    string input;

    getline(cin , input);
 // exit command
    if( input == "exit") {
      break;
    }
//echo command
  if(input.substr(0,5) == "echo "){
    int a = strlen(input);
    a -= 5;
    cout << input.substr(5,a) << "\n" ;
    break ;
  }    

    cerr << input << ": command not found" << "\n" ;
  }

  return 0;

}
