#!/bin/bash
set -e
ROOT=$(cd $(dirname "${BASH_SOURCE[0]}")/.. && pwd)
"$ROOT"/build/mst-sequential 20 30
dot -Tpdf sequential_mst.gv -o sequential.pdf
