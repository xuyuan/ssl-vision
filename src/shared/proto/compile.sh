#!/bin/sh
protoc --cpp_out=cpp *.proto
protoc --python_out=python *.proto
