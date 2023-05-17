#!/bin/sh

if [ $# -lt 2 ]
then
	echo "Invalid Number of Parameters given! Exiting..."
    exit 1
fi

if [ ! -d $1 ]
then
    echo "Directory $1 does not exist in filesystem! Exiting..."
    exit 1
fi

DIR_PATH=$1
SEARCH_STR=$2

NumberOfFiles=$( find "$DIR_PATH" -type f | wc -l )

NumberOfMatchingLines=0
for file in $(find "$DIR_PATH" -type f)
do
    matches=$(grep "$SEARCH_STR" $file -o | wc -l)
    NumberOfMatchingLines=$(($NumberOfMatchingLines + $matches))
done

echo "The number of files are $NumberOfFiles and the number of matching lines are $NumberOfMatchingLines"