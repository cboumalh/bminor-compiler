#!/bin/sh

# encoder
for testfile in ./test/encode/good*.bminor
do
	if ./bminor --encode $testfile > $testfile.out
	then
		echo "$testfile success (as expected)"
	else
		echo "$testfile failure (INCORRECT)"
	fi
done

for testfile in ./test/encode/bad*.bminor
do
	if ./bminor --encode $testfile > $testfile.out
	then
		echo "$testfile success (INCORRECT)"
	else
		echo "$testfile failure (as expected)"
	fi
done

# scanner
for testfile in ./test/scanner/good*.bminor
do
	if ./bminor --scan $testfile > $testfile.out
	then
		echo "$testfile success (as expected)"
	else
		echo "$testfile failure (INCORRECT)"
	fi
done

for testfile in ./test/scanner/bad*.bminor
do
	if ./bminor --scan $testfile > $testfile.out
	then
		echo "$testfile success (INCORRECT)"
	else
		echo "$testfile failure (as expected)"
	fi
done

# parser
for testfile in ./test/parser/good*.bminor
do
	if ./bminor --parse $testfile > $testfile.out
	then
		echo "$testfile success (as expected)"
	else
		echo "$testfile failure (INCORRECT)"
	fi
done

for testfile in ./test/parser/bad*.bminor
do
	if ./bminor --parse $testfile > $testfile.out
	then
		echo "$testfile success (INCORRECT)"
	else
		echo "$testfile failure (as expected)"
	fi
done

