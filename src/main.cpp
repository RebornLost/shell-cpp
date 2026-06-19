#include <iostream>
#include <string>

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
    cout << input.string(5,strlen(input)-5) << "\n" ;
    break ;
  }    

    cerr << input << ": command not found" << "\n" ;
  }

  return 0;

}
