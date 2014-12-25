#!/bin/bash
# You need run to link to correct name
#'ln -s synconfig.sh ~/bin/push_config'
#'ln -s synconfig.sh ~/bin/pull_config'

BASE_NAME=$0

# You need change to the Dest machine ip or local alias.
DEST_MACHINE="T440.local"

# The directory which we need to sync, don't add '/' at the end.
SYN_DIR="/work/private/irc"

if [ "$BASE_NAME" = "push_config" ]; then
	echo "rsync -avz $SYN_DIR $DEST_MACHINE:${SYN_DIR%/*}"
	rsync -avz $SYN_DIR $DEST_MACHINE:${SYN_DIR%/*}
fi
if [ "$BASE_NAME" = "pull_config" ]; then
	echo "rsync -avz $DEST_MACHINE:$SYN_DIR ${SYN_DIR%/*}"
	rsync -avz $DEST_MACHINE:$SYN_DIR ${SYN_DIR%/*}
fi
