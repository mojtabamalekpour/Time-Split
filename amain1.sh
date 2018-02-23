#!/bin/bash
for k in {1..18}
do
	for j in {0..0}
	do
mkdir "mojio_report/level$j/ConNum$k"
	for i in {3..4}
		do
			
			mkdir "mojio_report/level$j/ConNum$k/Run$i"
			
			./waf --run "scratch/meshCN --RANDOM_SEED=$i --level=$j --NUM_CONNS=$k"&> mojio_report/level$j/ConNum$k/Run$i/res.txt
			mv *.xml mojio_report/level$j/ConNum$k/Run$i
		done
	done
done

