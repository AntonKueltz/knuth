from __future__ import annotations
from typing import Callable


class BinaryTreeNode[T]:
    value: T
    left: BinaryTreeNode | None
    right: BinaryTreeNode | None
    
    def __init__(self, value: T, left: BinaryTreeNode | None, right: BinaryTreeNode | None) -> None: ...


class RecursiveBinaryTree[T]:
    root: BinaryTreeNode[T] | None
    
    def __init__(self, root: BinaryTreeNode[T] | None) -> None: ...
    def preorder(self, visit: Callable[[BinaryTreeNode], None]) -> None: ...
    def inorder(self, visit: Callable[[BinaryTreeNode], None]) -> None: ...
    def postorder(self, visit: Callable[[BinaryTreeNode], None]) -> None: ...