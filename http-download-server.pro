TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    config.c \
    setup_tcp.c \
    argument.c \
    thread_handle.c \
    accept_list.c \
    conn_manage.c \
    feed_conn.c \
    http_head.c
INCLUDEPATH += /usr/include/glib-2.0 \
 /usr/lib/x86_64-linux-gnu/glib-2.0/include
LIBS = -lglib-2.0 -lpthread

HEADERS += \
    config.h \
    setup_tcp.h \
    thread_handle.h \
    accept_list.h \
    conn_manage.h \
    feed_conn.h \
    http_head.h \
    error_reply.h
