#include<iostream>
#include<string>
#include <tuple>
#include<wiringPi.h>
#include<limits.h>
#include<chrono>
#include<thread>

#define M1_IN1 0
#define M1_IN2 1

#define M2_IN1 3
#define M2_IN2 4

struct motor_inputs {
	int in1;
	int in2;
};

motor_inputs get_motors(int motor) {
	if(motor == 1) {
		return motor_inputs{M1_IN1, M1_IN2};
	}

	return motor_inputs{M2_IN1, M2_IN2};
}

void run_forwards(int motor) {
	motor_inputs mi = get_motors(motor);

	digitalWrite(mi.in1, LOW);
	digitalWrite(mi.in2, HIGH);
}

void run_back(int motor) {
	motor_inputs mi = get_motors(motor);

	digitalWrite(mi.in2, LOW);
	digitalWrite(mi.in1, HIGH);
}

void stop(int motor) {
	motor_inputs mi = get_motors(motor);

	digitalWrite(mi.in1, LOW);
	digitalWrite(mi.in2, LOW);
}


int main() {
	if (wiringPiSetup() == -1) {
		exit(1);
	}
	
	pinMode(M1_IN1, OUTPUT);
	pinMode(M1_IN2, OUTPUT);
	pinMode(M2_IN1, OUTPUT);
	pinMode(M2_IN2, OUTPUT);

	while(true) {
		std::string v;
		int m;
		std::cout << "Input command:";
		std::cin >> v;
		std::cout << "Input Motor (1,2):";
		std::cin >> m;

		if(v == "fwd") {
			std::cout << "running forwards" << std::endl;
			run_forwards(m);
		}else if(v == "stop") {
			std::cout << "STOPPING" << std::endl;
			stop(m);
		}else if(v == "bck") {
			std::cout << "running backwards maybe" << std::endl;
			run_back(m);
		}
	}
}
