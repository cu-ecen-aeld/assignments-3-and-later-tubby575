#!/bin/sh

if [ $# -lt 2 ]
then
	echo "Invalid Number of Parameters given! Exiting..."
    exit 1
fi

FULL_FILE_PATH=$1
DIR_PATH=$(dirname "$FULL_FILE_PATH")
FILE_NAME=$(basename "$FULL_FILE_PATH")
WRITE_STR=$2

if [ ! -d $DIR_PATH ]
then
    mkdir -p $DIR_PATH
    if [ $? -ne 0 ]
    then
        echo "Failed to create directory! Exiting..."
        exit 1
    fi
fi

if [ -f $FULL_FILE_PATH ]
then
    rm $FULL_FILE_PATH
fi

touch $FULL_FILE_PATH
if [ $? -eq 0 ]
then
    echo "$WRITE_STR" >> $FULL_FILE_PATH
else
    echo "Failed to create file! Exiting..."
    exit 1
fi