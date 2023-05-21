#!/bin/zsh

set -e

function run() {
    for i in {1..100}; {
        $1 $i | grep result | cut -f 2- -d ' '
    }
}

run ./a.out > p1.txt
gnuplot -e '
    set xlabel "n";
    set ylabel "x(1)";
    set terminal pngcairo;
    set output "/tmp/output.png";
    plot "p1.txt" with linespoints title "x(1)", 
          cos(1);
' && imgview /tmp/output.png

