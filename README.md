# Time-Split 

This is the ns3 implimentation of our paper, Slythrin. 

https://link.springer.com/article/10.1007/s11277-016-3347-0



# Run
```
cd <to the directory>
```
Configure code using this commmand:
```
CXXFLAGS=“-Wall -O0” ./waf -d optimized configure
```
Build the code using this command:
```
./waf
```
Finally run the code:
```
./waf --run “scratch/mesh --load=0.6 --dctcp=1 --filename=Stats.txt --runtime=0.1”
```


