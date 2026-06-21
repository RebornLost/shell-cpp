#include <iostream>
#include <string>
#include <cstring>

using namespace std;

int main()
{

  cout << std::unitbuf;
  cerr << std::unitbuf;

  while (true)
  {

    cout << "$ ";

    string input;

    getline(cin, input);

    string command = input.substr(0, input.find(' '));
    string parameters = input.substr(input.find(' ') + 1);

    if (command == "exit")
    {
      break;
    }

    else if (command == "echo")
    {
      cout << parameters << "\n";
    }

    else if (command == "type")
    {
      if (parameters == "echo" || parameters == "exit" || parameters == "type")
      {
        cout << parameters << " is a shell builtin" << "\n";
        
      }
      else
        cout << parameters << ": not found"<<"\n";
    }

    else
    {
      cerr << input << ": command not found" << "\n";
    }

    }
  return 0;
  }
