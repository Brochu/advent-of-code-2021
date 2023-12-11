cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DDAY=%1

ninja -C build
