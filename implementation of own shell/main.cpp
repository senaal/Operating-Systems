#include <iostream>
#include <vector>
#include <unistd.h>
#include <wait.h>
#include <cstring>
#include <fstream>
//Fatma Sena Alci-2019400045
char* username = std::getenv("USER");
static std::vector <std::string> vecOfWords;
//parse input string using char and string concatenation
void ParseStr( const std::string& line) {
    std::string parse;
    int i = 0;
    for (char a : line){
        i++;
        if (a != ' '){
            if(i == line.length()){
                parse  += a;
                vecOfWords.push_back(parse);
                parse  = "";
            }
            else{
                parse  +=  a;
            }
        }
        else
        {
            vecOfWords.push_back(parse);
            parse = "";
        }
    }
}

//after using printfile, make a single element from all other elements in line except dididothat
std::string printCheck(std::vector <std::string> list){
    if(list.size() == 2){
        return vecOfWords[1];
    }
    if(vecOfWords[1] == "printfile" || vecOfWords[1] == "\"printfile"){
        if(list.size() == 3){
            std:: string checker = vecOfWords[1] + " " + vecOfWords[2];
            return checker;
        }
        if(list.size() == 5){
            std:: string checker = vecOfWords[1] + " " + vecOfWords[2] + " " + vecOfWords[3] + " " + vecOfWords[4];
            return checker;
        }
    }
    return vecOfWords[1];
}

//takes input and for commands call fork and exec functions.
int main() {
    std::vector <std::string> dididothatCheck;
    while(true){
        std:: cout << username << " >>> ";
        std:: string line;
        std:: getline(std::cin, line);
        ParseStr(line);
        std:: string command = vecOfWords[0];


        char *argument_list[] = {};

        if (command == "printfile") { //for printfile command with 1 argument while loop reads file and write to the shell
            if (vecOfWords.size() == 2) {
                dididothatCheck.push_back(line); //add line to history array
                std::ifstream file(vecOfWords[1]);
                std::string line2;
                while (getline(file, line2)) {
                    std::cout << line2;
                    if (std::cin.get() != '\n') {
                        continue;
                    }
                }
                vecOfWords.clear();
                command.clear();
                line.clear();
            }
        }
        pid_t  pid;
        pid = fork();
        //create child process with fork
        if (command == "exit") {
            break;
        }

        if (pid == 0) { //child processes
            if (command == "listdir") { //for listdir command exec executes ls command
                argument_list[0] = strdup("ls");
                argument_list[1] = nullptr;
                execvp("ls", argument_list);

            }
            if (command == "mycomputername") { //for mycomputername command exec executes hostname command and gives computer name
                argument_list[0] = strdup("hostname");
                argument_list[1] = nullptr;
                execvp("hostname", argument_list);

            }
            if (command == "whatsmyip") { //for whatsmyip command exec executes hostname command and gives ip of computer
                argument_list[0] = strdup("hostname");
                argument_list[1] = strdup("-i");
                argument_list[2] = nullptr;
                execvp("hostname", argument_list);
            }

            if (command == "printfile") { //for printfile command with 2 arguments exec executes cp command and copy first file to second file
                argument_list[0] = strdup("cp");
                argument_list[1] = strdup(vecOfWords[1].data());
                argument_list[2] = strdup(vecOfWords[3].data());
                argument_list[3] = nullptr;
                execvp("cp",argument_list);
            }

            if (command == "hellotext") { //for hellotext command exec executes gedit command and opens text editor
                argument_list[0] = strdup("gedit");
                argument_list[1] = nullptr;
                execvp("gedit", argument_list);
            }
            if (command == "dididothat") { //for dididothat command, for loop executes 15 times and checks history if it cannot find process in history, writes "No".
                bool boom = true;
                for(int i = 0; i < dididothatCheck.size() && i < 15; i++){
                    if(dididothatCheck.size()>15) //if history size is bigger than 15, it looks last 15 elements
                    {   //control and control2 elements are history elements. control is normal, other one is "control".
                        std:: string control = dididothatCheck[dididothatCheck.size()-15+i];
                        std:: string control2 = "\"" + dididothatCheck[dididothatCheck.size()-15+i] + "\"";
                        if (vecOfWords.size() > 2 ) {
                            std:: string checker = printCheck(vecOfWords);
                            if (( checker == control || checker  == control2)) {
                                boom = false;
                            }
                        }
                        else{ //dididothat printfile should give "No".
                            if(vecOfWords[1] != "printfile" && vecOfWords[1] != "\"printfile\"" && (vecOfWords[1] == control || vecOfWords[1] == control2)){
                                boom = false;
                            }
                        }
                    }
                    else{ //same process for maximum 15 history elements
                        std:: string control = dididothatCheck[i];
                        std:: string control2 = "\"" + dididothatCheck[i] + "\"" ;

                        if (vecOfWords.size() > 2 ) {
                            std:: string checker = printCheck(vecOfWords);
                            if (( checker == control || checker  == control2)) {
                                boom = false;
                            }
                        }
                        else{
                            if(vecOfWords[1] != "printfile" && vecOfWords[1] != "\"printfile\"" && (vecOfWords[1] == control || vecOfWords[1] == control2)){
                                boom = false;
                            }
                        }
                    }
                }
                vecOfWords.clear();//clear all static variables
                command.clear();
                line.clear();
                if(boom){
                    std::cout << "No" << std::endl;
                    exit(0);
                }
                else{
                    std::cout << "Yes" << std::endl;
                    exit(0);
                }
            }

            vecOfWords.clear();
            command.clear();
            line.clear();
            exit(0);
        }

        if(pid < 0){ // an error occurs
            continue;
        }
        if( pid > 0 ){ //parent process
            dididothatCheck.push_back(line);//add line to history array for all commands except printfile with 1 argument
            wait(nullptr);
            vecOfWords.clear();
            command.clear();
            line.clear();
            continue;
        }
    }
}
