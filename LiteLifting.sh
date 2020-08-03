#!/bin/bash
#Calvin Mak
#4/30/2018
#Total Byte Count
myTBCount=$(cat $1 | wc -c)
#Byte coutn without lines
myNCount=$(cat $1 | tr -d '\n' | wc -c)
lnCount=$(cat $1 | grep -cx '')
anChar=$(cat $1 | tr -d '\n' | tr -cd [[:alnum:]] | wc -c)
word=$(cat $1 | sed -n '3p' | awk '{print $4}')
if $(cat $1 | sed ':a;N;$!ba;s/\n/ /g' | grep -q "Capital Idea");
then
echo $(cat $1 | sed 's/./\L&/g') > LiteLifting.lc
fi

echo This file has $myTBCount characters in it.
echo This file has $myNCount characters in it.
echo This file has $lnCount empty lines.
echo This file contains $anChar alphanumeric characters.
echo "The 4th word of the 3rd line of text is /$word/"
