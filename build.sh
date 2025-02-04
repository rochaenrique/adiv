SRC_DIR="./src"
VENDOR_DIR="./vendor"
FLAGS="-std=c++20 -g -ggdb -Wall -Wextra"

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

# Platform
PLATFORM="linux"
if [[ "$1" =~ -platform=[a-zA-Z][a-zA-Z]* ]]
then
	PLATFORM=$(echo $1 | cut -d'=' -f2)
	if [[ $PLAFORM != 'linux_amd64' || $PLATFORM != 'macos' ]]
	then
		echo Specified plaform is \"$PLATFORM\"
	else
		echo \"$PLAFORM\" is NOT avalid platform
		exit
	fi
else
	echo No platform specified, default is \"$PLATFORM\"
fi

# Souce files
SOURCES=`get_sources $SRC_DIR`
echo === Source Files === 
echo $SOURCES | tr ' ' '\n'
echo === ===

# Libs
RAY_DIR="$VENDOR_DIR/raylib-5.5_$PLATFORM"
LIBS="-L$RAY_DIR/lib -lraylib"
if [ $PLATFORM = 'macos' ]
then
	LIBS="-Wl,-rpath,$RAY_DIR/lib/ $LIBS"
fi

CMD="g++ $FLAGS $SOURCES -I$RAY_DIR/include/ -I$SRC_DIR  $LIBS -o adiv"

# Run comand
echo Building with: $CMD
eval $CMD
