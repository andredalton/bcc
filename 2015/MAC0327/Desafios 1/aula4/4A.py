__author__ = 'andre'

if __name__ == "__main__":
    k = int(raw_input())
    odds = map(int, raw_input().split())
    
    vs = []
    for odd in odds:
		vs.append(1+odd/2)
    vs.sort()
    
    print sum(vs[:(1+len(vs))/2])
