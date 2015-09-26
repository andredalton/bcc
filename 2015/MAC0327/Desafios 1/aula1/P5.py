# coding=utf-8
__author__ = 'andre'

"""
/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Aula 1 - Desafio E
 * MAC0327 -- 12/07/2015 -- IME/USP, -- Prof. Cristina Gomes Fernandes
 ******************************************************************************/
"""

def backtrack(index, parts):
    if index == len(parts):
        return True

    for actor in parts[index]['actors']:
        if actor['receiver'] == 0:
            continue
        actor['receiver'] -= 1
        parts[index]['actor'] = actor['index']
        if not backtrack(index+1, parts):
            actor['receiver'] += 1
            parts[index]['actor'] = False
        else:
            return True
    return False

def main():
    nparts = int(raw_input())
    parts = []
    for i in range(nparts):
        (start, end) = map(int, raw_input().split())
        parts.append({'index': i+1, 'start': start, 'end': end, 'actors': [], 'actor': False})
    nactors = int(raw_input())
    posibilities = 0
    actors = []
    for i in range(nactors):
        (start, end, receiver) = map(int, raw_input().split())
        actors.append({'index': i+1, 'start': start, 'end': end, 'receiver': receiver})
        posibilities += receiver
        for part in filter(lambda x: x['start'] >= actors[i]['start'] and x['end'] <= actors[i]['end'], parts):
            part['actors'].append(actors[i])

    parts.sort(key=lambda x: x['start'])
    for part in parts:
        part['actors'].sort(key=lambda x: x['end'])

    if posibilities < nparts or len(parts[0]['actors']) == 0:
        print "NO"
    else:
        if backtrack(0, parts):
            parts.sort(key=lambda x: x['index'])
            print "YES"
            print " ".join(map(lambda x: str(x['actor']), parts))
        else:
            print "NO"

if __name__ == "__main__":
    main()
