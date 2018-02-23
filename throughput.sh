#!/bin/bash
for i in {1..30}
do
	cat mojio_report/Run$i/res.txt |grep "Avg throughput" |cut -d "=" -f 2 |cut -d " " -f 2 >>  mojio_report/AllThru.txt
done
awk '{sum+=$1; sumsq+=$1*$1}END{print sum/NR , sqrt(sumsq/NR - (sum/NR)**2)}' mojio_report/AllThru.txt >> mojio_report/Thro.txt

for i in {1..30}
do
	cat mojio_report/Run$i/res.txt |grep "Traffic volume" |cut -d "=" -f 2 |cut -d " " -f 2 >>  mojio_report/Allvol.txt
done
awk '{sum+=$1; sumsq+=$1*$1}END{print sum/NR , sqrt(sumsq/NR - (sum/NR)**2)}' mojio_report/Allvol.txt >> mojio_report/Thro.txt


for i in {1..30}
do
	cat mojio_report/Run$i/res.txt |grep "Mean delay" |cut -d "=" -f 2 |cut -d " " -f 2 >>  mojio_report/AllDelay.txt
done
awk '{sum+=$1; sumsq+=$1*$1}END{print sum/NR , sqrt(sumsq/NR - (sum/NR)**2)}' mojio_report/AllDelay.txt  >> mojio_report/Thro.txt


for i in {1..30}
do
	cat mojio_report/Run$i/res.txt |grep "PDR" |cut -d "=" -f 2 |cut -d " " -f 2 >>  mojio_report/AllPDR.txt
done
awk '{sum+=$1; sumsq+=$1*$1}END{print sum/NR , sqrt(sumsq/NR - (sum/NR)**2)}' mojio_report/AllPDR.txt  >> mojio_report/Thro.txt


for i in {1..30}
do
	cat mojio_report/Run$i/res.txt |grep "Mean jitter" |cut -d "=" -f 2 |cut -d " " -f 2 >>  mojio_report/Alljitter.txt
done
awk '{sum+=$1; sumsq+=$1*$1}END{print sum/NR , sqrt(sumsq/NR - (sum/NR)**2)}' mojio_report/Alljitter.txt >> mojio_report/Thro.txt



for i in {1..30}
do
	cat mojio_report/Run$i/res.txt |grep "mean transmit Byterate" |cut -d "=" -f 2 |cut -d " " -f 2 >>  mojio_report/AllTbyte.txt
done
awk '{sum+=$1; sumsq+=$1*$1}END{print sum/NR , sqrt(sumsq/NR - (sum/NR)**2)}' mojio_report/AllTbyte.txt >> mojio_report/Thro.txt



for i in {1..30}
do
	cat mojio_report/Run$i/res.txt |grep "mean received Byterate" |cut -d "=" -f 2 |cut -d " " -f 2 >>  mojio_report/AllRbyte.txt
done
awk '{sum+=$1; sumsq+=$1*$1}END{print sum/NR , sqrt(sumsq/NR - (sum/NR)**2)}' mojio_report/AllRbyte.txt >> mojio_report/Thro.txt



for i in {1..30}
do
	cat mojio_report/Run$i/res.txt |grep "mean hop count" |cut -d "=" -f 2 |cut -d " " -f 2 >>  mojio_report/AllHop.txt
done
awk '{sum+=$1; sumsq+=$1*$1}END{print sum/NR , sqrt(sumsq/NR - (sum/NR)**2)}' mojio_report/AllHop.txt >> mojio_report/Thro.txt



for i in {1..30}
do
	cat mojio_report/Run$i/res.txt |grep "packet loss ratio" |cut -d "=" -f 2 |cut -d " " -f 2 >>  mojio_report/Allloss.txt
done
awk '{sum+=$1; sumsq+=$1*$1}END{print sum/NR , sqrt(sumsq/NR - (sum/NR)**2)}' mojio_report/Allloss.txt >> mojio_report/Thro.txt



for i in {1..30}
do
	cat mojio_report/Run$i/res.txt |grep "packet loss" |cut -d "=" -f 2 |cut -d " " -f 2 >>  mojio_report/Allpacketloss.txt
done
awk '{sum+=$1; sumsq+=$1*$1}END{print sum/NR , sqrt(sumsq/NR - (sum/NR)**2)}' mojio_report/Allpacketloss.txt >> mojio_report/Thro.txt



for i in {1..30}
do
	cat mojio_report/Run$i/res.txt |grep "txBytes" |cut -d "=" -f 2 |cut -d " " -f 2 >>  mojio_report/AlltxBytes.txt
done
awk '{sum+=$1; sumsq+=$1*$1}END{print sum/NR , sqrt(sumsq/NR - (sum/NR)**2)}' mojio_report/AlltxBytes.txt >> mojio_report/Thro.txt

awk '{z=$2*1.96/5.4772255751;print $1 ,$1 - z,$1 + z}END{}' mojio_report/Thro.txt >> mojio_report/ThroPrint.txt

rm mojio_report/All*
rm mojio_report/Thro.txt
