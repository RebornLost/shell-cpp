#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include <unistd.h>

using namespace std;
/*
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

      const char* env_p = getenv("PATH");
      cout << env_p << "\n";

      

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
*/
string builtin_commands[] = {"echo", "exit", "type"};

string PATH(string command){
  const char* env_p = getenv("PATH");
  //cout << env_p << "\n";
  stringstream ss(env_p);
  string tokens;
  vector<string> directory;
  
   while(getline(ss, tokens, ':' )){
    directory.push_back(tokens);
  }
  for (auto it: directory){
    string updated_path = it + "/" + command;
    if(access(updated_path.c_str() , X_OK)==0){
    return updated_path;}
  }
  return "not found";
  
}
void REPL(){
  string userinput ;

  while(true){
  cout << "$ ";
  getline (cin,userinput);

  string command = userinput.substr(0, userinput.find(' '));
  string parameters = userinput.substr(userinput.find(' ')+1);
  
  if (command == "exit")
  {
    return;
  }
  else if (command == "echo")
  {
    cout << parameters << "\n";
  }
  else if(command == "type"){
    bool is_builtin = false;
    for (const auto& cmd : builtin_commands) {
      if (cmd == parameters) {
        is_builtin = true;
        break;
      }
    }
    if (is_builtin) {
      cout << parameters << " is a shell builtin" << "\n";
    } else {
      string path_result ;
      path_result = PATH(parameters);

      if (path_result != "not found") {
        cout << parameters << " is " << path_result << "\n";
      } else {
      cout << parameters << ": not found" << "\n";}
    }
  }
  else {
    cout << "" << command << ": command not found" << "\n";
  }
  }
}
int main(){
  
REPL();



return 0;
}