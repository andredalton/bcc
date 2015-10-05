__author__ = 'avale'

# from pprint import pprint

def main():
    mapa = {}
    floodgate = raw_input()
    buff = raw_input()
    used = set()
    exists = set()
    has_mapa = False
    while buff != "#":
        has_mapa = True
        (orig, dest) = buff.split("-")
        exists.add(orig)
        exists.add(dest)
        try:
            mapa[orig].append(dest)
        except KeyError:
            mapa[orig] = [dest]
        try:
            mapa[dest].append(orig)
        except KeyError:
            mapa[dest] = [orig]
        buff = raw_input()

    opens = 0
    if has_mapa:
        floodrooms = [floodgate]
        used.add(floodgate)
        while len(floodrooms) > 0:
            nfloods = []
            for room in floodrooms:
                for neighbor in mapa[room]:
                    if neighbor not in used:
                        opens += 1
                        used.add(neighbor)
                        nfloods.append(neighbor)
            floodrooms = nfloods

    print opens
    # print floodgate
    # pprint(mapa)
    # pprint(exists)

if __name__ == '__main__':
    main()