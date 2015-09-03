__author__ = 'andre'


def main():
    n = int(raw_input())
    bills = {}
    locations = {}
    for i in range(n):
        (bil, location, value) = raw_input().split()
        value = long(value)
        bills[bil] = {'location': location, 'money': value}
        try:
            locations[location] += value
        except KeyError:
            locations[location] = value
    (m, k) = map(int, raw_input().split())
    current_day = 0
    for i in range(k):
        (day, bil, location) = raw_input().split()
        day = int(day)
        if current_day == 0:
            print "Primeira viagem."
            current_day = day
        elif current_day != day:
            print "Novo dia de viagem"


if __name__ == "__main__":
    main()