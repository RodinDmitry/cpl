import collections

import numpy as np


class Generator(object):
    def __init__(self, generation_list=[], gen=None):
        assert (len(generation_list) <= 0 or gen is None)
        self.n = np.inf
        self.filter = None
        self.function = None
        self.num = 0
        self.prev_generator = gen
        self.generation_list = generation_list
        if len(generation_list) > 0:
            self.n = len(generation_list)
            self.from_list = True
        else:
            self.from_list = False

    def __iter__(self):
        return self

    def __next__(self):
        return self.next()

    def add_layer(self):
        new_generator = Generator()
        new_generator.prev_generator = self
        return new_generator

    def select(self, expression):
        new_generator = self.add_layer()
        new_generator.function = expression
        return new_generator

    def flatten(self):
        flat_list = []
        for item in self:
            if isinstance(item, collections.Sequence):
                for x in item:
                    flat_list.append(x)
            else:
                flat_list.append(item)
        new_generator = Generator()
        new_generator.from_list = True
        new_generator.generation_list = flat_list
        new_generator.n = len(flat_list)
        return new_generator

    def where(self, expression):
        new_generator = self.add_layer()
        new_generator.filter = expression
        return new_generator

    def take(self, n):
        self.n = n
        return self

    def group_by(self, key=lambda x: x):
        prev_layer_list = self.to_list()
        prev_layer_list = sorted(prev_layer_list, key=key)
        groups = []
        keys = []
        for item in prev_layer_list:
            if key(item) not in keys:
                groups.append((key(item), [item]))
                keys.append(key(item))
            else:
                index = keys.index(key(item))
                groups[index][1].append(item)
        return Generator(groups)

    def order_by(self, expression):
        prev_layer_list = self.to_list()
        prev_layer_list = sorted(prev_layer_list, key=expression)
        new_generator = Generator()
        new_generator.from_list = True
        new_generator.generation_list = prev_layer_list
        new_generator.n = len(prev_layer_list)
        return new_generator

    def to_list(self):
        result = []
        for item in self:
            result.append(item)
        return result

    def next(self):
        if self.num < self.n:
            if self.from_list:
                current = self.generation_list[self.num]
            else:
                if self.prev_generator is None:
                    current = self.num
                else:
                    while True:
                        current = self.prev_generator.next()
                        if self.filter is None:
                            break
                        if self.filter(current):
                            break
            self.num += 1
            if self.function is None:
                return current
            current = self.function(current)
            return current
        else:
            raise StopIteration()
