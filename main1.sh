#!/bin/bash
for j in {1..4}
do
	for i in {3..4}
	do
		mkdir "mojio_report/level$j/Run$i"
		./waf --run "scratch/mesh --RANDOM_SEED=$i --level=$j" &> mojio_report/level$j/Run$i/res.txt
	done
done
