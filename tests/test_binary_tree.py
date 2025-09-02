from unittest import TestCase

from knuth.binary_tree import BinaryTreeNode, RecursiveBinaryTree

from parameterized import parameterized


class TestBinaryTree(TestCase):
    def _get_test_tree(self, cls: type):
        root = BinaryTreeNode("A", None, None)
        root.left = BinaryTreeNode("B", None, None)
        root.left.left = BinaryTreeNode("D", None, None)
        root.right = BinaryTreeNode("C", None, None)
        root.right.left = BinaryTreeNode("E", None, None)
        root.right.right = BinaryTreeNode("F", None, None)
        root.right.left.right = BinaryTreeNode("G", None, None)
        root.right.right.left = BinaryTreeNode("H", None, None)
        root.right.right.right = BinaryTreeNode("J", None, None)
        return cls(root)
    
    @parameterized.expand([
        RecursiveBinaryTree,
    ])
    def test_preorder_traversal(self, tree_type: type):
        expected =  ['A', 'B', 'D', 'C', 'E', 'G', 'F', 'H', 'J']        
        order = []
        tree = self._get_test_tree(tree_type)
        
        tree.preorder(lambda node: order.append(node.value))
        
        self.assertEqual(order, expected)

    @parameterized.expand([
        RecursiveBinaryTree,
        # StackBinaryTree,
    ])
    def test_inorder_traversal(self, tree_type: type):
        expected =  ['D', 'B', 'A', 'E', 'G', 'C', 'H', 'F', 'J']        
        order = []
        tree = self._get_test_tree(tree_type)
        
        tree.inorder(lambda node: order.append(node.value))
        
        self.assertEqual(order, expected)

    @parameterized.expand([
        RecursiveBinaryTree,
    ])
    def test_postorder_traversal(self, tree_type: type):
        expected =  ['D', 'B', 'G', 'E', 'H', 'J', 'F', 'C', 'A']        
        order = []
        tree = self._get_test_tree(tree_type)
        
        tree.postorder(lambda node: order.append(node.value))
        
        self.assertEqual(order, expected)
    
    def test_noncallable_visit(self):
        tree = RecursiveBinaryTree(None)
        
        with self.assertRaises(TypeError):
            tree.preorder(1)  # type: ignore
        with self.assertRaises(TypeError):
            tree.inorder(1)  # type: ignore
        with self.assertRaises(TypeError):
            tree.postorder(1)  # type: ignore
