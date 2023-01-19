#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <pthread.h>
#include <mutex>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;
ofstream outputfile;
const int TH = 10;
pthread_t pid[TH];
pthread_mutex_t mutMachine[10]; //mutex ids for machines
pthread_mutex_t mutCompany[5]; //mutex ids for companies
int check;

class Company { //company object
    public:
    string name;
    int totalBalance;
    }; 

class Customer { //customer object
    public:
    int id;
    int sleep_time;
    int venting_machine;
    int company_ind;
    int money;
    };

class Machine { //customer object
    public:
    int id;
    int cust_id;
    bool busy;
    };     

Company companies[5] = {{"Kevin",0}, {"Bob", 0}, {"Stuart", 0}, {"Otto", 0}, {"Dave", 0}};
Machine machines[10] = {{0, 0, false},{1, 0, false},{2, 0, false},{3, 0, false},{4, 0, false},{5, 0, false},{6, 0, false},{7, 0, false},{8, 0, false},{9, 0, false}};
vector<Customer> customers;

void *customer_thread(void *arg){   
    Customer cust = *((Customer*)arg); 
    int sleep_t = cust.sleep_time;
    usleep(sleep_t*1000); //sleeps each customer in sleep time miliseconss
    int mac_id = cust.venting_machine-1;
    pthread_mutex_lock(&mutMachine[mac_id]); //first comes locks the thread which id is equals to the customers venting machine
    machines[mac_id].cust_id = cust.id; //set the machines cust id to the executing customer id
    machines[mac_id].busy = true; //change busy status of the machine to true
    pthread_exit(nullptr);
    };

void *machine_thread(void *arg){ //vending machine function
while (check>0){
    Machine vmachine = *((Machine*) arg);
    bool availability = machines[vmachine.id].busy;
    int cust_id = machines[vmachine.id].cust_id;

    if (availability){ //checks a thread has a customer or not if has execute customer
        Customer cust = customers[cust_id-1];        
        pthread_mutex_lock(&mutCompany[cust.company_ind]); //locks the company until change the total balance of company
        companies[cust.company_ind].totalBalance += cust.money; //increases the company's total balance as the money of customer
        outputfile << "[vtm" << vmachine.id + 1 <<"]: " << "Customer" << cust_id << "," << cust.money <<"TL," << companies[cust.company_ind].name << endl;
        pthread_mutex_unlock(&mutCompany[cust.company_ind]); //unlocks the company
        machines[vmachine.id].busy = false; //busy status of machine to the false
        machines[vmachine.id].cust_id = 0; //id of customer in the machine is 0 which means thread is empty
        check -= 1; //executed customer decreases
        pthread_mutex_unlock(&mutMachine[machines[vmachine.id].id]); //release the machine and takes the other customer which has venting machine is equal to the id of machine       
        }
    }
    pthread_exit(nullptr);
};


int comp_conv(string compName){ //convert string company names to the indexes
    if(compName == "Kevin"){
        return 0;
    }
    else if(compName == "Bob"){
        return 1;
    }
    else if(compName == "Stuart"){
        return 2;
    }
    else if(compName == "Otto"){
        return 3;
    }
    else if(compName == "Dave"){
        return 4;
    }
    return -1;
}

int main(int argc, char** argv){
    ifstream inputfile; //input reading
    inputfile.open(argv[1]);
    string line;
    vector<string> words;
    getline(inputfile, line);
    int numberOfCust = stoi(line); //number of customers
    check = numberOfCust;
    string nameF = argv[1];
    string outputfileName = nameF + "_log.txt";
    outputfile.open(outputfileName);
    while (getline(inputfile, line)){ //read line by line, then split by comma
        stringstream ss(line);
        string word;
        while(getline(ss, word, ',')){
            words.push_back(word);
        }
    }

    int id = 1;
    for(int j = 0; j < words.size(); j+=4){ //creates customer objects with id, sleep time, comp id, money and venting machine
        Customer cust;
        cust.id = id;
        cust.sleep_time = stoi(words[j]);
        cust.venting_machine = stoi(words[j+1]);
        string comp = words[j+2];
        cust.company_ind = comp_conv(comp);
        cust.money = stoi(words[j+3]);
        customers.push_back(cust);
        id++; 
    }

    inputfile.close();

    for (int i = 0; i < 10; i++){ //initialize machine mutexes
        pthread_mutex_init(&mutMachine[i],nullptr);
    }
    for (int i = 0; i < 5; i++){  //initialize company mutexes
        pthread_mutex_init(&mutCompany[i],nullptr);
    }    
    for(int i = 0; i < TH; i++){
        pthread_create(&pid[i], nullptr, &machine_thread, &machines[i]); //thread creation for each vending machine
    }

    pthread_t cust_pid[numberOfCust];

    for(int i = 0; i < numberOfCust; i++){
        pthread_create(&cust_pid[i], nullptr, &customer_thread, &customers[i]); //thread creation for each customer
    }
    for(int i = 0; i < numberOfCust; i++){ //join all customers
        pthread_join(cust_pid[i], nullptr);
    }
    for(int i = 0; i < 10; i++){ //join all machines
        pthread_join(pid[i], nullptr);
    }
    outputfile << "[Main]: All prepayments are completed." <<endl;

    for(int i = 0; i < 5; i++){
        outputfile << "[Main]: " << companies[i].name << ": " << companies[i].totalBalance<< endl;
    }
}