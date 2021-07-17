#!/bin/sh

source /etc/profile.d/emscripten.sh

cd bin/


if  [[ $1 = "--debug" || $1 = "-d" ]]; then
    echo -n "Debug mode enabled!"
    echo ""

    em++ -std=c++17 -O2 -s ALLOW_MEMORY_GROWTH=1 -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 -s USE_LIBPNG=1 -DDEBUG_APPLICATION -lopenal ../src/*.cpp ../src/Shapes/*.cpp ../src/Shapes/HitBox/*.cpp -o index.html --preload-file ../assets/
else
    echo -n "Release Mode enabled!"
    echo ""
    em++ -std=c++17 -O2 -s ALLOW_MEMORY_GROWTH=1 -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 -s USE_LIBPNG=1  ../src/*.cpp ../src/Shapes/*.cpp ../src/Shapes/HitBox/*.cpp -o index.html --preload-file ../assets/
fi

rm -Rf test_web.sh

touch test_web.sh

echo "#!/bin/sh" >> test_web.sh
echo "source /etc/profile.d/emscripten.sh" >> test_web.sh
echo "emrun bin/index.html" < test_web.sh >> test_web.sh

chmod u+x test_web.sh

cd ..