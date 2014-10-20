#! /usr/bin/env python3

def priority(skill):
    priority_table = {"Switching out": 6, "item": 6, "escaping": 6, "Quick Attack": 1, "Counter": -5}
    try:
        return priority_table[skill.get_name()]
    except KeyError:
        return 0

def execute_duel(skill1, skill2):
    skill1.action()
    if skill2.get_owner().get_HP() > 0:
    	skill2.action()
    else:
    	print("%(name)s fainted!" % {"name": skill2.get_owner().get_name()})

def set_target(skill1, skill2):
	skill1.prepare(skill2.get_owner())
	skill2.prepare(skill1.get_owner())

def duel(skill1, skill2):
    p1 = priority(skill1)
    p2 = priority(skill2)

    set_target(skill1, skill2)

    if p1 == p2:
    	if skill1.get_owner().get_SPD() >= skill2.get_owner().get_SPD():
    		execute_duel(skill1, skill2)
    	else:
    		execute_duel(skill2, skill1)
    elif p1 > p2:
    	execute_duel(skill1, skill2)
    else:
    	execute_duel(skill2, skill1)
        