#!/bin/bash
for k in {7..10}
do
	for j in {3..3}
	do
	mkdir "mojio_report/level$j/ConNum$k"
	for i in {1..1}
		do
			mkdir "mojio_report/level$j/ConNum$k/Run$i"			
			./waf --run "scratch/mesh --RngRun=$i --RANDOM_SEED=$i  --NUM_CONNS=$k --inf_prefix=/home/mesh/app/Org-allinone-3.19/ns-3.19/mojio_report/l3/ConNum$k/Run1" &> mojio_report/level$j/ConNum$k/Run$i/res.txt
			mv *.xml mojio_report/level$j/ConNum$k/Run$i
		done
	done
done
