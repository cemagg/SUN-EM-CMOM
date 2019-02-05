TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    ../file_io/mom_file_reader.cpp \
    ../mom_by_face/vrhs.cpp \
    fill_vrhs.cpp \
    ../mom_by_face/zmn_by_face.cpp \
    ../mom_by_face/quadrature.cpp \
    fill_zmn.cpp

HEADERS += \
    ../data_structures/node.h \
    ../data_structures/edge.h \
    ../data_structures/triangle.h \
    ../file_io/mom_file_reader.h \
    ../mom_by_face/vrhs.h \
    fill_vrhs.h \
    ../mom_by_face/zmn_by_face.h \
    ../mom_by_face/quadrature.h \
    fill_zmn.h
