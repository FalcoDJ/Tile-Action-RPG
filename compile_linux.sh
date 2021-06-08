#!/bin/sh
cd bin/

if  [[ $1 = "--debug" || $1 = "-d" ]]; then
    echo -n "Debug mode enabled!"
    echo ""

    g++ -o ActionRPG ../src/*.cpp ../src/Shapes/*.cpp ../src/Shapes/HitBox/*.cpp -DDEBUG_APPLICATION -lX11 -lGL -lopenal -lpthread -lpng -lstdc++fs -std=c++17
else
    echo -n "Release Mode enabled!"
    echo ""
    g++ -o ActionRPG ../src/*.cpp ../src/Shapes/*.cpp ../src/Shapes/HitBox/*.cpp -lX11 -lGL -lasound -lpthread -lpng -lstdc++fs -std=c++17
fi

cd ..