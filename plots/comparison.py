#!/usr/bin/env python3
import sys
import os
import matplotlib.pyplot as plt
import numpy as np
import csv

PLOTDIR = os.path.dirname(os.path.abspath(sys.argv[0]))
csv.register_dialect("bench", skipinitialspace=True, quotechar='"')

def read_data(filename):
	k = list()
	v = list()
	with open(filename) as f:
		rd = csv.DictReader(f, dialect="bench")
		for row in rd:
			k.append("G(%s,%s)" % (row["vertices"], row["edge probability"]))
			v.append(float(row["execution time [s]"]))
	return (k,v)

boost_key, boost_val = read_data(PLOTDIR+"/../benchmarks/boost.csv")
sequential_key, sequential_val = read_data(PLOTDIR+"/../benchmarks/sequential.csv")

indices = np.arange(len(boost_key))
bar_width = 0.35

fig, ax = plt.subplots(figsize=(9,5))
plt.bar(indices, boost_val, bar_width)
plt.bar(indices + bar_width, sequential_val, bar_width)

ax.set_xlabel("Erdös-Renyi Graph")
ax.set_ylabel("Execution Time [s]")
ax.set_title("Minimum Spanning Tree Implementations")
ax.set_xticks(indices + bar_width / 2)
ax.set_xticklabels(boost_key, rotation=0)
ax.legend()

fig.tight_layout()

plt.savefig(PLOTDIR+"/comparison.pdf")
