/****************************************************************************
** Meta object code from reading C++ file 'WorldEditor.h'
**
** Created: Wed Nov 26 21:30:10 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../WorldEditor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WorldEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_WorldEditor[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_WorldEditor[] = {
    "WorldEditor\0\0rotateOneStep()\0"
};

const QMetaObject WorldEditor::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_WorldEditor,
      qt_meta_data_WorldEditor, 0 }
};

const QMetaObject *WorldEditor::metaObject() const
{
    return &staticMetaObject;
}

void *WorldEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WorldEditor))
	return static_cast<void*>(const_cast< WorldEditor*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int WorldEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: rotateOneStep(); break;
        }
        _id -= 1;
    }
    return _id;
}
