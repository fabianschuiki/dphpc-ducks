#!/bin/bash
set -e
ROOT=$(cd $(dirname "${BASH_SOURCE[0]}")/.. && pwd)
"$ROOT"/build/mst-sequential "$ROOT"/data/er_20_30.bin
dot -Tpdf sequential_mst.gv -o sequential.pdf
