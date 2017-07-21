#!/bin/sh
# COMUT exits on error in which user provides negative line/col number for option rs/re

test_negative()
{
	if test $2 = "line" ; then
		LOCATION="-1 1"
	else
		LOCATION="1 -1"
	fi

	mkdir $OUTPUT_FOLDER_NAME
    
    # Run the tool with the input source and negative line/col
    echo "../../tool input-src/${TEST_INPUT} -o $OUTPUT_FOLDER_NAME $1 $LOCATION" 
    ../../tool input-src/${TEST_INPUT} -o $OUTPUT_FOLDER_NAME $1 $LOCATION > /dev/null 2>&1

    # The test success if exit value is NOT 0
    # and no files are generated in output folder
    if test $? != 0 && test `find ${OUTPUT_FOLDER_NAME} -type f -name \* | wc -l` = 0
    then
        echo "[SUCCESS] ${TEST_INPUT} $1 $2 is negative"
    else
        echo "[FAIL] $TEST_INPUT $1 $2 is negative"
    fi
    
    # Remove created output folder for this input source file
    rm -R $OUTPUT_FOLDER_NAME
}


# DIR: the directory that this script is in
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

echo "Executing $0"
cd input-src

# Exit script if there is no input source file
if test `find . -type f -name \* | wc -l` = 0 ; then
    exit
fi

for TEST_INPUT in *
do
	cd $DIR

    # Make folder name in output directory with input source name without .c
    OUTPUT_FOLDER_NAME=`echo "output/$TEST_INPUT" | sed 's/.\{2\}$//'`

    test_negative "-rs" "line"
    test_negative "-rs" "col"
    test_negative "-re" "line"
    test_negative "-re" "col"

    cd input-src
done