#!/usr/bin/env bash

set -e -u

function publish() {
    node-pre-gyp package testpackage
    node-pre-gyp publish
    node-pre-gyp info
    make clean
}

# test installing from source
echo "building binaries for publishing"
CFLAGS="${CFLAGS:-} -include $(pwd)/src/gcc-preinclude.h" CXXFLAGS="${CXXFLAGS:-} -include $(pwd)/src/gcc-preinclude.h" V=1 npm install --build-from-source
nm lib/binding/*/node_sqlite3.node | grep "GLIBCXX_" | c++filt  || true
nm lib/binding/*/node_sqlite3.node | grep "GLIBC_" | c++filt || true
npm test

publish