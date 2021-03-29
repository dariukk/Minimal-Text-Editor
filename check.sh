#!/bin/bash
TOTAL=0
WORKING_DIR=`pwd`
EXEC=editor
input="${WORKING_DIR}/tests/"
output="${WORKING_DIR}/out/"
points=0

make

if [ ! -f "$EXEC" ]; then
    echo "Binary \"editor\" not found"
    exit 1;
fi

function check_test() {
    in_path=$1
    out_path=$2
    ref_path=$3
    test_nr=$4

    rm "$WORKING_DIR/editor.in" 2> /dev/null
    rm "$WORKING_DIR/editor.out" 2> /dev/null
    cp "$in_path" "$WORKING_DIR/editor.in"
    > "$WORKING_DIR/editor.out"
    #exec="$EXEC"
    ./$EXEC
    mv "$WORKING_DIR/editor.out" "$out_path"
    echo -n "Test: $test_nr ....................... "

    diff $ref_path $out_path 2>&1 1>> $output/difr
    if test $? -eq 0; then
        echo "PASS"
        points=$(bc <<< "$points + 10")
    else
        echo "FAILED"
    fi
}

if [ ! -d "$input" ]; then
    echo "tests dir not found"
    exit 1
fi

if [ ! -d "$output" ]; then
    mkdir "$output"
fi
> $output/difr
NO_TESTS=10

echo -e "\n===========RUNNING TESTS============"
for i in $(seq 1 $NO_TESTS); do
    test_inp="$input/editor$i.in"
    test_ref="$input/editor$i.out"
    test_out="$output/editor$i.out"

    check_test $test_inp $test_out $test_ref $i
done
rm "$WORKING_DIR/editor.in" 2> /dev/null
echo -e "=========== points: $points / 100 ===========\n\n"
