#!/bin/bash

BIN=bin

if [ $# == 1 ]; then
        if [ $1 == "clean" ]; then
                rm -rf $BIN
                exit 0
        fi
fi

mkdir -p $BIN

g++ test_thread.cpp ../pub/CPPThread.cpp ../pub/ThreadPool.cpp -I../pub -pthread -std=c++11 -o $BIN/test_thread

g++ test_log.cpp ../pub/pub_log/WHPSLog.cpp ../pub/String.cpp ../pub/Codec.cpp ../pub/stdio/WHPSStdioBase.cpp -I../pub -I../pub/stdio -I../pub/pub_log/ -std=c++11 -Wall -pedantic -Wextra -std=c++11 -MMD -D_GLIBCXX_USE_NANOSLEEP -Wno-deprecated -Wdeprecated-declarations -o $BIN/test_log

g++ test1.cpp -std=c++11 -g -o $BIN/test1

g++ test_config.cpp -std=c++11 -g -o $BIN/test_config

g++ test_file.cpp -std=c++11 -g -o $BIN/test_file

g++ test_map.cpp -std=c++11 -I../pub/ -g -o $BIN/test_map

g++ DynamicFactory.cpp -I. -std=c++11 -g -fPIC -shared -o libdymc.so
g++ test_reflect.cpp -I. -std=c++11 -g -o $BIN/test_reflect

g++ test_vector.cpp -std=c++11 -I../pub -g -o $BIN/test_vector
