ADIV_DIR=~/dev/adiv
SRC_DIR=./src
VENDOR_DIR=./vendor
FLAGS="-std=c++23 -g -ggdb -Wall -Wextra -Wno-missing-field-initializers"

get_sources () {
	for f in $(echo $1/*)
	do
		if [ -d $f ]
		then
			get_sources $f
		elif [[ $(basename $f) =~ ^[a-zA-Z_].*\.cpp$ ]]
		then
			echo $f | cut -c3-
		fi
	done
}

echo Building from $PWD

# Platform
PLATFORM="linux_amd64"
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
echo ==== Source Files === 
echo $SOURCES | tr ' ' '\n'
echo === ===

# Libs
RAY_DIR="$VENDOR_DIR/raylib-5.5_$PLATFORM"
LIB_DIRS="-L$RAY_DIR/lib"
LIBS="-l:libraylib.a"
if [ $PLATFORM = 'macos' ]
then
	LIB_DIRS="-Wl,-rpath,$RAY_DIR/lib/ $LIB_DIRS"
	LIBS="-lraylib"
fi

# Run comand

CMD="g++ $FLAGS $SOURCES -I$RAY_DIR/include/ -I$SRC_DIR $LIB_DIRS $LIBS -o adiv"
echo Compile command:
echo $CMD
eval $CMD


