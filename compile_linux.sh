#!/bin/sh
cd bin/
g++ -o ActionRPG ../src/*.cpp ../src/Shapes/*.cpp -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17
cd ..