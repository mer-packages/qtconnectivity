SOURCES += tst_qbluetoothlocaldevice.cpp
TARGET=tst_qbluetoothlocaldevice
CONFIG += testcase

QT = core concurrent bluetooth testlib

CONFIG += insignificant_test    # QTBUG-22017
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
