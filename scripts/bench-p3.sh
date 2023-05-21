#!/bin/zsh

set -e

echo '1
1
0
0
1
1
-1
0
0
-1
' | ./a.out $1 0.05 200 > p3.txt

# remove prompt line
sed -i 1d p3.txt

cmd="set title \"$1\";"'
    set xlabel "x";
    set ylabel "y";
    set terminal pngcairo;
    set output "/tmp/output.png";
    plot    "p3.txt" using 2:3 with linespoints title "p1", 
            "p3.txt" using 4:5 with linespoints title "p1";
'

gnuplot -e "$cmd" && imgview /tmp/output.png
