__author__ = 'andre'

import sys

def main():
    n = int(raw_input())

    sys.stdout.write("\t")
    for i in range(27):
        sys.stdout.write(str(i+1) + "\t")
    for i in range(27):
        sys.stdout.write("\n" + str(i+1)+"\t")
        for j in range(27):
            if (i+1+(j+1)**2)%n==0:
                sys.stdout.write("X.......")
            else:
                sys.stdout.write("........")

    print ""
if __name__ == "__main__":
    main()