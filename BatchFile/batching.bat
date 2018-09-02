call "%vs140comntools%\VsDevCmd.bat"
echo call complete
pause
cd C:\tfs.sbdinc.com
tf get $/MAC_MBA/CoreApplicationAndReports/Main/Application/Solution /recursive
echo get complete
pause
cd C:\tfs\CoreApplicationAndReports\Main\Application\Solution
msbuild
echo build complete
pause
devenv mba.sln
echo ide launch complete
pause