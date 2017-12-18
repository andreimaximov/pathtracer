#!/bin/bash

rm -rf ./.cache
mkdir -p ./.cache
cd ./.cache

curl -O http://g3d.cs.williams.edu/g3d/data10/research/model/lost_empire/lost-empire.zip
mkdir -p ./lost-empire
unzip ./lost-empire.zip -d ./lost-empire

curl -O http://g3d.cs.williams.edu/g3d/data10/research/model/rungholt/rungholt.zip
mkdir -p ./rungholt
unzip ./rungholt.zip -d ./rungholt

touch ./ok
