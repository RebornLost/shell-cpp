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
    if( input == "exit") {break;}
 
    //echo command
  if(input.substr(0,5) == "echo "){
    cout << input.substr(5) << "\n";
  }

  if(input.substr(0,5) == "type "){
    if (input.substr(5) == "echo"){cout << "echo is a shell builtin" <<"\n"; }
    else if (input.substr(5) == "exit"){cout << "exit is a shell builtin" <<"\n"; }
    else if (input.substr(5) == "type"){cout << "type is a shell builtin" <<"\n";
  }
 
  //command not found    
  else {
    cerr << input << ": command not found" << "\n" ;
  }
}
  return 0;

}
