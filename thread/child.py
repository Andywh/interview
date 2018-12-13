# EventLoopThread
class Person:
    def __init__(self, name=None, age=None):
        self.name = name
        self.baby_num = 0
        self.child = []

    def name(self):
        return self.name

    def add_child(self):
        self.baby_num += 1

    def birth(self, name=None):
        c = Child(name, self.add_child)
        self.child.append(c)
        c.run()

    def __repr__(self):
        s = 'name: {}, baby_num: {}, child: {}'.format(self.name, self.baby_num, self.child)
        return s


# Thread
class Child:
    def __init__(self, name=None, func=None):
        self.name = name
        self.func = func

    def run(self):
        self.func()

    def __repr__(self):
        s = '{}'.format(self.name)
        return s


def main():
    p = Person("Tony", 14)
    p.birth("W")
    p.birth('Q')
    print(p)

if __name__ == '__main__':
    main()
