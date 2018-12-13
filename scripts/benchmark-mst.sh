#!/bin/bash
set -e
ROOT=$(cd $(dirname "${BASH_SOURCE[0]}")/.. && pwd)

if [ $# -ne 1 ]; then
	echo "usage: $0 EXEC" >&2
	exit 1
fi

EXEC=$1
CSV="$ROOT"/benchmarks/$EXEC.csv
echo "\"vertices\", \"edge probability\", \"execution time [s]\"" > "$CSV"
for CONFIG in \
	"10 0.9" \
	"100 0.2" \
	"1000 0.05" \
	"10000 0.05" \
	"20000 0.05" \
	"50000 0.05" \
; do
	N=`echo $CONFIG | awk '{ print $1 }'`
	P=`echo $CONFIG | awk '{ print $2 }'`
	echo "G($N, $P)"
	echo -n "$N, $P, " >> "$CSV"
	"$ROOT"/build/mst-$EXEC $N $P | grep minimum_spanning_tree | awk '{ print $2 }' >> "$CSV"
done
