#include<iostream>
#include<wiringPi.h>
#include<thread>
#include<chrono>
extern "C" {
	#include<lua.h>
	#include<lauxlib.h>
	#include<lualib.h>
}

typedef int (*lua_CFunction) (lua_State *L);

int sleep(lua_State* L) {
	int milis = lua_tonumber(L, 1);

	std::this_thread::sleep_for(std::chrono::milliseconds(milis));

	return 0;
}

int setPinMode(lua_State* L) {	
	
	return 0;
}


int main() {
  std::cout << "Hello, C++!" << std::endl;
 	
	wiringPiSetupGpio();

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	lua_pushcfunction(L, sleep);
	lua_setglobal(L, "sleep");

	luaL_dostring(L, "print('Hello, Lua!')");

	lua_close(L);

	wiringPiSetupGpio();

	pinMode(27, OUTPUT);

	pullUpDnControl(27, PUD_UP);
	digitalWrite(27, HIGH);

	bool is_on = true;

	while(true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		is_on = !is_on;
	  // std::cout << "is on: " << (is_on? "ON" : "OFF") << std::endl;
		digitalWrite(27, is_on ? HIGH : LOW);
	}

	return 0;
}
