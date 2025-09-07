#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stddef.h>


/** BinaryTreeNode type **/
typedef struct {
    PyObject_HEAD
    PyObject * value;
    PyObject * left;
    PyObject * right;
} BinaryTreeNodeObject;

static void BinaryTreeNode_dealloc(BinaryTreeNodeObject * self) {
    Py_XDECREF(self->value);
    Py_XDECREF(self->left);
    Py_XDECREF(self->right);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static int BinaryTreeNode_init(BinaryTreeNodeObject * self, PyObject * args) {
    PyObject * value = NULL, * left = NULL, * right = NULL;

    if (!PyArg_ParseTuple(args, "OOO", &value, &left, &right)) {
        return -1;
    }

    if (value) {
        Py_XSETREF(self->value, Py_NewRef(value));
    }
    if (left) {
        Py_XSETREF(self->left, Py_NewRef(left));
    }
    if (right) {
        Py_XSETREF(self->right, Py_NewRef(right));
    }

    return 0;
}

static PyMemberDef BinaryTreeNode_members[] = {
    {"value", Py_T_OBJECT_EX, offsetof(BinaryTreeNodeObject, value), 0, "value stored in the node"},
    {"left", Py_T_OBJECT_EX, offsetof(BinaryTreeNodeObject, left), 0, "the left child of the node"},
    {"right", Py_T_OBJECT_EX, offsetof(BinaryTreeNodeObject, right), 0, "the right child of the node"},
    {NULL}
};

static PyTypeObject BinaryTreeNodeType = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "knuth.binary_tree.BinaryTreeNode",
    .tp_doc = PyDoc_STR("A node in a binary tree that can store arbitrary data."),
    .tp_basicsize = sizeof(BinaryTreeNodeObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc)BinaryTreeNode_init,
    .tp_dealloc = (destructor) BinaryTreeNode_dealloc,
    .tp_members = BinaryTreeNode_members,
};


/** BinaryTree type **/
typedef struct {
    PyObject_HEAD
    PyObject * root;
} BinaryTreeObject;

static void BinaryTree_dealloc(BinaryTreeObject * self) {
    Py_XDECREF(self->root);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static int BinaryTree_init(BinaryTreeObject * self, PyObject * args) {
    PyObject * root = NULL;

    if (!PyArg_ParseTuple(args, "O", &root)) {
        return -1;
    }

    if (root) {
        Py_XSETREF(self->root, Py_NewRef(root));
    }
    return 0;
}

static PyMemberDef BinaryTree_members[] = {
    {"root", Py_T_OBJECT_EX, offsetof(BinaryTreeObject, root), 0, "the root node of the tree"},
    {NULL}
};

static void preorderHelper(PyObject * node, PyObject * visit) {
    if (node == Py_None) {
        return;
    }

    PyObject_CallObject(visit, Py_BuildValue("(O)", node));
    preorderHelper(((BinaryTreeNodeObject *)node)->left, visit);
    preorderHelper(((BinaryTreeNodeObject *)node)->right, visit);
}

static PyObject * BinaryTree_preorder(BinaryTreeObject * self, PyObject * visitFunc) {
    if (!PyCallable_Check(visitFunc)) {
        PyErr_SetString(PyExc_TypeError, "visit paramater must be callable");
        return NULL;
    }

    preorderHelper(self->root, visitFunc);
    Py_RETURN_NONE;
}

static void recursiveInorderHelper(PyObject * node, PyObject * visit) {
    if (node == Py_None) {
        return;
    }

    recursiveInorderHelper(((BinaryTreeNodeObject *)node)->left, visit);
    PyObject_CallObject(visit, Py_BuildValue("(O)", node));
    recursiveInorderHelper(((BinaryTreeNodeObject *)node)->right, visit);
}

static PyObject * BinaryTree_inorder(BinaryTreeObject * self, PyObject * visitFunc) {
    if (!PyCallable_Check(visitFunc)) {
        PyErr_SetString(PyExc_TypeError, "visit paramater must be callable");
        return NULL;
    }

    recursiveInorderHelper(self->root, visitFunc);
    Py_RETURN_NONE;
}

static void postorderHelper(PyObject * node, PyObject * visit) {
    if (node == Py_None) {
        return;
    }

    postorderHelper(((BinaryTreeNodeObject *)node)->left, visit);
    postorderHelper(((BinaryTreeNodeObject *)node)->right, visit);
    PyObject_CallObject(visit, Py_BuildValue("(O)", node));
}

static PyObject * BinaryTree_postorder(BinaryTreeObject * self, PyObject * visitFunc) {
    if (!PyCallable_Check(visitFunc)) {
        PyErr_SetString(PyExc_TypeError, "visit paramater must be callable");
        return NULL;
    }

    postorderHelper(self->root, visitFunc);
    Py_RETURN_NONE;
}

static PyMethodDef BinaryTree_methods[] = {
    {"preorder", (PyCFunction)BinaryTree_preorder, METH_O, "Visit the nodes of the tree pre-order."},
    {"inorder", (PyCFunction)BinaryTree_inorder, METH_O, "Visit the nodes of the tree in-order."},
    {"postorder", (PyCFunction)BinaryTree_postorder, METH_O, "Visit the nodes of the tree post-order."},
    {NULL}
};

static PyTypeObject BinaryTreeType = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "knuth.binary_tree.BinaryTree",
    .tp_doc = PyDoc_STR("A binary tree whose algorithms are mainly implemeted with recursion."),
    .tp_basicsize = sizeof(BinaryTreeObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc)BinaryTree_init,
    .tp_dealloc = (destructor) BinaryTree_dealloc,
    .tp_members = BinaryTree_members,
    .tp_methods = BinaryTree_methods,
};


/** module configuration and initialization **/
static int binary_tree_module_exec(PyObject * m) {
    if (PyType_Ready(&BinaryTreeNodeType) < 0) {
        return -1;
    }
    if (PyModule_AddObjectRef(m, "BinaryTreeNode", (PyObject *) &BinaryTreeNodeType) < 0) {
        return -1;
    }

    if (PyType_Ready(&BinaryTreeType) < 0) {
        return -1;
    }
    if (PyModule_AddObjectRef(m, "BinaryTree", (PyObject *) &BinaryTreeType) < 0) {
        return -1;
    }

    return 0;
}

static PyModuleDef_Slot binary_tree_module_slots[] = {
    {Py_mod_exec, binary_tree_module_exec},
    {Py_mod_multiple_interpreters, Py_MOD_MULTIPLE_INTERPRETERS_NOT_SUPPORTED},
    {0, NULL}
};

static struct PyModuleDef binary_tree_module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "binary_tree",
    .m_doc = "Various implementations of the binary tree abstract data type (ADT).",
    .m_size = 0,
    .m_slots = binary_tree_module_slots,
};

PyMODINIT_FUNC PyInit_binary_tree(void) {
    return PyModuleDef_Init(&binary_tree_module);
}
