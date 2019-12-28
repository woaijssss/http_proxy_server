#!/bin/bash

cat \
        include/*.h \
	include/config/*.h \
        include/factory/*.h \
        include/http/*.h \
	include/openssl/*.h \
        include/register/*.h \
        src/*.cpp \
	src/config/*.cpp \
        src/factory/* \
        src/http/*.cpp \
	src/openssl/* \
        src/register/* \
        pub/*.h \
        pub/*.cpp \
        pub/pub_log/*\
        pub/pub_macro/* \
        pub/stdio/* \
        pub/stl/*\
	test_develop/* \
  > 1
