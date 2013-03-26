echo off
cls
echo "starting tests"

::echo "test uses statement"

echo "test uses procedure"
..\AutoTesterInUse.exe source.txt test_suchThatCl\uses_procedure.txt results\uses_procedure_results.xml

::echo "test modifies statement"

echo "test modifies procedure"
..\AutoTesterInUse.exe source.txt test_suchThatCl\modifies_procedure.txt results\modifies_procedure_results.xml

::echo "test calls"

::echo "test calls*"

::echo "test follows"

::echo "test follows*"

::echo "test parent"

::echo "test parent*"

echo "test next"
..\AutoTesterInUse.exe source.txt test_suchThatCl\next.txt results\next_results.xml

echo "test next*"
..\AutoTesterInUse.exe source.txt test_suchThatCl\nextT.txt results\nextT_results.xml

::echo "test affects"

::echo "test affects*"

echo "test withCl"
..\AutoTesterInUse.exe source.txt test_withCl\withCl_all.txt results\withCl_results.xml

echo "test pattern assign"
..\AutoTesterInUse.exe source.txt test_patternCl\patterns_assign.txt results\patterns_assign_results.xml

echo "test patterns if"
..\AutoTesterInUse.exe source.txt test_patternCl\patterns_if.txt results\patterns_if_results.xml

echo "test patterns while"
..\AutoTesterInUse.exe source.txt test_patternCl\patterns_while.txt results\patterns_while_results.xml

echo "test patterns all"
..\AutoTesterInUse.exe source.txt test_patternCl\patterns_all.txt results\patterns_all_results.xml

::echo "test such that assign"
::echo "test such that bool"
::echo "test such that call"
::echo "test such that constant"
::echo "test such that procedure"
::echo "test such that prog_line"
::echo "test such that stmt"
::echo "test such that variable"
::echo "test such that while"
::echo "test such that tuple"

echo "test mixture"
..\AutoTesterInUse.exe source.txt test_general\query_order.txt results\query_order_results.xml

PAUSE