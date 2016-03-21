# coding=utf-8
"""
Nome: André Meneghelli Vale
NUSP: 4898948
Problema P20 - B. Counting Rhombi
"""

def main():
    (h, w) = map(int, raw_input().split())
    print (h/2)*(h-h/2)*(w/2)*(w-w/2)


if __name__ == '__main__':
    main()
