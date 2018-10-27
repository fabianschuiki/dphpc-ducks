#!/bin/bash
set -e
ROOT=$(cd $(dirname "${BASH_SOURCE[0]}")/.. && pwd)
"$ROOT"/build/prim-example
dot -Tpdf example.gv -o example.pdf
dot -Tpdf example_mst.gv -o example_mst.pdf
