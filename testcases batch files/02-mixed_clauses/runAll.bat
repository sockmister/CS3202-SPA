echo off
cls
echo "starting tests"
..\AutoTesterInUse.exe and_source.txt and_testcases.txt results\and_results.xml
..\AutoTesterInUse.exe BitOfEverything_source.txt BitOfEverything.txt results\BitOfEverything_results.xml


PAUSE