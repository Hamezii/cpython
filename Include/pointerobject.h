/* C pointer object interface */

#ifndef Py_CPOINTEROBJECT_H
#define Py_CPOINTEROBJECT_H
#ifdef __cplusplus
extern "C" {
#endif


PyAPI_DATA(PyTypeObject) _PyInternalPointer_Type;
#define PyInternalPointer_Check(x) (Py_TYPE(x) == &_PyInternalPointer_Type)
/* Function to return a bool from a C long */
PyAPI_FUNC(PyObject *) PyInternalPointer_FromVoidPointer(void *);
PyAPI_FUNC(void *) PyInternalPointer_AsVoidPointer(PyObject *);

#ifdef __cplusplus
}
#endif
#endif /* !Py_CPOINTEROBJECT_H */
