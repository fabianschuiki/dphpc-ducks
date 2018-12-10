#!/usr/bin/env python3
# This script generates the C++ LFSR tap lookup table.
import sys
import argparse

# Parse the input arguments.
parser = argparse.ArgumentParser()
parser.add_argument("TAPSTXT")
args = parser.parse_args()

# Parse the input text file.
with open(args.TAPSTXT) as f:
	taps = list()
	for line in f:
		segments = line.split()
		if len(segments) != 2:
			continue
		fields = segments[1].split(",")
		taps.append((int(segments[0]), [int(x)-1 for x in fields]))

# Emit the C++ code.
cpp = ",\n".join([
	"    /* %2d */  %s" % (idx, " | ".join(["((uint64_t)1 << %2d)" % x for x in shifts]))
	for (idx, shifts) in taps
])
sys.stdout.write("const static uint64_t LFSR_TAPS[%d] = {\n%s\n};\n" % (len(taps), cpp))
