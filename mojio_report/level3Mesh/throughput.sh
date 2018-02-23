#!/bin/bash
for K in {0..8}
do

	for j in {3..3}
	do

		for i in {1..1}
		do
			cat ./level$j/ConNum$K/Run$i/res.txt |grep "Mean jitter" |cut -d "=" -f 2 |cut -d " " -f 2 
		done

	done
done
