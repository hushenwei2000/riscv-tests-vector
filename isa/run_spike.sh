#!/bin/bash
rm rv64uv-v-v* rv64uv-p-v*
make
FILES=`find ./ -name "rv64uv-[p|v]*" | grep -v "dump"`
SUCC_COUNT=0
FAIL_COUNT=0
for item in $FILES
do
  echo "Testing $item..."
  spike -l --isa rv64gcv $item 2> temp
  if [ `grep -c "FAIL" temp` -ne '0' ]; then
    echo "$item FAIL!"
    FAIL_COUNT=`expr $FAIL_COUNT + 1`
  else
    SUCC_COUNT=`expr $SUCC_COUNT + 1`
  fi
done
echo "Success $SUCC_COUNT , Fail $FAIL_COUNT"
