#!/bin/zsh

set -e

./a.out 0.01 1000 > p3.txt

gnuplot -e '
    set xlabel "x";
    set ylabel "y";
    set terminal pngcairo;
    set output "/tmp/output.png";
    plot    "p3.txt" using 2:3 with linespoints title "p1", 
            "p3.txt" using 4:5 with linespoints title "p1";
' && imgview /tmp/output.png
