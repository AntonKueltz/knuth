from __future__ import annotations
from dataclasses import dataclass
from timeit import timeit

from knuth.binary_tree import BinaryTree, BinaryTreeNode


class PurePythonBinaryTree[T]:
    @dataclass
    class Node[S]:
        value: S
        left: PurePythonBinaryTree.Node[S] | None
        right: PurePythonBinaryTree.Node[S] | None
    
    root: PurePythonBinaryTree.Node[T] | None
    
    def __init__(self, root):
        self.root = root
    
    def inorder(self, visit):
        def helper(node):
            if node is None:
                return
            helper(node.left)
            visit(node)
            helper(node.right)
        
        helper(self.root)

       
def get_test_tree(cls: type, node_cls: type):
        root = node_cls("A", None, None)
        root.left = node_cls("B", None, None)
        root.left.left = node_cls("D", None, None)
        root.right = node_cls("C", None, None)
        root.right.left = node_cls("E", None, None)
        root.right.right = node_cls("F", None, None)
        root.right.left.right = node_cls("G", None, None)
        root.right.right.left = node_cls("H", None, None)
        root.right.right.right = node_cls("J", None, None)
        return cls(root)



def visit(node):
    pass

n = 1000000
bt = get_test_tree(BinaryTree, BinaryTreeNode)
ppbt = get_test_tree(PurePythonBinaryTree, PurePythonBinaryTree.Node)
bt_time = timeit(stmt=lambda: bt.inorder(visit), number=n)
ppbt_time = timeit(stmt=lambda: ppbt.inorder(visit), number=n)

print (f"===== {n} iterations =====")
print(f"C Extension type: {bt_time}")
print(f"Pure Python type: {ppbt_time}")
