IF NOT EXIST "build" mkdir build
cd build
cmake -A x64 ..
cmake --build . --config Release
cd ..

IF NOT EXIST "build32" mkdir build32
cd build32
cmake -A Win32 ..
cmake --build . --config Release
cd ..