#ifndef SYNCHRONISATION_SERVER_H
#define SYNCHRONISATION_SERVER_H

#include <string>
#include <iostream>
#include <dirent.h>
#include <stdio.h>
#include <fstream>

using namespace std;

class SynchronisationServer {
private:

public:
	std::string* read_ltsa_exports();
	void run();
};
#endif