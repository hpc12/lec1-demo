#! /bin/sh

for i in *-start.c; do
  echo cp $i ${i%%-start.c}-demo.c
done
