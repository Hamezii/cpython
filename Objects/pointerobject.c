/* C pointer type, a subtype of int */

#include "Python.h"
#include "longintrepr.h"
#include "pointerobject.h"

/* The type object for C pointers.  Note that this cannot be subclassed! */
typedef struct {
        PyObject_HEAD
        /* TODO: store the type of the pointer + whether it's valid? */
        void* pointer;
} PyInternalPointerObject;

/* Methods */
static int
pointer_compare(PyInternalPointerObject *a, PyInternalPointerObject *b)
{
    if (a->pointer == b->pointer)
        return 0;
    return a->pointer < b->pointer ? -1 : 1;
}

static PyObject *
pointer_richcompare(PyObject *self, PyObject *other, int op)
{
    int result;
    /* XXX: allow comparng against integers? */
    if (!PyInternalPointer_Check(self) || !PyInternalPointer_Check(other))
        Py_RETURN_NOTIMPLEMENTED;
    if (self == other)
        result = 0;
    else
        result = pointer_compare((PyInternalPointerObject*)self, (PyInternalPointerObject*)other);
    Py_RETURN_RICHCOMPARE(result, 0, op);
}

static Py_hash_t
pointer_hash(PyObject *v)
{
    assert(PyInternalPointer_Check(v));
    return _Py_HashPointer(((PyInternalPointerObject *)v)->pointer);
}

static int
pointer_bool(PyObject *v)
{
    assert(PyInternalPointer_Check(v));
    return ((PyInternalPointerObject *)v)->pointer != NULL;
}

static PyObject *
pointer_int(PyObject *v)
{
    assert(PyInternalPointer_Check(v));
    PyInternalPointerObject *cp = (PyInternalPointerObject *)v;
    return PyLong_FromPyAddr((Py_addr_t)cp->pointer);
}

static PyObject *
pointer_repr(PyObject *v)
{
    assert(PyInternalPointer_Check(v));
    PyInternalPointerObject *cp = (PyInternalPointerObject *)v;
    /* TODO: print CHERI permissions? */
    return PyUnicode_FromFormat("<C pointer:%p>", cp->pointer);
}


static PyNumberMethods pointer_as_number = {
    .nb_bool = pointer_bool,
    .nb_int = pointer_int,
};

PyObject *
PyInternalPointer_FromVoidPointer(void* value)
{
    PyInternalPointerObject *p;
    p = PyObject_New(PyInternalPointerObject, &_PyInternalPointer_Type);
    if (p == NULL)
        return NULL;
    p->pointer = value;
    return (PyObject *)p;
}

void *
PyInternalPointer_AsVoidPointer(PyObject *vv)
{

    void* pointer = NULL;
    if (PyInternalPointer_Check(vv)) {
        return ((PyInternalPointerObject*)vv)->pointer;
    }
    Py_addr_t addr = PyLong_AsPyAddr(vv);
    if (addr == -1 && PyErr_Occurred())
        return NULL;

    /* Probably not correct since it's not a valid pointer... */
    return (void*)(uintptr_t)pointer;
}

static PyObject *
pointer_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyInternalPointerObject *self = (PyInternalPointerObject *) type->tp_alloc(type, 0);
    if (self != NULL) {
        self->pointer = NULL;
    }
    return (PyObject *) self;
}

static void
pointer_dealloc(PyObject *obj)
{
    // PyInternalPointerObject *self = (PyInternalPointerObject *)obj;
    // fprintf(stderr, "releasing c pointer %p\n", self->pointer);
    Py_TYPE(obj)->tp_free(obj);
}

PyTypeObject _PyInternalPointer_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "_pointer",
    .tp_doc = "Internal pointer representationi0",
    .tp_basicsize = sizeof(PyInternalPointerObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT, /* TODO: Py_TPFLAGS_LONG_SUBCLASS */
    .tp_richcompare = pointer_richcompare,
    .tp_repr = pointer_repr,
    .tp_new = pointer_new,
    .tp_dealloc = pointer_dealloc,
    .tp_hash = pointer_hash,
    .tp_as_number = &pointer_as_number,
    .tp_alloc = PyType_GenericAlloc,
    .tp_free = PyObject_Del,
};
