#!/bin/bash

line_count=4

pass=1
first=1

declare -A duplicate_set

for file in $@
do
  if ( [ -e $file ] )
  then
    if ( [ ${duplicate_set[${file}]} ] )
    then
      continue
    fi

    duplicate_set[$file]=1

    diff -q <(head -n ${line_count} $file) <(head -n ${line_count} ../LICENCE_HEADER) &>/dev/null

    if ( [ $? -ne 0 ] )
    then
      if ( [ ${first} -eq 1 ] )
      then
        echo
        echo "These files do not have the correct license header"
        echo "Please prepend LICENCE_HEADER to them"
        echo
        first=0
      fi
      echo ${file}
      pass=0
    fi
  fi
done

if ( [ ${first} -ne 1 ] )
then
  echo
  exit 1
fi
