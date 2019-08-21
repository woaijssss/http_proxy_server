#!/bin/bash

cat \
        include/*.h \
        include/http/*.h \
        include/factory/* \
        include/register/* \
        src/*.cpp \
        src/http/*.cpp \
        src/factory/* \
        src/register/* \
        pub/*.h \
        pub/*.cpp \
        pub/pub_macro/* \
        pub/stdio/* \
        pub/pub_log/*\
        pub/stl/*\
	tests/* \
	test_develop/* \
  > 1
