set -e

dir="$1\/day$(printf '%02d' $2)"
path="$dir\/part$3.c"

echo $path

cat CMakeLists-Copy.txt | sed "s/\$\$PATH/$path/g" > CMakeLists.txt

mkdir -p build
cd build
cmake ..
make

WD="$(pwd)"

clear
cd "../$1/day$(printf '%02d' $2)/"

if [ "$4" = "-d" ]; then
    valgrind $WD/AOC
else
    $WD/AOC
fi
