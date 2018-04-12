#!bin/bash

TEST_VAL=1
echo "test 1: IF path"
if grep "TEST_VAL" if-test.sh
then
  echo "predicate is true"
else
  echo "predicate is false"
fi

echo "test 2: ELSE path" 
if grep $PWD if-test.sh
then
  echo "predicate is true"
else
  echo "predicate is false"
fi
