RAY_PATH="./raylib/raylib-5.5_linux_amd64"
SOURCES="main.cpp"
FLAGS="-g -ggdb -Wall -Wextra"

g++ $FLAGS \
	-I"$RAY_PATH/include/" \
	-o adiv $SOURCES \
	-L"$RAY_PATH/lib" -l:libraylib.a 
