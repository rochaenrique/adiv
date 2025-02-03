RAY_DIR="./raylib/raylib-5.5_linux_amd64"
SRC_DIR="src"
FLAGS="-g -ggdb -Wall -Wextra"

get_sources () {
	for f in $(echo $1/*)
	do
		if [ -d $f ]
		then
			get_sources $f
		elif [[ $(basename $f) =~ ^[a-zA-Z_].*\.cpp$ ]]
		then
			echo $f
		fi
	done
}

SOURCES=`get_sources $SRC_DIR`
echo === Source Files === 
echo $SOURCES | tr ' ' '\n'
echo === ===

g++ $FLAGS \
	-I$RAY_DIR/include/ -I$SRC_DIR \
	-o adiv $SOURCES \
	-L$RAY_DIR/lib -l:libraylib.a




