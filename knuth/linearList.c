#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stddef.h>



/***************************
 * knuth.linear_list.Stack *
 ***************************/
typedef struct {
    PyObject_HEAD
    PyObject ** data;
    Py_ssize_t maxSize;
    Py_ssize_t stackPointer;
} StackObject;

// knuth.linear_list.Stack.__init__
static int Stack_init(StackObject * self, PyObject * args) {
    Py_ssize_t n;
    if (!PyArg_ParseTuple(args, "n", &n)) {
        return -1;
    }

    if (self-> data != NULL) {
        PyMem_Free(self->data);
        self->data = NULL;
    }

    self->data = PyMem_Calloc(n, sizeof(PyObject *));
    if (self->data == NULL) {
        PyErr_NoMemory();
        return -1;
    }

    self->maxSize = n;
    self->stackPointer = -1;
    return 0;
}

static void Stack_dealloc(StackObject * self) {
    if (self->data != NULL) {
        PyMem_Free(self->data);
        self->data = NULL;
    }

    Py_TYPE(self)->tp_free((PyObject *)self);
}

// knuth.linear_list.Stack.insert / knuth.linear_list.Stack.push
static PyObject * Stack_insert(StackObject * self, PyObject * value) {
    if (self->stackPointer + 1 >= self->maxSize) {
        PyErr_SetString(PyExc_IndexError, "Stack overflow - attempted to insert on a full stack");
        return NULL;
    }

    self->stackPointer++;
    Py_XSETREF(self->data[self->stackPointer], value);
    
    Py_RETURN_NONE;
}

// knuth.linear_list.Stack.delete / knuth.linear_list.Stack.pop
static PyObject * Stack_delete(StackObject * self) {
    if (self->stackPointer < 0) {
        PyErr_SetString(PyExc_IndexError, "Stack underflow - attempted to delete on an empty stack");
        return NULL;
    }

    PyObject * result = self->data[self->stackPointer];
    Py_INCREF(result);
    self->stackPointer--;
    
    return result;
}

static PyMethodDef Stack_methods[] = {
    {"insert", (PyCFunction)Stack_insert, METH_O, "Insert a value at the top of the stack"},
    {"push", (PyCFunction)Stack_insert, METH_O, "Insert a value at the top of the stack"},
    {"delete", (PyCFunction)Stack_delete, METH_NOARGS, "Delete a value from the top of the stack and return it"},
    {"pop", (PyCFunction)Stack_delete, METH_NOARGS, "Delete a value from the top of the stack and return it"},
    {NULL}
};

static PyTypeObject StackType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "knuth.linear_list.Stack",
    .tp_basicsize = sizeof(StackObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "An implementation of the stack abstract data type (ADT)",
    .tp_init = (initproc)Stack_init,
    .tp_dealloc = (destructor)Stack_dealloc,
    .tp_new = PyType_GenericNew,
    .tp_methods = Stack_methods,
};



/*********************
 * knuth.linear_list *
 *********************/
static int linear_list_module_exec(PyObject * m) {
    if (PyType_Ready(&StackType) < 0) {
        return -1;
    }
    if (PyModule_AddObjectRef(m, "Stack", (PyObject *) &StackType) < 0) {
        return -1;
    }

    return 0;
}

static PyModuleDef_Slot linear_list_module_slots[] = {
    {Py_mod_exec, linear_list_module_exec},
    {Py_mod_multiple_interpreters, Py_MOD_MULTIPLE_INTERPRETERS_NOT_SUPPORTED},
    {0, NULL}
};

static struct PyModuleDef linear_list_module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "linear_list",
    .m_doc = "Various implementations of the linear list abstract data types (ADT).",
    .m_size = 0,
    .m_slots = linear_list_module_slots,
};

PyMODINIT_FUNC PyInit_linear_list(void) {
    return PyModuleDef_Init(&linear_list_module);
}
