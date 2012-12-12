all: version1 version2

# segfaults for some reason
version1:
	g++ -g -Wall   version1.cpp   -o version1

# works + has servo commands
# TODO: add ifdef rules to allow compiling for MBED or other (PC) without changing the code
version2:
	g++ -g -Wall   version2.cpp   -o version2

clean:
	rm -v version1 version2
