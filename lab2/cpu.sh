#!/bin/bash


echo "Model,Task,OpType,Opt,InsCount,Timer,Time,LNum,AvTime,AbsErr,RelError,TaskPerf" > file.csv

model=$(lscpu | grep "Имя" | cut -b 24-)
task="dgemm"
OpType="int"
opt="none"
InsCount="1e+08"
Timer="wtime"
LNum=10

for ((idx=1;idx<13;idx++))
do
	if [[ $idx == 1 ]]
	then
		echo -e $(g++ -O0 -Wall -o test cpuTest.cpp -lm)
		opt="-O0"
	fi
	
	if [[ $idx == 4 ]]
	then
		echo -e $(g++ -O1 -Wall -o test cpuTest.cpp -lm)
		opt="-O1"
	fi
	
	if [[ $idx == 7 ]]
	then
		echo -e $(g++ -O2 -Wall -o test cpuTest.cpp -lm)
		opt="-O2"
	fi
	
	if [[ $idx == 10 ]]
	then
		echo -e $(g++ -O3 -Wall -o test cpuTest.cpp -lm)
		opt="-O3"
	fi
	
	if [[ $(($idx%3)) == 1 ]]
	then
		echo -e $(./test int)
		OpType="int"
		echo -e "./test int\n"
	fi
	
	if [[ $(($idx%3)) == 2 ]]
	then
		echo -e $(./test float)
		OpType="float"
		echo -e "./test float\n"
	fi
	
	if [[ $(($idx%3)) == 0 ]]
	then
		echo -e $(./test double)
		OpType="double"
		echo -e "./test double\n"
	fi
	
	out1=${model}";"${task}";"${OpType}";"${opt}";"${InsCount}";"${Timer}";"
	out2=$(cat omega.txt | grep "TimeBuf" | awk '{print $6}')";"${LNum}";"
	out3=$(cat omega.txt | grep "AvTime" | awk '{print $2}')";"$(cat omega.txt | grep "absErr" | awk '{print $2}')";"
	out4=$(cat omega.txt | grep "relErr" | awk '{print $2}')";"$(cat omega.txt | grep "W" | awk '{print $2}MIps')
	echo -e ${out1} ${out2} ${out3} ${out4} >> file.csv
	
done
