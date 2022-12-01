from random import *
import os
import sys
import subprocess

orig_stdout = sys.stdout
log = open('out.log', 'w')
os.system("g++ main.cpp -o main")

for j in range(1, 100):
    sys.stdout = log
    print("Test " + str(j) + ": ", end='')

    #generate input.txt with random numbers
    f = open('input.txt', 'w')
    sys.stdout = f
    for i in range(0, 10000):
        p = randrange(1, 10)
        if p == 5:
            print('\n')
            i -= 1
        else:
            print(str(randrange(0, 999999)) + "\t" + str(randrange(0, 2**64 - 1)))

    sys.stdout = log
    result = subprocess.run(['./main', 'input.txt'], stdout=subprocess.PIPE)
    print(result.stdout.decode('utf-8'))

    sys.stdout = orig_stdout
    result1 = subprocess.run(['echo', 'test ' + str(j) + ': ' + result.stdout.decode('utf-8')], stdout=subprocess.PIPE)
    print(result1.stdout.decode('utf-8'), end='')
    os.system("cp input.txt files/" + "input" + str(j) + ".txt")

sys.stdout = orig_stdout
log.close()
f.close()
os.system("rm input.txt my_output.txt std_output.txt main")