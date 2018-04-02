mkdir build
mkdir ouput_pcm
del build\*.* /Q /F
del output_pcm\*.* /Q /F
cd build

cmake .. -G "MinGW Makefiles"					&& ^
make -j4										&& ^
copy vad-demo.exe ..							&& ^
echo #-----------build succeeded----------#		&& ^
..\vad-demo.exe

cd ..