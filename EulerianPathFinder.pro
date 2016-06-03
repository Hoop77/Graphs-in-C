QMAKE_CXX = gcc

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    comparator.c \
    dlist.c \
    dlistiterator.c \
    dlistnode.c \
    edge.c \
    graph.c \
    main.c \
    path.c \
    pathelement.c \
    vertex.c

HEADERS += \
    dlistnode.h \
    basic.h \
    dlist.h \
    dlistiterator.h \
    comparator.h \
    edge.h \
    vertex.h \
    graph.h \
    path.h \
    pathelement.h

