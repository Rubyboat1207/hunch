#include<iostream>
#include<wiringPi.h>
#include<limits.h>
#include<chrono>
#include<thread>


int main() {
	if (wiringPiSetupGpio() == -1) {
		exit(1);
	}

	pinMode(13, PWM_OUTPUT);	
	pinMode(6, OUTPUT);
	
	pwmWrite(13, 1023);
	digitalWrite(6, LOW);

	while(true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}
