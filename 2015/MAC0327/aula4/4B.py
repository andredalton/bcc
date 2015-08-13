__author__ = 'andre'

primos = {1:2, 2:3}

def enesimo(n):
	try:
		return primos[n]
	except KeyError:
		i = len(primos)
		primo = primos[len(primos)]
		for i in range(len(primos)+1, n+1):
			k = 2
			while(True):
				for nprimo in primos.values():
					if (primo+k)%nprimo==0:
						k += 2
						break
					if nprimo**2 > primo+k:
						break
				if nprimo**2 > primo+k:
					break
			primo += k
			primos[i] = primo
		return primos[n]

def main():
	k = int(raw_input())
	
	indices = []
	for i in range(k):
		n = int(raw_input())
	
	print max(indices)

if __name__ == "__main__":
	main()
