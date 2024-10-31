#include <iostream>
#include <random>
#include <fstream>
#include <cmath>

const int k = 200; //coupling strength
const int N = 10;
const double pi = 3.141592653589793;
const double dt = 0.0001;

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

void theta_dot(Osc* oscillators) {
    for (int i = 0; i < N * N; i++) {
        double coupling_effect = 0.0;
        for (int j = 0; j < N * N; j++) {
            if (i != j) { // Avoid self-interaction
                coupling_effect += sin(oscillators[j].past_phase - oscillators[i].past_phase);
            }
        }
        coupling_effect *= (k / (N * N)); // Scale by coupling constant and number of oscillators
        oscillators[i].omega = 0.5 + coupling_effect; // Update omega with coupling
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