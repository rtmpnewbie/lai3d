/****************************************************************************
** Meta object code from reading C++ file 'QD3DWidget.h'
**
** Created: Wed Nov 26 21:38:05 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../QD3DWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QD3DWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_QD3DWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QD3DWidget[] = {
    "QD3DWidget\0\0updateD3D()\0"
};

const QMetaObject QD3DWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QD3DWidget,
      qt_meta_data_QD3DWidget, 0 }
};

const QMetaObject *QD3DWidget::metaObject() const
{
    return &staticMetaObject;
}

void *QD3DWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QD3DWidget))
	return static_cast<void*>(const_cast< QD3DWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int QD3DWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: updateD3D(); break;
        }
        _id -= 1;
    }
    return _id;
}
