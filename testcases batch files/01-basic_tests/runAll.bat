echo off
cls
echo "starting basic tests"

echo "testing calls, modifies procedure, uses procedure and pattern"
..\AutoTesterInUse.exe calls_mod_uses_patterns_source.txt calls_mod_uses_patterns.txt results\calls_mod_uses_patterns_results.xml

echo "testing next and next*"
..\AutoTesterInUse.exe nextAndnextT_source.txt next_testcases.txt results\next_results.xml
..\AutoTesterInUse.exe nextAndnextT_source.txt nextT_testcases.txt results\nextT_results.xml

echo "testing with"
..\AutoTesterInUse.exe with_source.txt with_testcases.txt results\with_results.xml

echo "testing ands"
..\AutoTesterInUse.exe final_sourcesource.txt and.txt results\and_results.xml

PAUSE