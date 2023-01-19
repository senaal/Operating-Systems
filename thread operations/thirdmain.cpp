#include <iostream>
#include <vector>
#include <valarray>
#include <fstream>
#include <pthread.h>
#include <chrono>
#include <iomanip>

using namespace std;
int maximum,minimum,range_list,mode_list;
double median_list, sum_list, arm, harm, st_d, iq_r;

int N;
vector<int> list;
void min(void* argv){
    int x = list.front();
    minimum = x;
}
void max(void* argv){
    int x = list.back();
    maximum = x;
}
void range(void* argv){
    int x = list.back() - list.front();
    range_list = x;
}
void mode(void* argv){
    int pivot = list.front();
    int position = list.front();
    int repeated_number = 1;
    int final = 1;
    for(int i = 0; i < (list.size() - 1); i++){
        if(list[i+1] == pivot){
            repeated_number += 1;
        }
        else{
            repeated_number = 1;
            pivot = list[i+1];
        }
        if(repeated_number > final){
            position = pivot;
            final = repeated_number;
        }
    }
    mode_list = position;
}
void median(void* argv){
    double med;
    if(N%2 == 0){
        double mod1 = list[(N/2) - 1];
        double mod2 = list[N/2];
        med = (mod1 + mod2)/2;
    }
    else{
        med = list[((N+1)/2) - 1];
    }
    median_list = med;
}
void sum(void* argv){
    double sum = 0;
    for(int i : list){
        sum+= i;
    }
    sum_list = sum;
}
void arithmetic_mean(void* argv){
    double numb = list.size();
    double sum = 0;
    for(int i : list){
        sum+= i;
    }
    double ar_mean = sum/numb;
    arm = ar_mean;
}
void harmonic_mean(void* argv){
    double sum = 0;
    double one = 1;
    double numb = list.size();

    for(int i : list){
        sum += one/i;
    }
    double har_mean = numb / sum;
    harm = har_mean;
}
void standard_deviation(void* argv){
    if(list.size() == 1){
        st_d = 0;
    }

    else{
        double numb = list.size();
        double sum = 0;
        double std_sum = 0;
        for(int i : list){
            sum+= i;
        }
        double mean = sum/numb;
        for(int i : list){
            std_sum += (i - mean) * (i - mean);
        }
        double fin = sqrt(std_sum / (numb - (double)1));
        st_d = fin;
    }
}
void interquartile_range(void* argv){
    if(N == 1){
        iq_r = list[0];
    }
    double med1, med2;
    int indFirst, indSecond, ind, ind2;
    if((N%4 == 0) || ((N-1)%4 == 0)){
        ind = N/4;
        indFirst = ind - 1;
        ind2 = N - ind - 1;
        indSecond = N - indFirst - 1;
        med1 = (double)(list[ind] + list[indFirst])/(double)2;
        med2 = (double)(list[ind2] + list[indSecond])/(double)2;
    }
    else{
        indFirst = N/4;
        indSecond = (3*N)/4;
        med1 = list[indFirst];
        med2 = list[indSecond];
    }
    double val = med2-med1;
    iq_r = val;
}
void* momax(void* argv){
    mode(argv);
    max(argv);
    pthread_exit(nullptr);
}
void* sumran(void* argv){
    sum(argv);
    range(argv);
    pthread_exit(nullptr);
}
void* armharm(void* argv){
    arithmetic_mean(argv);
    harmonic_mean(argv);
    pthread_exit(nullptr);
}
void* stmin(void* argv){
    standard_deviation(argv);
    min(argv);
    pthread_exit(nullptr);
}
void* mediqr(void* argv){
    median(argv);
    interquartile_range(argv);
    pthread_exit(nullptr);
}

int main(int argc,char* argv[]) {
    ofstream out ("output3.txt");
    N = atoi(argv[1]);
    srand (time(nullptr));

    for(int i = 0; i < N;i++){
        list.push_back(rand()%9001 + 1000);
    }
    sort(list.begin(), list.end());
    auto start = chrono::steady_clock::now();
    pthread_t pid0,pid1,pid2,pid3,pid4;
    pthread_create(&pid0, nullptr,momax , nullptr);
    pthread_join( pid0, nullptr);
    pthread_create(&pid1, nullptr, sumran, nullptr);
    pthread_join( pid1, nullptr);
    pthread_create(&pid2, nullptr, armharm, nullptr);
    pthread_join( pid2, nullptr);
    pthread_create(&pid3, nullptr, stmin, nullptr);
    pthread_join( pid3, nullptr);
    pthread_create(&pid4, nullptr, mediqr, nullptr);
    pthread_join( pid4, nullptr);
    out << minimum << endl;
    out << maximum << endl;
    out << range_list << endl;
    out << mode_list << endl;
    out << fixed << setprecision(5) << median_list<< endl;
    out << fixed << setprecision(0) << sum_list << endl;
    out << fixed << setprecision(5) << arm << endl;
    out << fixed << setprecision(5) << harm << endl;
    out << fixed << setprecision(5) << st_d << endl;
    out << fixed << setprecision(5) << iq_r << endl;
    auto end = chrono::steady_clock::now();
    double time_taken
            = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    time_taken *= 1e-9;
    out << fixed << setprecision(5) << time_taken << endl;
    return 0;
}
