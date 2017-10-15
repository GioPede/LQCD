TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11

SOURCES += \
	Math/su3.cpp \
    GaugeFieldFactory/gaugefieldfactory.cpp \
    Math/random.cpp \
    Math/complex.cpp \
    Action/action.cpp \
    Action/puregauge.cpp \
    Observables/observable.cpp \
    InputOutput/inputparser.cpp \
    InputOutput/outputconf.cpp \
    InputOutput/outputobs.cpp \
	Utils/point.cpp\
	Utils/lattice.cpp \
    Observables/plaquette.cpp \
    ParallelTools/parallel.cpp \
    pgfg.cpp \
    gfe.cpp \
    gfr.cpp \
    InputOutput/inputconf.cpp \
    GaugeFieldReader/gaugefieldreader.cpp

HEADERS += Math/su3.h \
    GaugeFieldFactory/gaugefieldfactory.h \
    Math/random.h \
    Math/complex.h \
    Action/action.h \
    Action/puregauge.h \
    Observables/observable.h \
    InputOutput/inputparser.h \
    InputOutput/outputconf.h \
    InputOutput/outputobs.h \
	Utils/point.h\
	Utils/lattice.h \
    Observables/plaquette.h \
    ParallelTools/parallel.h \
    InputOutput/inputconf.h \
    GaugeFieldReader/gaugefieldreader.h \
    Action/actionlist.h \
    clusterspecifier.h

DISTFILES +=

