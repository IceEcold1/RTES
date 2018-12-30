#ifndef SYNCHRONISATION_SERVER_H
#define SYNCHRONISATION_SERVER_H

#include <string>
#include <iostream>
#include <dirent.h>
#include <stdio.h>
#include <fstream>
#include <vector>

using namespace std;

class SynchronisationServer {
private:
	std::vector<std::string> test;

public:
	std::vector<std::string> read_ltsa_exports();
	void run();
};
#endif