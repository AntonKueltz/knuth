from __future__ import annotations


class SinglyLinkedListNode[T]:
    value: T
    next: SinglyLinkedListNode[T] | None

    def __init__(self, value: T, next: SinglyLinkedListNode[T | None]) -> None: ...


class SinglyLinkedList[T]:
    head: SinglyLinkedListNode[T] | None
    size: int

    def __init__(self, head: SinglyLinkedListNode[T] | None) -> None: ...
    def insert(self, value: T) -> None: ...
    def pop(self) -> T: ...
