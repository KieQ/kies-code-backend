git submodule update --init --recursive

if [ ! -d "build/" ]; then
  mkdir build
fi

cd build

conan install .. --build=missing

cmake -DCMAKE_BUILD_TYPE=Release -DSKIP_TEST=ON ..

make -j

make install
