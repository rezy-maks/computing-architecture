#!/bin/bash

memoryType=$1
path=$2
echo $2
s1="MemoryType;BlockSize;ElementType;BufferSize;LaunchNum;Timer;"
s2="WriteTime;AverageWriteTime;WriteBandWIdth;AbsError(write);RelError(write);"
s3="ReadTime;AverageReadTime;ReadBandWidth;AbsError(read);RelError(read)"

echo ${s1}${s2}${s3} > file.csv

size=4
launchNum=5
if [[ $memoryType == "ln" ]]
then
	for ((idx=1;idx<22;idx++))
	do
		echo ./ABC_lab3 -m SSD -b 4Mb -l ${launchNum} -p /home/vladislav/CLionProjects/ABC_lab3
		echo $(./ABC_lab3 -m SSD -b 4Mb -l ${launchNum} -p /home/vladislav/CLionProjects/ABC_lab3)
		launchNum=$((launchNum+1))
	done
fi
if [[ $memoryType == "All" ]]
then
	echo ./ABC_lab3 -m RAM -b 64 -l 10
	echo $(./ABC_lab3 -m RAM -b 64 -l 10 -p path)
	echo ./ABC_lab3 -m RAM -b 256Kb -l 10
	echo $(./ABC_lab3 -m RAM -b 256Kb -l 10 -p path)
	echo ./ABC_lab3 -m RAM -b 1Mb -l 10
	echo $(./ABC_lab3 -m RAM -b 1Mb -l 10 -p path)
	echo ./ABC_lab3 -m RAM -b 6Mb -l 10
	echo $(./ABC_lab3 -m RAM -b 6Mb -l 10 -p path)
	echo ./ABC_lab3 -m RAM -b 12Mb -l 10
	echo $(./ABC_lab3 -m RAM -b 12Mb -l 10 -p path)
	for ((idx=1;idx<21;idx++))
	do
		echo ./ABC_lab3 -m HDD -b ${size}Mb -l 10 -p /media/vladislav/Expansion/test
		echo $(./ABC_lab3 -m HDD -b ${size}Mb -l 10 -p /media/vladislav/Expansion/test)
		size=$((size+4))
	done
	size=4
	for ((idx=1;idx<21;idx++))
	do
		echo ./ABC_lab3 -m SSD -b ${size}Mb -l 10 -p /home/vladislav/CLionProjects/ABC_lab3/cmake-build-debug

		echo  $(./ABC_lab3 -m SSD -b ${size}Mb -l 10 -p /home/vladislav/CLionProjects/ABC_lab3/cmake-build-debug)
		size=$((size+4))
	done
	size=4
	for ((idx=1;idx<21;idx++))
	do
		echo ./ABC_lab3 -m flash-b ${size}Mb -l 10 -p /media/vladislav/USB
		echo  $(./ABC_lab3 -m flash -b ${size}Mb -l 10 -p /media/vladislav/USB)
		size=$((size+4))
	done
fi
if [[ $memoryType == "RAM" ]]
then
	echo ./ABC_lab3 -m RAM -b 28Mb -l 10
	echo $(./ABC_lab3 -m RAM -b 28Mb -l 10 -p ${path})
elif [[ $memoryType == "HDD" ]]
then
	for ((idx=1;idx<21;idx++))
	do
		echo ./ABC_lab3 -m HDD -b ${size}Mb -l 10 -p ${path}
		echo $(./ABC_lab3 -m HDD -b ${size}Mb -l 10 -p ${path})
		size=$((size+4))
	done
	memoryType=2
elif [[ $memoryType == "SSD" ]]
then
	for ((idx=1;idx<21;idx++))
	do
		echo ./ABC_lab3 -m SSD -b ${size}Mb -l 10 -p ${path}
		echo  $(./ABC_lab3 -m SSD -b ${size}Mb -l 10 -p ${path})
		size=$((size+4))
	done
	memoryType=3
elif [[ $memoryType == "flash" ]]
then
	for ((idx=1;idx<21;idx++))
	do
		echo ./ABC_lab3 -m flash-b ${size}Mb -l 10 -p ${path}
		echo  $(./ABC_lab3 -m flash -b ${size}Mb -l 10 -p ${path})
		size=$((size+4))
	done
	memoryType=4
fi


