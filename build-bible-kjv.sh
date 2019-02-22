#!/bin/bash
set -e
# run this script with bash ./build-bible-kjv.sh
if ! [ -d deps ]; then
    mkdir deps
fi

pushd deps
if ! [ -d editline ]; then
    git clone https://github.com/umeboshi2/editline.git
fi
if ! [ -d emcurses ]; then
    git clone https://github.com/rhaberkorn/emcurses.git
fi
popd

# build editline first
pushd deps/editline

if ! [ -r configure ]; then
    ./autogen.sh
fi

if ! [ -r Makefile ]; then
    PREFIX=`pwd`/../libs
    emconfigure ./configure --disable-shared --enable-static --prefix=$PREFIX
fi

make
make install
popd

# build emcurses
pushd deps/emcurses

if ! [ -r emscripten/libpdcurses.so ]; then
    pushd emscripten
    emmake make
    popd
fi

popd

# build bible-kjv

# build things that need host tools
make bible.data bible.data.conc makeindex

# remove host objects already used but also
# needed in target
for objfile in util.o compresslib.o; do
    if [ -r $objfile ]; then
	rm $objfile
    fi
done
echo "---------------Ready to build bible ------------------"

emmake make bible

# we need an extension for emcc
cp bible bible.bc

# prepare files
mkdir -p usr/lib
cp bible.data bible.data.conc usr/lib

#_deps="deps/emcurses/emscripten/libpdcurses.so deps/libs/lib/libeditline.a"
_deps="deps/libs/lib/libeditline.a"

echo "Running emcc..."
emcc -v bible.bc ${_deps} --preload-file usr -o bible-kjv.html -s ASSERTIONS=1 --proxy-to-worker --emrun
emcc -v bible.bc ${_deps} --embed-file usr -o bible-cli.js -s ASSERTIONS=1 -s EXIT_RUNTIME=1
emcc -v bible.bc ${deps} --preload-file usr -o bible-wp.js -s ASSERTIONS=1 --proxy-to-worker



# the target should be at bible-kjv/bible-kjv.html

