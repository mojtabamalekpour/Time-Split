#!/bin/bash
for l in {0..5}
do
  mkdir mojio_report/l$l/
  for i in {1..30}
  do	
	cat mojio_report/level$l/Run$i/res.txt |grep "Scheduled lists" | cut -d " " -f 5|sort -n &>  mojio_report/l$l/Run$i.txt	
  done
done
