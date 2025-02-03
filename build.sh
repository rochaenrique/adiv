RAY_DIR="./raylib/raylib-5.5_linux_amd64"
SRC_DIR="src"
SOURCES="main.cpp Player.cpp"
FLAGS="-g -ggdb -Wall -Wextra"

g++ $FLAGS \
	-I$RAY_DIR/include/ -I$SRC_DIR \
	-o adiv  `for f in "$(echo $SRC_DIR/*.cpp)"; do echo $f; done`\
	-L$RAY_DIR/lib -l:libraylib.a




