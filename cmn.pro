TARGET = cmn

CONFIG += console
CONFIG -= app_bundle
CONFIG += warn_on c++11

TEMPLATE = app

SOURCES += \
    main.cpp \
    variant.cpp

HEADERS += \
    property_set.hpp \
    type_id.hpp \
    variant.hpp
