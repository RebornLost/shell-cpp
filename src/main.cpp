#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fstream>
#include <filesystem>

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
  if( parameters == it ){
  is_builtin = true;
  return is_builtin;
  }
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

  if(is_builtin)
  {
      cout << parameters << " is a shell builtin" << "\n";
  }
  

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

void builtin_exe(string userinput){
 string command = userinput.substr(0, userinput.find(' '));
 string parameters = userinput.substr(userinput.find(' ')+1);
 if (command == "exit")
  {
    return ;
  }
  else if (command == "echo")
  {
    echo(parameters);
  }
  else if (command == "type")
  {
    type(parameters);
  }
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
 builtin_exe(userinput);
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

void redirect_right(string cmd , string file_part){
 size_t first = file_part.find_first_not_of(" \t");
 size_t last = file_part.find_last_not_of(" \t");
 if(first != std::string::npos && last != std::string::npos){
       file_part = file_part.substr(first , last - first + 1);
 }

   std::filesystem::path p(file_part);
    if(p.has_parent_path()){
        std::filesystem::create_directories(p.parent_path());
    }

    int saved_stdout = dup(1);
    int fd = open(file_part.c_str() , O_WRONLY | O_CREAT | O_TRUNC , 0664);
    dup2(fd , 1);
    close(fd);

    execute(cmd);
    std::cout.flush();

    dup2(saved_stdout , 1);
    close(saved_stdout);

}



//REPL
void REPL(){
  string userinput ;

  while(true){
  cout << "$ ";
  getline (cin,userinput);

  string command = userinput.substr(0, userinput.find(' '));
  string parameters = userinput.substr(userinput.find(' ')+1);

   size_t pos = userinput.find("1>");
   int op_len = 2;
   if(pos == std::string::npos){
     pos = userinput.find(">");
     op_len = 1;
   }

   if(pos !=  std::string::npos ){
     std::string cmd = userinput.substr(0 , pos);
     std::string file_part = userinput.substr(pos + op_len);
     redirect_right(cmd , file_part);
     continue; 
   }
if(command == "exit"){
      break;
  }
 bool test = builtin(command);
  if (test == true){
     builtin_exe(userinput);
   }
  
  else
   execute(userinput);
   }  
}

int main(){
cout << unitbuf; 
cerr << unitbuf;  

REPL();
return 0;
}
