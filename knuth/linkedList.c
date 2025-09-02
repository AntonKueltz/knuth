#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stddef.h>


/** SinglyLinkedListNode type **/
typedef struct {
    PyObject_HEAD
    PyObject * value;
    PyObject * next;
} SinglyLinkedListNodeObject;

static void SinglyLinkedListNode_dealloc(SinglyLinkedListNodeObject * self) {
    Py_XDECREF(self->value);
    Py_XDECREF(self->next);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static int SinglyLinkedListNode_init(SinglyLinkedListNodeObject * self, PyObject * args) {
    PyObject * value = NULL, * next = NULL;

    if (!PyArg_ParseTuple(args, "OO", &value, &next)) {
        return -1;
    }

    if (value) {
        Py_XSETREF(self->value, Py_NewRef(value));
    }
    if (next) {
        Py_XSETREF(self->next, Py_NewRef(next));
    }

    return 0;
}

static PyMemberDef SinglyLinkedListNode_members[] = {
    {"value", Py_T_OBJECT_EX, offsetof(SinglyLinkedListNodeObject, value), 0, "value stored in the node"},
    {"next", Py_T_OBJECT_EX, offsetof(SinglyLinkedListNodeObject, next), 0, "the next node in the linked list"},
    {NULL}
};

static PyTypeObject SinglyLinkedListNodeType = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "knuth.linked_list.SinglyLinkedListNode",
    .tp_doc = PyDoc_STR("A node in a singly linked list that can store arbitrary data."),
    .tp_basicsize = sizeof(SinglyLinkedListNodeObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc)SinglyLinkedListNode_init,
    .tp_dealloc = (destructor) SinglyLinkedListNode_dealloc,
    .tp_members = SinglyLinkedListNode_members,
};


/** SinglyLinkedList type **/
typedef struct {
    PyObject_HEAD
    PyObject * head;
    long size;
} SinglyLinkedListObject;

static void SinglyLinkedList_dealloc(SinglyLinkedListObject * self) {
    Py_XDECREF(self->head);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static int SinglyLinkedList_init(SinglyLinkedListObject * self, PyObject * args) {
    PyObject * head = NULL;

    if (!PyArg_ParseTuple(args, "O", &head)) {
        return -1;
    }

    if (head) {
        Py_XSETREF(self->head, Py_NewRef(head));
    }

    self->size = (head == Py_None) ? 0 : 1;

    return 0;
}

static PyMemberDef SinglyLinkedList_members[] = {
    {"head", Py_T_OBJECT_EX, offsetof(SinglyLinkedListObject, head), 0, "the head of the list"},
    {"size", Py_T_LONGLONG, offsetof(SinglyLinkedListObject, size), 0, "the number of entries in the list"},
    {NULL}
};

static PyObject * SinglyLinkedList_insert(SinglyLinkedListObject * self, PyObject * value) {
    PyObject * next = self->head ? self->head : Py_None;
    PyObject * node = PyObject_CallFunction((PyObject *)&SinglyLinkedListNodeType, "OO", value, next);

    Py_XSETREF(self->head, node);
    self->size++;

    Py_RETURN_NONE;
}

static PyObject * SinglyLinkedList_pop(SinglyLinkedListObject * self) {
    if (self->size <= 0) {
        PyErr_SetString(PyExc_IndexError, "cannot pop an empty list");
        return NULL;
    }

    SinglyLinkedListNodeObject * head = (SinglyLinkedListNodeObject *)self->head;
    PyObject * result = head->value;
    Py_INCREF(result);

    PyObject * next = (PyObject *)head->next;
    Py_XINCREF(next);
    Py_XSETREF(self->head, next);
    self->size--;

    return result;
}

static PyMethodDef SinglyLinkedList_methods[] = {
    {"insert", (PyCFunction)SinglyLinkedList_insert, METH_O, "Insert a node at the head of the list."},
    {"pop", (PyCFunction)SinglyLinkedList_pop, METH_NOARGS, "Remove the head of the list and return the value stored in it."},
    {NULL}
};

static PyTypeObject SinglyLinkedListType = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "knuth.linked_list.SinglyLinkedList",
    .tp_doc = PyDoc_STR("A linked list whose nodes have a single link to the next node in the list."),
    .tp_basicsize = sizeof(SinglyLinkedListObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc)SinglyLinkedList_init,
    .tp_dealloc = (destructor) SinglyLinkedList_dealloc,
    .tp_members = SinglyLinkedList_members,
    .tp_methods = SinglyLinkedList_methods,
};


/** module configuration and initialization **/
static int linked_list_module_exec(PyObject * m) {
    if (PyType_Ready(&SinglyLinkedListNodeType) < 0) {
        return -1;
    }
    if (PyModule_AddObjectRef(m, "SinglyLinkedListNode", (PyObject *) &SinglyLinkedListNodeType) < 0) {
        return -1;
    }

    if (PyType_Ready(&SinglyLinkedListType) < 0) {
        return -1;
    }
    if (PyModule_AddObjectRef(m, "SinglyLinkedList", (PyObject *) &SinglyLinkedListType) < 0) {
        return -1;
    }

    return 0;
}

static PyModuleDef_Slot linked_list_module_slots[] = {
    {Py_mod_exec, linked_list_module_exec},
    {Py_mod_multiple_interpreters, Py_MOD_MULTIPLE_INTERPRETERS_NOT_SUPPORTED},
    {0, NULL}
};

static struct PyModuleDef linked_list_module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "linked_list",
    .m_doc = "Various implementations of the linked list abstract data type (ADT).",
    .m_size = 0,
    .m_slots = linked_list_module_slots,
};

PyMODINIT_FUNC PyInit_linked_list(void) {
    return PyModuleDef_Init(&linked_list_module);
}