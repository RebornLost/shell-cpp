#include <iostream>
#include <string>

using namespace std;

int main() {
  
  cout << std::unitbuf;
  cerr << std::unitbuf;

  cout << "$ " ; 
  
  string a;
   
  cin >> a;

  cerr << a << ": command not found" ;


  return 0;

}
