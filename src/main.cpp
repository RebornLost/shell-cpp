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
 //remove spaces in front
 size_t first = parameters.find_first_not_of(" \t");
 if(first != std::string::npos){
      parameters = parameters.substr(first);
 }
//separate the input by spaces
 stringstream ss(parameters);
 string tokens;
 vector<string> inputcommands;

 while(getline(ss,tokens,' ')){
	 inputcommands.push_back(tokens);
 }
 
  for(auto it : builtin_commands){
    if( inputcommands[0] == it ){
         return true;
    }
  }
   return false;
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
    cerr << parameters << ": not found" << "\n";
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
void run_cmd(string userinput){ 	
//remove spaces in front
 size_t first = userinput.find_first_not_of(" \t");
 if(first != std::string::npos){
       userinput = userinput.substr(first);
 }
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
 cerr << inputcommands[0] << ": command not found" << "\n";
 exit(0);
 return;
 }
 vector<char*> char_commands;

 for(auto const  &x: inputcommands){
     char_commands.push_back(const_cast<char*>(x.c_str()));//convert to char*
 }

 char_commands.push_back(NULL); // last must be NULL
 execv(address.c_str(),char_commands.data());//execute command
 perror("execv");
}

void execute( string userinput){

   pid_t p;
   p = fork();//create child process
  if (p == 0){
    run_cmd(userinput); 
    exit(0);
  }
  else{
    wait(NULL);     
  }
}
void append_output(string cmd , string file_part){
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
    int fd = open(file_part.c_str() , O_WRONLY | O_CREAT | O_APPEND , 0664);
    dup2(fd , 1);
    close(fd);

    execute(cmd);
    std::cout.flush();

    dup2(saved_stdout , 1);
    close(saved_stdout);

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

void append_error(string cmd , string file_part){
 size_t first = file_part.find_first_not_of(" \t");
 size_t last = file_part.find_last_not_of(" \t");
 if(first != std::string::npos && last != std::string::npos){
       file_part = file_part.substr(first , last - first + 1);
 }

   std::filesystem::path p(file_part);
    if(p.has_parent_path()){
        std::filesystem::create_directories(p.parent_path());
    }

    int saved_stdout = dup(2);
    int fd = open(file_part.c_str() , O_WRONLY | O_CREAT | O_APPEND , 0664);
    dup2(fd , 2);
    close(fd);

    execute(cmd);
    std::cerr.flush();

    dup2(saved_stdout , 2);
    close(saved_stdout);

}



void redirect_error(string cmd , string file_part){
 size_t first = file_part.find_first_not_of(" \t");
 size_t last = file_part.find_last_not_of(" \t");
 if(first != std::string::npos && last != std::string::npos){
       file_part = file_part.substr(first , last - first + 1);
 }

   std::filesystem::path p(file_part);
    if(p.has_parent_path()){
        std::filesystem::create_directories(p.parent_path());
    }

    int saved_stdout = dup(2);
    int fd = open(file_part.c_str() , O_WRONLY | O_CREAT | O_TRUNC , 0664);
    dup2(fd , 2);
    close(fd);

    execute(cmd);
    std::cerr.flush();
    dup2(saved_stdout , 2);
    close(saved_stdout);

}
void piping(string left , string right){
    int pipefds[2];

    pipe(pipefds);
    
    pid_t p1 ;
    pid_t p2 ;     

        int saved_stdout = dup(1);
        int saved_stdin = dup(0);
   /* if(builtin(left) && builtin(right)){
        dup2(pipefds[1] , 1);
        dup2(pipefds[0] , 0);

        close(pipefds[0]);
        close(pipefds[1]);

        builtin_exe(left); 
        builtin_exe(right);

        dup2(saved_stdin , 0);
        close(saved_stdin);

        dup2(saved_stdout , 1);
        close(saved_stdout);

        return;
        }
    
    if(builtin(left)){
        dup2(pipefds[1] , 1);
        close(pipefds[0]);
        close(pipefds[1]);
        builtin_exe(left); 
    }
    else{
    */
    p1 = fork();

    if(p1 == 0){
        dup2(pipefds[1] , 1);
        close(pipefds[1]);
        close(pipefds[0]);  
        run_cmd(left);

        exit(0); //close the child to avoid it going into parent and not make a child inside the child
       }
   
    p2 = fork();

    if(p2 == 0){
        dup2(pipefds[0] , 0);
        close(pipefds[0]);
        close(pipefds[1]);
        run_cmd(right);

        exit(0);
       } 

        std::cout.flush();
        close(pipefds[0]);
        close(pipefds[1]);
        dup2(saved_stdout , 1);
        close(saved_stdout); 
        dup2(saved_stdin,0);
        close(saved_stdin);


        int status1;
        int status2;
        wait(&status1);
        wait(&status2);
   } 

//REPL
void REPL(){
  string userinput ;

  while(true){
  cout << "$ ";
  getline (cin,userinput);

  string command = userinput.substr(0, userinput.find(' '));
  string parameters = userinput.substr(userinput.find(' ')+1);

  if(command == "exit"){
       break;
  }
    
    //pipe
  size_t p_pos = userinput.find("|");
  if(p_pos != std::string::npos){
    std::string left = userinput.substr(0 , p_pos);
    std::string right = userinput.substr(p_pos + 1);
     size_t first = left.find_first_not_of(" \t");
     size_t last = left.find_last_not_of(" \t");
     if(first != std::string::npos && last != std::string::npos){
        left = left.substr(first , last - first + 1);
    }

    piping(left , right);
    continue;
  }

  //append error
  size_t a_e_pos = userinput.find("2>>");
   if(a_e_pos != std::string::npos){
    std::string left = userinput.substr(0 , a_e_pos);
    std::string right = userinput.substr(a_e_pos + 3);
    append_error(left , right);
    continue;
   }
  
  //redirect error
   size_t e_pos = userinput.find("2>");
   if(e_pos != std::string::npos){
    std::string left = userinput.substr(0 , e_pos);
    std::string right = userinput.substr(e_pos + 2);
    redirect_error(left , right);
    continue;
   }
  
   //append
   size_t a_pos = userinput.find("1>>");
   int a_op_len = 3;
   
   if(a_pos == std::string::npos){
     a_pos = userinput.find(">>");
     a_op_len = 2;
   }

   if(a_pos !=  std::string::npos ){
       std::string left = userinput.substr(0 , a_pos);
       std::string right = userinput.substr(a_pos + a_op_len);
     append_output(left , right);
     continue; 
   }

  //redirect output
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

 bool test = builtin(command);
  if (test == true){
    if(command == "exit"){
        break;
    }
     builtin_exe(userinput);
   }
  
  else
   execute(userinput);
   }  
}

int main(){
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    setvbuf(stdin,  NULL, _IONBF, 0);
cout << unitbuf; 
cerr << unitbuf;  

REPL();
return 0;
}
