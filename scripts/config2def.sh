#!/bin/bash

CONF=.config
TARGET=makefile.def

if [ "" != "$1" ] &&  [ -f $1 ]; then
	echo "Set CONF to $1"
	CONF=$1
fi

if [  ! -f "$CONF" ]; then
	echo "$CONF doesn't exist"
	exit 1
fi

grep -v '^#' $CONF | cut -d '=' -f 1 | sed 's/.*/DEFS += &/' > $TARGET
