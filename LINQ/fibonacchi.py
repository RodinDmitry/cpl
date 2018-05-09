from LINQ import Generator
import numpy as np


class Fibonacci:
    def __init__(self):
        self.a = 1
        self.b = 1

    def __next__(self):
        return self.next()

    def next(self):
        current = np.copy(self.a)
        next = np.copy(self.a + self.b)
        self.a = np.copy(self.b)
        self.b = np.copy(next)
        return int(current)


fib = Fibonacci()
gen = Generator(gen=fib)
print(gen.where(lambda x: x % 3 == 0).select(lambda x: x * x if x % 2 == 0 else x).take(5).to_list())
