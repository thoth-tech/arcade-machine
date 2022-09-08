#!/bin/bash
rm stats/lines-of-code.txt
touch stats/lines-of-code.txt
CPPLINES=$(find . -name '*.cpp' | xargs wc -l | tail -1 | sed 's/[^0-9]*//g')
echo "C++ (code): $CPPLINES" >> stats/lines-of-code.txt
HLINES=$(find . -name '*.h' | xargs wc -l | tail -1 | sed 's/[^0-9]*//g')
echo -n "C++ (header): $HLINES" >> stats/lines-of-code.txt

rm stats/git.txt
touch stats/git.txt
COMMITCOUNT=$(git rev-list --all --count | sed 's/[^0-9]*//g')
echo "Commits: $COMMITCOUNT" >> stats/git.txt
BRANCHCOUNT=$(git branch -r | wc -l | sed 's/[^0-9]*//g')
echo "Branches: $BRANCHCOUNT" >> stats/git.txt
CONTRIBUTORCOUNT=$(git shortlog -snc | wc -l | sed 's/[^0-9]*//g')
echo -n "Contributors: $CONTRIBUTORCOUNT" >> stats/git.txt

rm stats/contributors.txt
touch stats/contributors.txt
git shortlog -snc | sed 's/^ *//;s/ *$//' > stats/contributors.txt