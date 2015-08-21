__author__ = 'andre'

from datetime import datetime

def bin_search(lst, value):
    if len(lst) == 0:
        return 0
    if value > lst[len(lst)-1][0]:
        return len(lst)
    l = 0
    r = len(lst)
    m = r/2
    while r-l > 1:
        if lst[m][0] > value:
            r = m
        else:
            l = m
        m = (r+l)/2
    return m

def bin_add(lst, cell):
    pos = bin_search(lst, cell[0])
    if pos == 0:
        cell.append(cell[1])
    else:
        cell.append(lst[pos-1][2]+cell[1])
    lst.insert(pos, cell)
    for i in range(pos+1, len(lst)):
        lst[i][2] = lst[i-1][2] + lst[i][1]

def main():
    n = int(raw_input())
    sums = []

    for i in range(n):
        (credit, day, hour) = raw_input().split()
        credit = long(credit)
        (day, month) = map(int, day.split('.'))
        (hour, minute) = map(int, hour.split(':'))
        dt = datetime(year=2042, day=day, month=month, hour=hour, minute=minute)
        bin_add(sums, [dt, credit])
        divida = 0
        for cr in sums[::-1]:
            if cr[1] < 0:
                divida = cr[2]
                break
        print divida if divida < 0 else 0


if __name__ == "__main__":
    main()
