#!/bin/bash

cat \
	include/*.h \
	include/http/*.h \
	include/factory/* \
	src/*.cpp \
	src/http/*.cpp \
	src/factory/* \
	pub/*.h \
	pub/*.cpp \
	pub/pub_macro/* \
	pub/stdio/* \
	pub/pub_log/*\
  > 1
