#/bin/bash
g++ src/*.cpp -o tuto -Iinclude -L. -ltcod -ltcodxx_debug -Wl,-rpath=. -Wall
