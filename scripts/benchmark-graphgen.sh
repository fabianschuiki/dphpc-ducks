#!/bin/bash
set -e
ROOT=$(cd $(dirname "${BASH_SOURCE[0]}")/.. && pwd)
CSV="$ROOT"/benchmarks/graphgen.csv
echo "\"vertices\", \"edges\", \"execution time [s]\"" > "$CSV"
for V in 10 100 1000 10000 20000; do
	echo "$V vertices"
	E=0.05
	echo -n "$V, $E, " >> "$CSV"
	# "$ROOT"/build/graphgen ${V} ${E} /dev/zero
	"$ROOT"/build/graphgen ${V} ${E} /dev/zero | grep setup | awk '{ print $2 }' >> "$CSV"
done
