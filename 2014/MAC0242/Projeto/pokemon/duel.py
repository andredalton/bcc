class Duel():
    def __init__(self):
        self.priority_table = {"Switching out": 6, "item": 6, "escaping": 6, "Quick Attack": 1, "Counter": -5}

    def priority(self, skill):
        try:
            return self.priority_table[skill.get_name()]
        except KeyError:
            return 0

    def faint_message(self, skill1, skill2):
        if skill1.get_owner().get_HP() == 0:
            print("%(name)s fainted!" % {"name": skill1.get_owner().get_name()})
        if skill2.get_owner().get_HP() == 0:
            print("%(name)s fainted!" % {"name": skill2.get_owner().get_name()})

    def execute_duel(self, skill1, skill2):
        skill1.action()
        if skill2.get_owner().get_HP() > 0 and skill1.get_owner().get_HP() > 0:
            skill2.action()
            self.faint_message(skill1, skill2)
        else:
            self.faint_message(skill1, skill2)

    def set_target(self, skill1, skill2):
        skill1.prepare(skill2.get_owner())
        skill2.prepare(skill1.get_owner())

    def duel(self, skill1, skill2):
        p1 = self.priority(skill1)
        p2 = self.priority(skill2)

        self.set_target(skill1, skill2)

        if p1 == p2:
            if skill1.get_owner().get_SPD() >= skill2.get_owner().get_SPD():
                self.execute_duel(skill1, skill2)
            else:
                self.execute_duel(skill2, skill1)
        elif p1 > p2:
            self.execute_duel(skill1, skill2)
        else:
            self.execute_duel(skill2, skill1)