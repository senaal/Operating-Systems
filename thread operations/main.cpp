#include <iostream>
#include <vector>
#include <valarray>
#include <fstream>
#include <chrono>
#include <iomanip>

using namespace std;
int N;
int max(vector<int> list){
    int max = list.back();
    return max;
}
int min(vector<int> list){
    int min = list.front();
    return min;
}
int range(const vector<int>& list){
    int range = max(list) - min(list);
    return range;
}
int mod(vector<int> list){
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
    return position;
}
float median(vector<int> list){
    float med;
    if(N%2 == 0){
        float mod1 = list[(N/2) - 1];
        float mod2 = list[N/2];
        med = (mod1 + mod2)/2;
    }
    else{
        med = list[((N+1)/2) - 1];
    }
    return med;
}
double sum(vector<int> list){
    double sum = 0;
    while(!list.empty()){
        sum += list.back();
        list.pop_back();
    }
    return sum;
}

float arithmetic_mean(vector<int> list){
    float numb = N;
    float ar_mean = sum(list)/ numb;
    return ar_mean;
}
double harmonic_mean(vector<int> list){
    double product = 0;
    double one = 1;
    while(!list.empty()){
        product += one/(list.back());
        list.pop_back();
    }
    double numb = N;
    double har_mean = N / product;
    return har_mean;

}
double standard_deviation(vector<int> list){
    if(N == 1){
        return 0;
    }
    double sum = 0;
    double mean = arithmetic_mean(list);
    double numb = list.size();
    while(!list.empty()){
        double pivot = list.back();
        double value = (pivot-mean)*(pivot-mean);
        sum += value;
        list.pop_back();
    }
    return sqrt(sum/(numb-(double)1));
}
double interquartile_range(vector<int> list){
    if(N == 1){
        return list[0];
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
    return med2-med1;
}

int main(int argc,char* argv[]) {
    N = atoi(argv[1]);
    srand (time(NULL));
    ofstream out ("output1.txt");
    vector<int> list;
    for(int i = 0; i < N;i++){
        list.push_back(rand()%9001 + 1000);
    }
    sort(list.begin(), list.end());
    auto start = chrono::steady_clock::now();
    out << min(list) << endl;
    out << max(list) << endl;
    out << range(list) << endl;
    out << mod(list) << endl;
    out << fixed << setprecision(5) << median(list) << endl;
    out << fixed << setprecision(0) << sum(list) << endl;
    out << fixed << setprecision(5) << arithmetic_mean(list) << endl;
    out << fixed << setprecision(5) << harmonic_mean(list) << endl;
    out << fixed << setprecision(5) << standard_deviation(list) << endl;
    out << fixed << setprecision(5) << interquartile_range(list) << endl;
    auto end = chrono::steady_clock::now();
    double time_taken
            = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    time_taken *= 1e-9;
    out << fixed << setprecision(5) << time_taken << endl;
    return 0;
}
