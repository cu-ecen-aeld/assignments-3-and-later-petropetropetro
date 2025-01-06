#!/bin/bash
if [ $# -lt 2 ]; then
  echo "Error: Not enough arguments provided."
  echo "Usage: $0 <filesdir> <searchstr>"
  exit 1
fi
filesdir=$1
searchstr=$2
if [ ! -d "$filesdir" ]; then
  echo "Error: $filesdir is not a directory."
  exit 1
fi
num_files=$(find "$filesdir" -type f | wc -l)
num_matches=$(grep -r "$searchstr" "$filesdir" 2>/dev/null | wc -l)
echo "The number of files are $num_files and the number of matching lines are $num_matches"
