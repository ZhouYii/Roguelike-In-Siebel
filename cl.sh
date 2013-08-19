#/bin/bash
g++ src/*.cpp -g -o tuto -Iinclude -L. -ltcod_debug -ltcodxx_debug -Wl,-rpath=. -Wall
