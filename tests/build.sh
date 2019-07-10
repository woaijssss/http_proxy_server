#!/bin/bash

g++ test_thread.cpp ../pub/CPPThread.cpp ../pub/ThreadPool.cpp -I../pub -pthread -std=c++11 -o test_thread
