#ifndef SYNCHRONISATION_SERVER_H
#define SYNCHRONISATION_SERVER_H

#include <string>

class SynchronisationServer {
private:
	void run();
	std::string* get_ltsa_export();
};
#endif