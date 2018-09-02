@echo OFF
cd C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\Build
call vcvarsall.bat x86
msbuild C:\Users\hyunb\Desktop\D3Dsimple\D3Dsimple\D3Dsimple.vcxproj
PAUSE
