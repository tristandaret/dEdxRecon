set -e # Exit on error

cd build
cmake ..
make -j8
cd ..
time ./build/Analysis