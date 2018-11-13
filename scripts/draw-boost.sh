#!/bin/bash
set -e
ROOT=$(cd $(dirname "${BASH_SOURCE[0]}")/.. && pwd)
"$ROOT"/build/mst-boost "$ROOT"/data/er_20_30.bin
dot -Tpdf boost_mst.gv -o boost.pdf
