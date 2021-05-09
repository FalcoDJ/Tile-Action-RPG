#!/bin/sh
cd bin/
g++ -o ActionRPG ../src/*.cpp ../src/Shapes/*.cpp ../src/Shapes/HitBox/*.cpp -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17
cd ..