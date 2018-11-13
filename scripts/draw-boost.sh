#!/bin/bash
set -e
ROOT=$(cd $(dirname "${BASH_SOURCE[0]}")/.. && pwd)
"$ROOT"/build/mst-boost 20 30
dot -Tpdf boost_mst.gv -o boost.pdf
