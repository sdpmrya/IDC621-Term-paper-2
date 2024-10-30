#include <iostream>
#include <random>
#include <fstream>
#include <cmath>

const int k = 10; //coupling strength
const int N = 10;
const float pi = 3.141592653589793;
const float dt = 0.0001;

struct Osc
{
    double curr_phase, past_phase;
    double omega;
};

void initialize(Osc* oscillators){
    using namespace std;

    default_random_engine generator;
    uniform_real_distribution<double> distribution(0,2*pi);

    for (int i=0; i< N*N; i++) {
        oscillators[i].past_phase = distribution(generator);
        oscillators[i].omega = 0.5;
    }
}

void theta_dot(Osc* oscillators){
    for (int i=0; i<N*N; i++) {
        float sum = 0.0;
        for (int j=0; j<N*N; j++) {
            sum += sin(oscillators[j].past_phase - oscillators[i].past_phase);
        }
        oscillators[i].omega = oscillators[i].omega + (k/N*N)*sum;
    }
}

void new_phase(Osc* oscillators){
    for (int i=0; i<N*N; i++) {
        oscillators[i].curr_phase =  fmod(oscillators[i].past_phase + oscillators[i].omega * dt, 2 * pi);
    }
}

void reassign(Osc* oscillators){
    for (int i=0; i<N*N; i++){
        oscillators[i].past_phase = oscillators[i].curr_phase;
    }
}

int main(){
    Osc* oscillators = new Osc[N*N];
    initialize(oscillators);

    std::ofstream outFile("Kuramoto_data.txt");

    int tot_time = 1;
    int num_steps = tot_time/dt;

    for (int i=0; i<num_steps; i++) {
        theta_dot(oscillators);
        new_phase(oscillators);

        for (int j=0; j<N*N; j++) {
            outFile << oscillators[j].curr_phase << " ";
        } outFile << "\n";

        reassign(oscillators);
    }
    outFile.close();
    return 0;
    delete[] oscillators;
}