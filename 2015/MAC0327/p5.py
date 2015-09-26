(v, t) = map(int, raw_input().split())
e = map(int, raw_input().split())

fila = 0

for s in e:
    fila += s
    fila -= v
    if fila < 0:
        fila = 0

print fila