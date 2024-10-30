#include <iostream>
#include <random>
#include <fstream>

const int k = 10; //coupling strength
const int N = 100;
const float pi = 3.14;
const float dt = 0.0001;

struct Osc
{
    double curr_phase, past_phase;
    double omega;
};

void initialize(Osc* oscillators){
    using namespace std;

    default_random_engine generator;
    uniform_real_distribution<double> distribution(0,pi);

    for (int i=0; i< N*N; i++) {
        oscillators[i].past_phase = distribution(generator);
        oscillators[i].omega = 0.5;
    }
}

void theta_dot(Osc* oscillators){
    for (int i=0; i<N*N; i++) {
        for (int j=0; j<N*N; j++) {
            oscillators[i].omega = oscillators[i].omega + (k/N*N)*sin(oscillators[j].past_phase - oscillators[i].past_phase);
        }
    }
}

void new_phase(Osc* oscillators){
    for (int i=0; i<N*N; i++) {
        oscillators[i].curr_phase = oscillators[i].past_phase + oscillators[i].omega*dt;
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

    int tot_time = 20;
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