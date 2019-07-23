#!/bin/bash

#g++ test_thread.cpp ../pub/CPPThread.cpp ../pub/ThreadPool.cpp -I../pub -pthread -std=c++11 -o test_thread

g++ test_log.cpp ../pub/pub_log/WHPSLog.cpp ../pub/String.cpp ../pub/stdio/WHPSStdioBase.cpp -I../pub -I../pub/stdio -I../pub/pub_log/ -std=c++11 -o test_log
