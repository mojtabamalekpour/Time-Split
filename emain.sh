#!/bin/bash
for j in {0..4}
do
	for i in {1..2}
	do
		mkdir "mojio_report/level$j/Run$i"
		./waf --run "scratch/meshEnergy --RANDOM_SEED=$i --level=$j" &> mojio_report/level$j/Run$i/res.txt
		mv *.xml mojio_report/level$j/Run$i
	done
done
