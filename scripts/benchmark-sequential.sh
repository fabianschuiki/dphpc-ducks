#!/bin/bash
set -e
ROOT=$(cd $(dirname "${BASH_SOURCE[0]}")/.. && pwd)
CSV="$ROOT"/benchmarks/sequential.csv
echo "\"vertices\", \"edges\", \"execution time [s]\"" > "$CSV"
for N in 10 100 1000 10000 20000 50000; do
	echo "$N vertices"
	P=0.05
	echo -n "$N, $P, " >> "$CSV"
	"$ROOT"/build/mst-sequential $N $P | grep minimum_spanning_tree | awk '{ print $2 }' >> "$CSV"
done
