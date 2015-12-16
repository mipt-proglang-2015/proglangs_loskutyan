#include <iostream>
#include <vector>

extern "C" {
#include <Python.h>
}

typedef std::vector<double> Row;
typedef std::vector<Row> Matrix;

using namespace std;

static Matrix PyMatrixToCxxMatrix(PyObject* py_matrix) {
    Matrix matrix(PyObject_Length(py_matrix));
    for (size_t i = 0; i < matrix.size(); ++i) {
        PyObject* py_row = PyList_GetItem(py_matrix, i);
        Row& row = matrix[i];
        row.resize(PyObject_Length(py_row));
        for (size_t j = 0; j < row.size(); ++j) {
            PyObject* py_elem = PyList_GetItem(py_row, j);
            row[j] = PyFloat_AsDouble(py_elem);
        }
    }
    return matrix;
}

static PyObject* CxxMatrixToPyMatrix(const Matrix& matrix) {
    PyObject* py_matrix  = PyList_New(matrix.size());
    for (size_t i = 0; i < matrix.size(); ++i) {
        PyObject* py_row = PyList_New(matrix[i].size());
        for (size_t j = 0; j < matrix[i].size(); ++j) {
            PyObject * py_elem = PyFloat_FromDouble(matrix[i][j]);
            PyList_SetItem(py_row, j, py_elem);
        }
        PyList_SetItem(py_matrix, i, py_row);
    }
    return py_matrix;
}

static PyObject* FloydWarshall(PyObject* module, PyObject* args) {
    Matrix matrix = PyMatrixToCxxMatrix(PyTuple_GetItem(args, 0));
    for (size_t k = 0; k < matrix.size(); ++k) {
        for (size_t i = 0; i < matrix.size(); ++i) {
            for (size_t j = 0; j < matrix.size(); ++j) {
                if ((i != j) && (matrix[i][k]+matrix[k][j] != 0)) {
                    double resistance = matrix[i][k] + matrix[k][j];
                    matrix[i][j] = resistance * matrix[i][j] / (resistance + matrix[i][j]);
                }
            }
        }
    }

    return CxxMatrixToPyMatrix(matrix);
}

PyMODINIT_FUNC PyInit_FloydWarshall()
{
    static PyMethodDef ModuleMethods[] = {
            { "calculate", FloydWarshall, METH_VARARGS, NULL },
            { NULL }
    };
    static PyModuleDef ModuleDef = {
            PyModuleDef_HEAD_INIT,
            "FloydWarshall",
            "algorithm",
            -1, ModuleMethods,
            NULL, NULL, NULL, NULL
    };
    PyObject* module = PyModule_Create(&ModuleDef);
    return module;
}
