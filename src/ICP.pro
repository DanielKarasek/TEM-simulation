android|ios|tvos|winrt {
    warning( "This example is not supported for android, ios, tvos, or winrt." )
}

QT += 3dcore 3drender 3dinput 3dextras
QT += widgets

CONFIG += c++17

SOURCES += main.cpp \
    detector.cpp \
    lens.cpp \
    microscope.cpp \
    microscopeobject.cpp \
    presenter.cpp \
    ray.cpp \
    scene.cpp \
    myqslider.cpp\
    specimen.cpp \
    specmat.cpp

HEADERS += \
    detector.h \
    lens.h \
    microscope.h \
    microscopeobject.h \
    mytexture.h \
    presenter.h \
    ray.h \
    scene.h \
    myqslider.h\
    specimen.h \
    specmat.h \
    typedefs.h

DISTFILES +=

FORMS +=
