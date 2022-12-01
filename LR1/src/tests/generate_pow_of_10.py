from random import *
import os
import sys
import subprocess

orig_stdout = sys.stdout
log = open('log.txt', 'w')
os.system("g++ main.cpp -o main")

for j in range(3, 7):
    sys.stdout = log
    print("Test " + str(j) + ": ", end='')

    #generate input.txt with random numbers
    f = open('inputPow10_' + str(j) + '.txt', 'w')
    sys.stdout = f
    for i in range(0, 10**j):
        print(str(randrange(0, 999999)) + "\t" + str(randrange(0, 2**64 - 1)))

sys.stdout = orig_stdout
log.close()
f.close()
os.system("rm my_output.txt std_output.txt")