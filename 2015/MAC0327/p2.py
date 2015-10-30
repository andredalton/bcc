__author__ = 'avale'

def main():
    buff = map(int, raw_input().split())
    a = (buff[0], buff[1])
    x1 = (buff[2]-a[0], buff[3]-a[1])
    x2 = (buff[4]-a[0], buff[5]-a[1])

    print a, x1, x2

if __name__ == '__main__':
    main()