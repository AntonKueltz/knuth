from unittest import TestCase

from knuth.linked_list import SinglyLinkedList, SinglyLinkedListNode


class TestSinglyLinkedList(TestCase):
    def test_is_empty(self):
        lst = SinglyLinkedList(None)
        self.assertTrue(lst.is_empty())
        
        lst = SinglyLinkedList(SinglyLinkedListNode(1, None))
        self.assertFalse(lst.is_empty())
    
    def test_insert(self):
        lst = SinglyLinkedList(None)
        self.assertEqual(lst.size, 0)
        lst.insert(1)
        self.assertEqual(lst.size, 1)
        lst.insert(2)
        self.assertEqual(lst.size, 2)
        lst.insert(3)
        self.assertEqual(lst.size, 3)

        self.assertEqual(lst.head.value, 3)
        self.assertEqual(lst.head.next.value, 2)
        self.assertEqual(lst.head.next.next.value, 1)
    
    def test_pop(self):
        lst = SinglyLinkedList(None)
        lst.insert(1)
        lst.insert(2)

        self.assertEqual(lst.pop(), 2)
        self.assertEqual(lst.size, 1)
        self.assertEqual(lst.pop(), 1)
        self.assertEqual(lst.size, 0)
    
    def test_pop_empty(self):
        lst = SinglyLinkedList(None)
        
        with self.assertRaises(IndexError):
            lst.pop()
