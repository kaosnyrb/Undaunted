#!/usr/bin/env bash

SCRIPT_DIR=$( dirname "${BASH_SOURCE[0]}" )

local utils
utils+=$(cat "$SCRIPT_DIR/utils-src/_notation.pas")
utils+=$'\n\n'
utils+=$(cat "$SCRIPT_DIR/utils-src/_globals.pas")
utils+=$'\n\n'

for file in $SCRIPT_DIR/utils-src/*.pas; do
	if [ -f $file ] && [ $file != $SCRIPT_DIR/utils-src/_notation.pas ] && [ $file != $SCRIPT_DIR/utils-src/_globals.pas ]; then
		utils+=$(cat "$file")
		utils+=$'\n\n'
	fi
done

utils+=$'\n'
utils+="end."

echo "$utils" | tee $SCRIPT_DIR/SkyrimUtils.pas