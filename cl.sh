#/bin/bash
g++ src/*.cpp -o tuto -Iinclude -L. -ltcod_debug -ltcodxx_debug -Wl,-rpath=. -Wall
