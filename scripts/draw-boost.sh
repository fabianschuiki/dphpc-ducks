#!/bin/bash
set -e
ROOT=$(cd $(dirname "${BASH_SOURCE[0]}")/.. && pwd)
"$ROOT"/build/mst-boost 8 0.2
dot -Tpdf boost_mst.gv -o boost.pdf
