import string

nodes = 0

def add_word(word, root):
	global nodes
	if len(word) > 0:
		c = word[0]
		if root[c] is None:
			nodes += 1
			root[c] = dict.fromkeys(string.ascii_lowercase, None)
		add_word(word[1:], root[c])

def main():
	global nodes
	word = raw_input()
	nodes = 0
	root = dict.fromkeys(string.ascii_lowercase, None)
	
	for i in range(len(word)):
		add_word(word[i:], root)
	
	print nodes	
	return 0
	
if __name__ == "__main__":
	main()
