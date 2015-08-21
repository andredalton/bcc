__author__ = 'andre'


def main():
    n = int(raw_input())
    bills = {}
    locations = set()
    for i in range(n):
        (bil, location, value) = raw_input().split()
        value = long(value)
        bills[bil] = (location, value)
        locations.add(location)
    (m, k) = map(int, raw_input(.split()))
    current_day = 0
    for i in range(k):
        (day, bil, location) = raw_input().split()
        day = int(day)
        if day != current_day


if __name__ == "__main__":
    main()