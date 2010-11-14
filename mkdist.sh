#!/bin/sh
PACKAGE="$1"
shift

rm -rf $PACKAGE &&
mkdir $PACKAGE &&
cp "$@" $PACKAGE &&
mv $PACKAGE/Makefile $PACKAGE/Makefile.dist &&
sed '/^#NONDIST/,/^#ENDNONDIST/d' \
  <$PACKAGE/Makefile.dist >$PACKAGE/Makefile &&
tar czf $PACKAGE.tgz $PACKAGE/* &&
rm -rf $PACKAGE ||
exit 1
