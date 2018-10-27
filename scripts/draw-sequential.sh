#!/bin/bash
set -e
ROOT=$(cd $(dirname "${BASH_SOURCE[0]}")/.. && pwd)
"$ROOT"/build/mst-sequential 20 30 > sequential.gv
dot -Tpdf sequential.gv -o sequential.pdf
