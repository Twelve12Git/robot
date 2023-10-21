call ${IDF_PATH}/export.bat
idf.py menuconfig
mkdir build
cd build
cmake .. -G Ninja
ninja