#!/bin/bash
for j in {1..4}
do
	for i in {5..6}
	do
		mkdir "mojio_report/level$j/Run$i"
		./waf --run "scratch/mesh --RANDOM_SEED=$i --level=$j" &> mojio_report/level$j/Run$i/res.txt
		mv *.xml mojio_report/level$j/Run$i
	done
done
