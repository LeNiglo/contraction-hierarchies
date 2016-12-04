#!/bin/bash

function die {
  echo "ERROR: $@"
  exit 1
}

[[ -f "data.csv" ]] || die "File not found: 'data.csv'. Please move it to the current working directory."

OUT=$(echo "-12.9857,45.194->-12.6864,45.1035" | $1 data.csv)

(echo "$OUT" | awk '
  BEGIN {
    e[1]=36315; n[1]="Num nodes"
    e[2]=308646; n[2]="Num arcs"
    e[3]=612965.047218; n[3]="Sum of arc durations (seconds)";
    e[4]=3206.549245; n[4]="Answer to the query: -12.9857,45.194->-12.6864,45.1035";
    error = 0
  }
  {
    num++;
    d=$1-e[FNR];
    printf("Testing %s ... ", n[FNR]);
    if (d < -0.01 || d > 0.01) {
      print "ERROR: expected " e[FNR], " got: " $1;
      error++;
    } else print "OK!";
  }
  END {
    if (num != 4) {
      error++;
      print "ERROR: Missing or unexpected output (expected 4 lines, got " num ").";
    }
    if (error > 0) exit 1;
  }') && echo "PASS" || echo "FAILED"
