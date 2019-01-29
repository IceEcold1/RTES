###############################################################
#
# Purpose: Makefile for "demo"
# Author.: robotis
# Version: 0.1
# License: GPL
#
###############################################################

TARGET = main

INCLUDE_DIRS = -L /usr/realtime/lib -I /usr/realtime/include

CXX = g++
CXXFLAGS += -std=c++0x -O2 -DLINUX -Wall $(INCLUDE_DIRS)
LFLAGS += -lpthread -ljpeg -lrt -lboost_system -lboost_thread

all: *.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) -I . *.cpp posix-regex-cpp/POSIXRegex.cpp $(LFLAGS)
	chmod 755 $(TARGET)

clean:
	rm -f *.a * $(TARGET) core *~ *.so *.lo

libclean:
	make -C clean

distclean: clean libclean

# useful to make a backup "make tgz"
tgz: clean
	mkdir -p backups
	tar czvf ./backups/DARwIn_demo_`date +"%Y_%m_%d_%H.%M.%S"`.tgz --exclude backups *
