from random import randint
from unittest import TestCase

from knuth.linear_list import Stack


class TestStack(TestCase):
    def test_init(self):
        stack = Stack(0)
        stack.__init__(1)
    
    def test_insert_and_delete(self):
        n = 10
        stack = Stack(n)
        values = [randint(1, 100) for _ in range(n)]
        
        for value in values:
            stack.insert(value)
        
        for i in range(n-1, -1, -1):
            self.assertEqual(stack.delete(), values[i])
    
    def test_push_and_pop(self):
        n = 10
        stack = Stack(n)
        values = [randint(1, 100) for _ in range(n)]
        
        for value in values:
            stack.push(value)
        
        for i in range(n-1, -1, -1):
            self.assertEqual(stack.pop(), values[i])
            
    def test_insert_overflow(self):
        stack = Stack(0)
        
        with self.assertRaises(IndexError):
            stack.insert("overflow")
    
    def test_insert_underflow(self):
        stack = Stack(1)
        
        with self.assertRaises(IndexError):
            stack.delete()
