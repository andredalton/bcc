def main():
    pontos = []
    n = int(raw_input())

    for i in range(n):
        x, y = map(int, raw_input().split())
        pontos.append((x, y))

    contador = 0
    for i in xrange(n):
        for j in xrange(i+1, n):
            if (pontos[i][0] + pontos[j][0])%2==0 and (pontos[i][1] + pontos[j][1])%2==0:
                ponto = (int((pontos[i][0]+pontos[j][0])/2), int((pontos[i][1] + pontos[j][1])/2))
                if ponto in pontos:
                    contador += 1

    print contador

if __name__ == '__main__':
    main()

#
# const int MAXN = 3001;
#
# int main()
# {
# 	int n;
# 	cin >> n;
# 	set<point> pts;
#
# 	for (int i = 0; i < n; ++i) {
# 		point p;
# 		cin >> p.x >> p.y;
# 		pts.insert( p);
# 	}
# 	int cnt = 0;
# 	for (set<point>::iterator it1 = pts.begin(); it1 != pts.end(); it1++) {
# 		set<point>::iterator it2 = it1;
# 		for (it2++; it2 != pts.end(); it2++) {
# 			if (((it1->x + it2->x) % 2 == 0) && ((it1->y + it2->y) % 2 == 0)) {
# 				point p;
# 				p.x = (it1->x + it2->x) / 2;
# 				p.y = (it1->y + it2->y) / 2;
# 				if (pts.find( p) != pts.end()) {
# 					cnt++;
# 				}
# 			}
# 		}
# 	}
# 	cout << cnt << endl;
#
# 	return 0;
# }



# import sys
# from pprint import pprint
#
# def main():
#     all_input = iter(sys.stdin.read().strip().replace('\n', ' ').split(' '))
#     n = int(all_input.next())
#
#     mtz = []
#     for i in xrange(8):
#         mtz.append([False]*8)
#
#     pts = []
#     pts_hash = {}
#     for i in xrange(n):
#         x = 4 + int(all_input.next())
#         y = 4 + int(all_input.next())
#         pts.append((x, y))
#         mtz[x][y] = True
#         # try:
#         #     pts_hash[x].append(y)
#         # except KeyError:
#         #     pts_hash[x] = [y]
#
#     # pprint(mtz)
#
#     qt = 0
#     count = set()
#     for i in xrange(n):
#         for j in xrange(i+1, n):
#             deltax = pts[j][0] - pts[i][0]
#             deltay = pts[j][1] - pts[i][1]
#             if mtz[pts[j][0]+deltax][pts[j][1]+deltay]:
#                 qt += 1
#                 print pts[i], pts[j], (pts[j][0]+deltax, pts[j][1]+deltay)
#
#
#
#     #         try:
#     #             if pts[j][1]+deltay in pts_hash[pts[j][0]+deltax]:
#     #                 aux = [pts[i], pts[j], (pts[j][0]+deltax, pts[j][1]+deltay)]
#     #                 aux.sort()
#     #                 count.add(str(aux))
#     #                 # count.add(set([pts[i], pts[j], (pts[j][0]+deltax, pts[j][1]+deltay)]))
#     #                 # print pts[i], pts[j]
#     #                 qt += 1
#     #         except KeyError:
#     #             pass
#     #         # print i, j
#
#     # print pts
#     # print pts_hash
#     # pprint(count)
#     # print len(count)
#     print qt
#
# if __name__ == '__main__':
#     main()