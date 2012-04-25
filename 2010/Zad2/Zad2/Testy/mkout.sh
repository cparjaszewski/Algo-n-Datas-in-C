#!/bin/sh

for I in *in
do
	./zad < $I > `echo $I | sed -e 's/.in/.out/'`
done
