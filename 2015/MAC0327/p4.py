(p, dp, t, dt) = map(int, raw_input().split())

if p >= t:
    print p
else:
    while t > p:
        p += dp
        if t <= p:
            print t
        else:
            t -= dt
            if t <= p:
                print p