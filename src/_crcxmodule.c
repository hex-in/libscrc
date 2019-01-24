/*
*********************************************************************************************************
*                              		(c) Copyright 2006-2017, HZ, Studio
*                                           All Rights Reserved
* File    : _crcxmodule.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V0.1.0
* Web	  : http://heyunhuan513.blog.163.com
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2017-09-22 09:36AM
*
*********************************************************************************************************
*/

#include <Python.h>
#include "_crcxtables.h"

/*
*********************************************************************************************************
*                                   POLY=0x03 [CRC4-ITU]
* Poly:    0x03
* Init:    0x00
* Refin:   True
* Refout:  True
* Xorout:  0x00
*
* 0x0C = reverse 0x03 >> (8-4)
*********************************************************************************************************
*/

static PyObject * _crc4_itu(PyObject *self, PyObject *args)
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00;
    unsigned char crc4    = 0x00;
    unsigned char result  = 0x00;

#if PY_MAJOR_VERSION >= 3
    if (!PyArg_ParseTuple(args, "y#|B", &data, &data_len, &crc4))
        return NULL;
#else
    if (!PyArg_ParseTuple(args, "s#|B", &data, &data_len, &crc4))
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hz_calc_crc4_0c(data, data_len, crc4);
    return Py_BuildValue("B", result);
}


/*
*********************************************************************************************************
*                                   POLY=0x15 [CRC5-ITU]
* Poly:    0x15
* Init:    0x00
* Refin:   True
* Refout:  True
* Xorout:  0x00
*
* 0x15 = reverse 0x15 >> (8-5)
*********************************************************************************************************
*/

static PyObject * _crc5_itu(PyObject *self, PyObject *args)
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00;
    unsigned char crc5    = 0x00;
    unsigned char result  = 0x00;

#if PY_MAJOR_VERSION >= 3
    if (!PyArg_ParseTuple(args, "y#|B", &data, &data_len, &crc5))
        return NULL;
#else
    if (!PyArg_ParseTuple(args, "s#|B", &data, &data_len, &crc5))
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hz_calc_crc5_15(data, data_len, crc5);
    return Py_BuildValue("B", result);
}

/*
*********************************************************************************************************
*                                   POLY=0x09 [CRC5-EPC]
* Poly:    0x09
* Init:    0x09
* Refin:   False
* Refout:  False
* Xorout:  0x00
*
* 0x48 = reverse 0x09 << (8-5)
*********************************************************************************************************
*/

static PyObject * _crc5_epc(PyObject *self, PyObject *args)
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00;
    unsigned char crc5    = 0x48;
    unsigned char result  = 0x00;

#if PY_MAJOR_VERSION >= 3
    if (!PyArg_ParseTuple(args, "y#|B", &data, &data_len, &crc5))
        return NULL;
#else
    if (!PyArg_ParseTuple(args, "s#|B", &data, &data_len, &crc5))
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hz_calc_crc5_48(data, data_len, crc5);
    result = result >> 3;
    return Py_BuildValue("B", result);
}

/*
*********************************************************************************************************
*                                   POLY=0x05 [CRC5-EPC]
* Poly:    0x05
* Init:    0x09
* Refin:   True
* Refout:  True
* Xorout:  0x1F
*
* 0x14 = reverse 0x05 << (8-5)
*********************************************************************************************************
*/

static PyObject * _crc5_usb(PyObject *self, PyObject *args)
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00;
    unsigned char crc5    = 0x1F;
    unsigned char result  = 0x00;

#if PY_MAJOR_VERSION >= 3
    if (!PyArg_ParseTuple(args, "y#|B", &data, &data_len, &crc5))
        return NULL;
#else
    if (!PyArg_ParseTuple(args, "s#|B", &data, &data_len, &crc5))
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hz_calc_crc5_14(data, data_len, crc5);
    result = result ^ 0x1F;
    return Py_BuildValue("B", result);
}

/*
*********************************************************************************************************
*                                   POLY=0x03 [CRC6-ITU]
* Poly:    0x03
* Init:    0x00
* Refin:   True
* Refout:  True
* Xorout:  0x00
*
* 0x30 = reverse 0x03 >> (8-6)
*********************************************************************************************************
*/

static PyObject * _crc6_itu(PyObject *self, PyObject *args)
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00;
    unsigned char crc6    = 0x00;
    unsigned char result  = 0x00;

#if PY_MAJOR_VERSION >= 3
    if (!PyArg_ParseTuple(args, "y#|B", &data, &data_len, &crc6))
        return NULL;
#else
    if (!PyArg_ParseTuple(args, "s#|B", &data, &data_len, &crc6))
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hz_calc_crc6_30(data, data_len, crc6);

    return Py_BuildValue("B", result);
}

/*
*********************************************************************************************************
*                                   POLY=0x09 [CRC7-MMC]
* Poly:    0x09
* Init:    0x00
* Refin:   False
* Refout:  False
* Xorout:  0x00
*
* 0x12 = reverse 0x09 << (8-7)
*********************************************************************************************************
*/

static PyObject * _crc7_mmc(PyObject *self, PyObject *args)
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00;
    unsigned char crc7    = 0x00;
    unsigned char result  = 0x00;

#if PY_MAJOR_VERSION >= 3
    if (!PyArg_ParseTuple(args, "y#|B", &data, &data_len, &crc7))
        return NULL;
#else
    if (!PyArg_ParseTuple(args, "s#|B", &data, &data_len, &crc7))
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hz_calc_crc7_12(data, data_len, crc7);
    result = result >> 1;

    return Py_BuildValue("B", result);
}

/* method table */
static PyMethodDef _crcxMethods[] = {
    {"itu4",    _crc4_itu,          METH_VARARGS, "Calculate ITU  of CRC4 [Poly = 0x03 Initial = 0x00 Xorout=0x00 Refin=True Refout=True]"},
    {"itu5",    _crc5_itu,          METH_VARARGS, "Calculate ITU  of CRC5 [Poly = 0x15 Initial = 0x00 Xorout=0x00 Refin=True Refout=True]"},
    {"epc",     _crc5_epc,          METH_VARARGS, "Calculate EPC  of CRC5 [Poly = 0x09 Initial = 0x09 Xorout=0x00 Refin=False Refout=False]"},
    {"usb5",    _crc5_usb,          METH_VARARGS, "Calculate USB  of CRC5 [Poly = 0x05 Initial = 0x1F Xorout=0x1F Refin=True Refout=True]"},
    {"itu6",    _crc6_itu,          METH_VARARGS, "Calculate ITU  of CRC6 [Poly = 0x03 Initial = 0x00 Xorout=0x00 Refin=True Refout=True]"},
    {"mmc",     _crc7_mmc,          METH_VARARGS, "Calculate MMC  of CRC7 [Poly = 0x09 Initial = 0x00 Xorout=0x00 Refin=False Refout=False]"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};


/* module documentation */
PyDoc_STRVAR(_crcx_doc,
"Calculation of CRCx \n"
"libscrc.itu4   -> Calculate ITU  of CRC4 [Poly = 0x03 Initial = 0x00 Xorout=0x00 Refin=True Refout=True]\n"
"libscrc.itu5   -> Calculate ITU  of CRC5 [Poly = 0x15 Initial = 0x00 Xorout=0x00 Refin=True Refout=True]\n"
"libscrc.epc    -> Calculate EPC  of CRC5 [Poly = 0x09 Initial = 0x09 Xorout=0x00 Refin=False Refout=False]\n"
"libscrc.usb5   -> Calculate USB  of CRC5 [Poly = 0x05 Initial = 0x1F Xorout=0x1F Refin=True Refout=True]\n"
"libscrc.itu6   -> Calculate ITU  of CRC6 [Poly = 0x03 Initial = 0x00 Xorout=0x00 Refin=True Refout=True]\n"
"libscrc.mmc    -> Calculate MMC  of CRC7 [Poly = 0x09 Initial = 0x00 Xorout=0x00 Refin=False Refout=False]\n"
"\n");


#if PY_MAJOR_VERSION >= 3

/* module definition structure */
static struct PyModuleDef _crcxmodule = {
   PyModuleDef_HEAD_INIT,
   "_crcx",                    /* name of module */
   _crcx_doc,                  /* module documentation, may be NULL */
   -1,                         /* size of per-interpreter state of the module */
   _crcxMethods
};

/* initialization function for Python 3 */
PyMODINIT_FUNC
PyInit__crcx(void)
{
    PyObject *m = NULL;

    m = PyModule_Create(&_crcxmodule);
    if (m == NULL) {
        return NULL;
    }

    PyModule_AddStringConstant(m, "__version__", "0.1.0");
    PyModule_AddStringConstant(m, "__author__",  "Heyn");

    return m;
}

#else

/* initialization function for Python 2 */
PyMODINIT_FUNC
init_crcx(void)
{
    (void) Py_InitModule3("_crcx", _crcxMethods, _crcx_doc);
}

#endif /* PY_MAJOR_VERSION */
