if [ ! -d "build/" ]; then
  mkdir build
fi

cd build

conan install .. --build=missing

cmake -DCMAKE_BUILD_TYPE=Release -DSKIP_TEST=ON ..

num_cpus=`grep 'physical id' /proc/cpuinfo | sort -u | wc -l`

echo 'compiling starts with' ${num_cpus} 'cpus'

make -j ${num_cpus}

make install