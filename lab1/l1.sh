#!/bin/bash

export NAME=$(whoami)
export DATE=$(date)
export DOMEN=$(hostname)
export MODEL_PROC=$(lscpu | grep 'Имя модели')
export ARCH=$(lscpu | grep 'Архитектура')
export TAKT=$(lscpu | grep 'CPU МГц')
export YADRA=$(lscpu | grep 'Ядер на сокет')
export POTOK=$(lscpu | grep 'Потоков на ядро')
export ALL_MEM=$(free -g | grep 'Память' | awk '{print $2}')"G"
export FREE_MEM=$(free -g | grep 'Память' | awk '{print $6}')"G"
export ALL_DRIVE=$(df -h --total | grep 'total' | awk '{print $2}')
export FREE_DRIVE=$(df -h --total | grep 'total' | awk '{print $4}')
export KOREN_DIR=$(df -h | grep '/dev/sda6' | awk '{print $2}')
export SWAP_ALL=$(free -g | grep 'Подкачка' | awk '{print $2}')"G"
export SWAP_FREE=$(free -g | grep 'Подкачка' | awk '{print $4}')"G"
RED='\033[0;31m'
NC='\033[0m'

echo Дата: $DATE
echo Имя учетной записи: $NAME
echo Доменное имя ПК: $DOMEN
echo -e Процессор:
echo -e "\t" $MODEL_PROC
echo -e "\t" $ARCH
echo -e "\t" $TAKT
echo -e "\t" $YADRA
echo -e "\t" $POTOK
echo -e "${RED}Оперативная память:${NC}"
echo -e "\t" Всего - $ALL_MEM
echo -e "\t" Доступно - $FREE_MEM
echo -e "${RED}Жесткий диск:${NC}"
echo -e "\t" Всего - $ALL_DRIVE
echo -e "\t" Доступно - $FREE_DRIVE
echo -e "\t" Смонтировано в корневую директорию: $KOREN_DIR
echo -e "\t" SWAP всего: $SWAP_ALL
echo -e "\t" SWAP доступно: $SWAP_FREE

echo -e "${RED}Сетевые интерфейсы:${NC}"
ifCount=$(ifconfig -s | wc -l | awk '{r = $1 - 1} {print r}')
echo -e "\t Количество сетевых интерфейсов: ${ifCount}"
echo -e "${RED}№\tИмя\tMAC\t\t\tIP\t\t\tСкорость${NC}"
for ((idx=1; idx <= $(($ifCount)); idx++))
do
        name=$(ifconfig -s | head -n $(($idx + 1)) | tail -n 1 | awk '{print $1}')
        mac=$(ifconfig -a | grep ether | head -n $(($idx + 1)) | tail -n 1 | awk '{print $2}')
        ip=$(ip -4 addr| grep inet | head -n $(($idx)) | tail -n 1 | awk '{print $2}')
	speed=$(ip addr | grep qlen | head -n $(($idx)) | tail -n 1 | awk '{print $13}')
	if [ $name == "lo" ]
	then
		mac="-\t\t"
	fi
	if [[ $lastip == $ip ]]
	then
		ip="-\t"
	fi
        echo -e "${idx}\t${name}\t${mac}\t${ip}\t\t${speed}"
	lastip=$ip
done

