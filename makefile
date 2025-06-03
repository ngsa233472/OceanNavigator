all: ocean_daemon shipctl

ocean_daemon: ocean_daemon.cpp
	g++ -std=c++17 -o ocean_daemon ocean_daemon.cpp

shipctl: shipctl.cpp
	g++ -std=c++17 -o shipctl shipctl.cpp

clean:
	rm -f ocean_daemon shipctl

.PHONY: all clean