#! /bin/sh

for i in *-start.c; do
  cp $i ${i%%-start.c}-demo.c
done
for i in *-start.cl; do
  cp $i ${i%%-start.cl}-demo.cl
done
