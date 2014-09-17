#!/bin/sh -e

VERSION=0.1

MODULE=$1

TEMPDIR=$(mktemp -d)
WORKDIR=$(pwd)

# Default options. These can be overwritten
if [ "$(uname -m)" = "x86_64" ]; then
	ARCH=amd64
else
	ARCH=i386
fi
HOMEPAGE="https://github.com/henu/libagl"
SHORT_DESCRIPTION="Libagl"
LONG_DESCRIPTION="Libagl"

if [ "$MODULE" = "libagl-dev" ]; then
	mkdir -p $TEMPDIR/data/usr/include/libagl/
	cp include/*.hpp $TEMPDIR/data/usr/include/libagl/
	cp -r include/Math $TEMPDIR/data/usr/include/libagl/

	NAME="libagl-dev"
	MAINTAINER_NAME="Henrik Heino"
	MAINTAINER_EMAIL="henrik.heino@gmail.com"
	SECTION="libs"
	DEPS=""
	SHORT_DESCRIPTION="Libagl developement files"
	LONG_DESCRIPTION="Libagl delelopement files"
fi

if [ "$MODULE" = "libagl-zlib" ]; then
	cmake .
	make agl_zlib

	mkdir -p $TEMPDIR/data/usr/lib/
	cp src/Zlib/libagl_zlib.so $TEMPDIR/data/usr/lib/

	NAME="libagl-zlib"
	MAINTAINER_NAME="Henrik Heino"
	MAINTAINER_EMAIL="henrik.heino@gmail.com"
	SECTION="libs"
	DEPS="zlib1g"
	SHORT_DESCRIPTION="Libagl zlib wrapper"
	LONG_DESCRIPTION="Libagl zlib wrapper"
fi

if [ "$MODULE" = "libagl-zlib-dev" ]; then
	mkdir -p $TEMPDIR/data/usr/include/libagl/
	cp -r include/Zlib $TEMPDIR/data/usr/include/libagl/

	NAME="libagl-zlib-dev"
	MAINTAINER_NAME="Henrik Heino"
	MAINTAINER_EMAIL="henrik.heino@gmail.com"
	SECTION="libdevel"
	PC_FILE="agl-zlib"
	LIBS="-lagl_zlib -lz"
	DEPS="zlib1g-dev, libagl-dev, libagl-zlib"
	SHORT_DESCRIPTION="Libagl zlib wrapper developement files"
	LONG_DESCRIPTION="Libagl zlib wrapper delelopement files"
fi

# Ensure proper module was selected
if [ -z "$NAME" ]; then
	echo "Invalid module!"
	exit 1
fi

# Generate .pc file, if this lib requires it
if [ -n "$PC_FILE" ]; then
	mkdir -p $TEMPDIR/data/usr/lib/pkgconfig/
	echo "prefix=/usr" >> $TEMPDIR/data/usr/lib/pkgconfig/${PC_FILE}.pc
	echo "exec_prefix=\${prefix}" >> $TEMPDIR/data/usr/lib/pkgconfig/${PC_FILE}.pc
	echo "libdir=\${exec_prefix}/lib" >> $TEMPDIR/data/usr/lib/pkgconfig/${PC_FILE}.pc
	echo "includedir=\${prefix}/include/libagl" >> $TEMPDIR/data/usr/lib/pkgconfig/${PC_FILE}.pc
	echo "" >> $TEMPDIR/data/usr/lib/pkgconfig/${PC_FILE}.pc
	echo "Name: $PC_FILE" >> $TEMPDIR/data/usr/lib/pkgconfig/${PC_FILE}.pc
	echo "Description: $SHORT_DESCRIPTION" >> $TEMPDIR/data/usr/lib/pkgconfig/${PC_FILE}.pc
	echo "Version: $VERSION" >> $TEMPDIR/data/usr/lib/pkgconfig/${PC_FILE}.pc
	echo "Requires: " >> $TEMPDIR/data/usr/lib/pkgconfig/${PC_FILE}.pc
	echo "Libs: $LIBS" >> $TEMPDIR/data/usr/lib/pkgconfig/${PC_FILE}.pc
	echo "Cflags: $CFLAGS" >> $TEMPDIR/data/usr/lib/pkgconfig/${PC_FILE}.pc
fi

# Pack all files
cd $TEMPDIR/data/
tar czf ../data.tar.gz *
DATA_SIZE=$(du -s|cut -f 1)

# Make control files
mkdir -p $TEMPDIR/control
find *|xargs md5sum 2> /dev/null > ../control/md5sums || true
cd $TEMPDIR/control/
echo "Package: $NAME" >> control
echo "Version: $VERSION" >> control
echo "Architecture: $ARCH" >> control
echo "Maintainer: $MAINTAINER_NAME <$MAINTAINER_EMAIL>" >> control
echo "Installed-Size: $DATA_SIZE" >> control
echo "Depends: $DEPS" >> control
echo "Section: $SECTION" >> control
echo "Priority: optional" >> control
echo "Homepage: $HOMEPAGE" >> control
echo "Description: $SHORT_DESCRIPTION" >> control
echo " $LONG_DESCRIPTION" >> control
tar czf ../control.tar.gz *

# Create other package files
echo "2.0" > $TEMPDIR/debian-binary

# Make package
cd $TEMPDIR
DEB_FILE="${NAME}_${ARCH}.deb"
ar r $WORKDIR/$DEB_FILE debian-binary control.tar.gz data.tar.gz 2> /dev/null

rm -rf $TEMPDIR

