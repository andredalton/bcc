__author__ = 'avale'

def walk(members, isanumb, names, index):
    lst = []
    for name in names:
        if isanumb[name] == 'undefined' or isanumb[name] > index:
            lst.append(name)
            isanumb[name] = index
    for name in lst:
        walk(members, isanumb, members[name], index+1)

def main():
    n = int(raw_input())
    members = {}
    isanumb = {}

    for i in range(n):
        names = raw_input().split()
        for j in range(3):
            if not isanumb.has_key(names[j]):
                isanumb[names[j]] = 'undefined'

            if not members.has_key(names[j]):
                members[names[j]] = set()
            for k in range(3):
                if k != j:
                    members[names[j]].add(names[k])

    if isanumb.has_key('Isenbaev'):
        walk(members, isanumb, ['Isenbaev'], 0)
    names = isanumb.keys()
    names.sort()
    for name in names:
        print name, isanumb[name]
    return True

if __name__ == "__main__":
    main()