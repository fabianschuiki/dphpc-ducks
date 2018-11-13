#!/bin/bash
set -e
ROOT=$(cd $(dirname "${BASH_SOURCE[0]}")/.. && pwd)
CSV="$ROOT"/benchmarks/boost.csv
echo "\"vertices\", \"edges\", \"execution time [s]\"" > "$CSV"
for V in 10 100 1000 10000 100000 1000000; do
	echo "$V vertices"
	E=$((V*2))
	echo -n "$V, $E, " >> "$CSV"
	"$ROOT"/build/mst-boost "$ROOT"/data/er_${V}_${E}.bin | grep minimum_spanning_tree | awk '{ print $2 }' >> "$CSV"
done
