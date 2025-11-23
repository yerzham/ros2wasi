#!/bin/bash

# Remove directories listed in unsupported.txt files
if [ -f unsupported.txt ]; then
    while read F; do
        if [ -n "$F" ]; then
            echo "Removing: src/$F"
            rm -rf "src/$F"
        fi
    done < unsupported.txt
fi

