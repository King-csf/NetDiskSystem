/****************************************************************************
** Meta object code from reading C++ file 'folderdownloader.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../folderdownloader.h"
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'folderdownloader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN16FolderDownloaderE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN16FolderDownloaderE = QtMocHelpers::stringData(
    "FolderDownloader",
    "downloadStarted",
    "",
    "directoryScanned",
    "totalFiles",
    "totalSize",
    "fileDownloadStarted",
    "filename",
    "fileDownloadProgress",
    "downloaded",
    "total",
    "fileDownloadCompleted",
    "downloadProgress",
    "completedFiles",
    "downloadedSize",
    "downloadCompleted",
    "downloadPaused",
    "downloadResumed",
    "downloadCancelled",
    "downloadError",
    "error",
    "onDirectoryListReceived",
    "onFileDownloadCompleted",
    "chunkId",
    "chunkSize",
    "haveWrite",
    "onFileDownloadFinished",
    "updateProgress"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN16FolderDownloaderE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      11,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  104,    2, 0x06,    1 /* Public */,
       3,    2,  105,    2, 0x06,    2 /* Public */,
       6,    1,  110,    2, 0x06,    5 /* Public */,
       8,    3,  113,    2, 0x06,    7 /* Public */,
      11,    1,  120,    2, 0x06,   11 /* Public */,
      12,    4,  123,    2, 0x06,   13 /* Public */,
      15,    0,  132,    2, 0x06,   18 /* Public */,
      16,    0,  133,    2, 0x06,   19 /* Public */,
      17,    0,  134,    2, 0x06,   20 /* Public */,
      18,    0,  135,    2, 0x06,   21 /* Public */,
      19,    1,  136,    2, 0x06,   22 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      21,    0,  139,    2, 0x08,   24 /* Private */,
      22,    4,  140,    2, 0x08,   25 /* Private */,
      26,    0,  149,    2, 0x08,   30 /* Private */,
      27,    0,  150,    2, 0x08,   31 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::LongLong,    4,    5,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::LongLong, QMetaType::LongLong,    7,    9,   10,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::LongLong, QMetaType::LongLong,   13,    4,   14,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   20,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::LongLong, QMetaType::LongLong,    7,   23,   24,   25,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject FolderDownloader::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ZN16FolderDownloaderE.offsetsAndSizes,
    qt_meta_data_ZN16FolderDownloaderE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN16FolderDownloaderE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<FolderDownloader, std::true_type>,
        // method 'downloadStarted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'directoryScanned'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        // method 'fileDownloadStarted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'fileDownloadProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        // method 'fileDownloadCompleted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'downloadProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        // method 'downloadCompleted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'downloadPaused'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'downloadResumed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'downloadCancelled'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'downloadError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onDirectoryListReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onFileDownloadCompleted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        // method 'onFileDownloadFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void FolderDownloader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<FolderDownloader *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->downloadStarted(); break;
        case 1: _t->directoryScanned((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<qint64>>(_a[2]))); break;
        case 2: _t->fileDownloadStarted((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->fileDownloadProgress((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<qint64>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<qint64>>(_a[3]))); break;
        case 4: _t->fileDownloadCompleted((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->downloadProgress((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<qint64>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<qint64>>(_a[4]))); break;
        case 6: _t->downloadCompleted(); break;
        case 7: _t->downloadPaused(); break;
        case 8: _t->downloadResumed(); break;
        case 9: _t->downloadCancelled(); break;
        case 10: _t->downloadError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 11: _t->onDirectoryListReceived(); break;
        case 12: _t->onFileDownloadCompleted((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<qint64>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<qint64>>(_a[4]))); break;
        case 13: _t->onFileDownloadFinished(); break;
        case 14: _t->updateProgress(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (FolderDownloader::*)();
            if (_q_method_type _q_method = &FolderDownloader::downloadStarted; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _q_method_type = void (FolderDownloader::*)(int , qint64 );
            if (_q_method_type _q_method = &FolderDownloader::directoryScanned; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _q_method_type = void (FolderDownloader::*)(const QString & );
            if (_q_method_type _q_method = &FolderDownloader::fileDownloadStarted; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _q_method_type = void (FolderDownloader::*)(const QString & , qint64 , qint64 );
            if (_q_method_type _q_method = &FolderDownloader::fileDownloadProgress; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _q_method_type = void (FolderDownloader::*)(const QString & );
            if (_q_method_type _q_method = &FolderDownloader::fileDownloadCompleted; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _q_method_type = void (FolderDownloader::*)(int , int , qint64 , qint64 );
            if (_q_method_type _q_method = &FolderDownloader::downloadProgress; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _q_method_type = void (FolderDownloader::*)();
            if (_q_method_type _q_method = &FolderDownloader::downloadCompleted; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _q_method_type = void (FolderDownloader::*)();
            if (_q_method_type _q_method = &FolderDownloader::downloadPaused; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
        {
            using _q_method_type = void (FolderDownloader::*)();
            if (_q_method_type _q_method = &FolderDownloader::downloadResumed; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 8;
                return;
            }
        }
        {
            using _q_method_type = void (FolderDownloader::*)();
            if (_q_method_type _q_method = &FolderDownloader::downloadCancelled; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 9;
                return;
            }
        }
        {
            using _q_method_type = void (FolderDownloader::*)(const QString & );
            if (_q_method_type _q_method = &FolderDownloader::downloadError; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 10;
                return;
            }
        }
    }
}

const QMetaObject *FolderDownloader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FolderDownloader::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN16FolderDownloaderE.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int FolderDownloader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void FolderDownloader::downloadStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void FolderDownloader::directoryScanned(int _t1, qint64 _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void FolderDownloader::fileDownloadStarted(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void FolderDownloader::fileDownloadProgress(const QString & _t1, qint64 _t2, qint64 _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void FolderDownloader::fileDownloadCompleted(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void FolderDownloader::downloadProgress(int _t1, int _t2, qint64 _t3, qint64 _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void FolderDownloader::downloadCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void FolderDownloader::downloadPaused()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void FolderDownloader::downloadResumed()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void FolderDownloader::downloadCancelled()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void FolderDownloader::downloadError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}
QT_WARNING_POP
