from curses.ascii import FF
from random import *
import os
import sys
import subprocess
import string

def randomword(length):
   letters = string.ascii_lowercase
   return ''.join(choice(letters) for i in range(length))

#orig_stdout = sys.stdout
#log = open('log.txt', 'w')
#os.system("cd ..; make; time make run; cd benchmarks")

for j in range(3, 7):
    #sys.stdout = log
    #print("Test " + str(j) + ": ", end='')

    #generate input.txt with random numbers
    fInsert     = open('tests/Insert__inputPow10_'     + str(j) + '.txt', 'w')
    fInsertSave = open('tests/InsertSave__inputPow10_' + str(j) + '.txt', 'w')
    fFind       = open('tests/Find__inputPow10_'       + str(j) + '.txt', 'w')
    fDelete     = open('tests/Delete__inputPow10_'     + str(j) + '.txt', 'w')
    for i in range(0, 10**j):
        word = randomword(randrange(1, 256)) 
        print("+ " + word + " " + str(randrange(0, 2**64 - 1)), file=fInsert)
        print("+ " + word + " " + str(randrange(0, 2**64 - 1)), file=fInsertSave)
        print(word, file=fFind)
        print("- " + word, file=fDelete)
    print("! Save dict_pow" + str(j) + ".bin", file=fInsertSave)


fInsert.close()
fDelete.close()
fFind.close()
fInsertSave.close()
#os.system("rm my_output.txt std_output.txt")