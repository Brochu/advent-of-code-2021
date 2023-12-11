cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DDAY=%1

ninja -C build
