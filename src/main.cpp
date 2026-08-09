#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

string builtin_commands[] = {"echo", "exit", "type"};
//PATH finder
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

//builtin checker
bool builtin(string parameters){
  bool is_builtin = false;
  for(auto it : builtin_commands){
  if( parameters == it )
  {cout << it << " is a shell builtin" << "\n"; 
  is_builtin = true;
  return is_builtin;}
  }
   return is_builtin;
}


//echo command
void echo(string output){
  cout << output << "\n";
  return;
}
//type command
void type(string parameters){
  bool is_builtin = builtin(parameters);

  if(!is_builtin){
 
  string path_result = PATH(parameters);
  
  if (path_result != "not found") {
    cout << parameters << " is " << path_result << "\n";
  } else {
    cout << parameters << ": not found" << "\n";
  }
}
  return;
}

//execute command
void execute(string userinput){ 
	
	
//separate the input by spaces
 stringstream ss(userinput);
 string tokens;
 vector<string> inputcommands;

 while(getline(ss,tokens,' ')){
	 inputcommands.push_back(tokens);
 }

 bool test = builtin(inputcommands[0]);
 if (test == true){
 return;
 }
 string address= PATH(inputcommands[0]);
 if(address == "not found"){
 cout << inputcommands[0] << ": command not found" << "\n";
 return;
 }
 vector<char*> char_commands;

 for(auto const  &x: inputcommands){
 char_commands.push_back(const_cast<char*>(x.c_str()));//convert to char*
 }
 char_commands.push_back(NULL);

 pid_t p;
	p = fork();//create child process

  if (p == 0){
  execv(address.c_str(),char_commands.data());//execute command
  }
 
  if(p > 0){
  int status;
  wait(&status);
  return;
  }
}
//REPL
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
    echo(parameters);
  }
  else if (command == "type")
  {
    type(parameters);
  }
   else{
   execute(userinput);}
 }  
}

int main(){
cout << unitbuf; 
cerr << unitbuf;  

REPL();
return 0;
}
