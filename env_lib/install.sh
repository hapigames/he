#!/bin/bash

user=`who am i | cut -d ' ' -f 1`

if [ $user != "root" ]
then
    echo "please run $0 as root, now is a $user"
#    exit 1
fi

#cmake
tar xf cmake-2.8.12.2.tar.gz
cd cmake-2.8.12.2
sh bootstrap
make && make install
cd ..
rm -rf cmake-2.8.12.2


#libconfig
tar xf libconfig-1.4.9.tar.gz
cd libconfig-1.4.9
./configure --prefix=/usr/local/
make && make install
cd ..
rm -rf libconfig-1.4.9


#libmysqlclient
tar xf mysql-connector-c-6.1.3-src.tar.gz 
cd mysql-connector-c-6.1.3-src
cmake .
make && make install
cd ..
rm -rf mysql-connector-c-6.1.3-src

#log4cxx
tar xf apr-1.4.8.tar.gz
cd apr-1.4.8
./configure --prefix=/usr/local/
make && make install
cd ..
rm -rf apr-1.4.8

tar xf apr-util-1.5.2.tar.gz
cd apr-util-1.5.2
./configure --prefix=/usr/local/ --with-apr=/usr/local
make && make install
cd ..
rm -rf apr-util-1.5.2

cd apache-log4cxx-0.10.0
./configure --prefix=/usr/local/ --with-apr=/usr/local/ --with-apr-util=/usr/local/apr/ 
make && make install
cd ..


#libcurl

#tar xf curl-7.36.0.tar.gz
#cd curl-7.36.0
#./configure --prefix=/usr/local/
#make && make install
#cd ..
#rm -rf curl-7.36.0







