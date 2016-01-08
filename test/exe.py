#!/usr/bin/env python
# -*- coding: utf8 -*-
import os
path = os.getcwd() 
print('Please give the way of your file:')
c= raw_input()
#f=open(c,'r')
#p = os.system('make')
os.system('clang -c -emit-llvm '+c+' -o a.bc')
os.system('make')
os.system('../../../Debug+Asserts/bin/opt -load ../../../Debug+Asserts/lib/test.so -T a.bc -o aa.bc')
print('**************************************After pasring **********************************************')
os.system('../../../Debug+Asserts/bin/lli  aa.bc')
