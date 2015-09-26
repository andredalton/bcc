nodes = 0

def new_hash():
    return dict(a=None, b=None, c=None, d=None, e=None, f=None, g=None, h=None, i=None, j=None, k=None, l=None, m=None,
                n=None, o=None, p=None, q=None, r=None, s=None, t=None, u=None, v=None, w=None, x=None, y=None, z=None)

def add_word(word, root):
    global nodes
    if len(word) > 0:
        c = word[0]
        if root[c] is None:
            nodes += 1
            root[c] = new_hash()
        add_word(word[1:], root[c])

def main():
    global nodes
    word = raw_input()
    nodes = 0
    root = new_hash()

    for i in range(len(word)):
        add_word(word[i:], root)

    print nodes
    return 0

if __name__ == "__main__":
    main()
