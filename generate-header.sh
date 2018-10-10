#!/usr/bin/env bash

command -v pcpp >/dev/null 2>&1 || {
    echo "Could not find pcpp. Install it with pip (pip install pcpp)."
    exit 1
}

pcpp -o dist/pacif.hpp \
    --passthru-defines \
    --passthru-unfound-includes \
    --passthru-unknown-exprs \
    --passthru-comments \
    pacif.hpp
