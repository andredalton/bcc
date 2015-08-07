#!/usr/bin/env bash
for file in HoaxImages/*
do
    python ela.py $file
done