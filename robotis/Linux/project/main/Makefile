###############################################################
#
# Purpose: Makefile for "demo"
# Author.: robotis
# Version: 0.1
# License: GPL
#
###############################################################

TARGET = main

INCLUDE_DIRS = -I../../include -I../../../Framework/include -L /usr/realtime/lib -I /usr/realtime/include

CXX = g++
CXXFLAGS += -O2 -DLINUX -Wall $(INCLUDE_DIRS)
LFLAGS += -lpthread -ljpeg -lrt -lboost_system -lboost_thread

all: darwin.a *.cpp *.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) -I . *.cpp *.o posix-regex-cpp/POSIXRegex.cpp ../../lib/darwin.a $(LFLAGS)
	chmod 755 $(TARGET)

clean:
	rm -f *.a *.o $(TARGET) core *~ *.so *.lo

libclean:
	make -C ../../build clean

distclean: clean libclean

darwin.a:
	make -C ../../build

# useful to make a backup "make tgz"
tgz: clean
	mkdir -p backups
	tar czvf ./backups/DARwIn_demo_`date +"%Y_%m_%d_%H.%M.%S"`.tgz --exclude backups *
