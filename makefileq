# COMPILER OPTIONS
CXX           = mpic++
CXXFLAGS      = -std=c++11 -O3 -w -fno-stack-protector
INCPATH       = -I.
LINK          = mpic++
LFLAGS        = -m64 -Wl,-O3
LIBS          = $(SUBLIBS)
CPPFLAGS      = -MMD -MP -MF $(@:$(OBJDIR)/%.o=$(DEPDIR)/%.d)

# DIRECTORY STRUCTURE
SRCDIR := src
BLDDIR := build
OBJDIR := $(BLDDIR)/obj
DEPDIR := $(BLDDIR)/dep

# SOURCES FOR GAUGE FIELD EXPLORER
SRCS_GFE  := $(SRCDIR)/gfe.cpp \
	     $(SRCDIR)/Utils/lattice.cpp \
	     $(SRCDIR)/Utils/point.cpp \
	     $(SRCDIR)/Math/su3.cpp \
	     $(SRCDIR)/Math/complex.cpp \
	     $(SRCDIR)/Math/random.cpp \
	     $(SRCDIR)/InputOutput/inputconf.cpp \
	     $(SRCDIR)/ParallelTools/parallel.cpp

OBJS_GFE  := $(SRCS_GFE:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
DEPS_GFE  := $(SRCS_GFE:$(SRCDIR)/%.cpp=$(DEPDIR)/%.d)

# SOURCES FOR GAUGE FIELD GENERATOR
SRCS_PGFG := $(SRCDIR)/pgfg.cpp \
	     $(SRCDIR)/Utils/lattice.cpp \
	     $(SRCDIR)/Utils/point.cpp \
	     $(SRCDIR)/Math/su3.cpp \
	     $(SRCDIR)/Math/complex.cpp \
	     $(SRCDIR)/Math/random.cpp \
	     $(SRCDIR)/InputOutput/inputparser.cpp \
	     $(SRCDIR)/InputOutput/inputconf.cpp \
	     $(SRCDIR)/InputOutput/outputconf.cpp \
	     $(SRCDIR)/InputOutput/outputobs.cpp \
	     $(SRCDIR)/GaugeFieldReader/gaugefieldreader.cpp \
	     $(SRCDIR)/GaugeFieldFactory/gaugefieldfactory.cpp \
	     $(SRCDIR)/Action/action.cpp \
	     $(SRCDIR)/Action/puregauge.cpp \
	     $(SRCDIR)/Observables/observable.cpp \
	     $(SRCDIR)/Observables/plaquette.cpp \
	     $(SRCDIR)/ParallelTools/parallel.cpp

OBJS_PGFG := $(SRCS_PGFG:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
DEPS_PGFG := $(SRCS_PGFG:$(SRCDIR)/%.cpp=$(DEPDIR)/%.d)

# SOURCES FOR GAUGE FIELD READER
SRCS_GFR  := $(SRCDIR)/gfr.cpp \
	     $(SRCDIR)/Utils/lattice.cpp \
	     $(SRCDIR)/Utils/point.cpp \
	     $(SRCDIR)/Math/su3.cpp \
	     $(SRCDIR)/Math/complex.cpp \
	     $(SRCDIR)/Math/random.cpp \
	     $(SRCDIR)/InputOutput/inputparser.cpp \
	     $(SRCDIR)/InputOutput/outputconf.cpp \
	     $(SRCDIR)/InputOutput/inputconf.cpp \
	     $(SRCDIR)/InputOutput/outputobs.cpp \
	     $(SRCDIR)/GaugeFieldReader/gaugefieldreader.cpp \
     	     $(SRCDIR)/GaugeFieldFactory/gaugefieldfactory.cpp \
	     $(SRCDIR)/Action/action.cpp \
	     $(SRCDIR)/Action/puregauge.cpp \
	     $(SRCDIR)/Observables/observable.cpp \
	     $(SRCDIR)/Observables/plaquette.cpp \
	     $(SRCDIR)/ParallelTools/parallel.cpp

OBJS_GFR  := $(SRCS_GFR:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
DEPS_GFR  := $(SRCS_GFR:$(SRCDIR)/%.cpp=$(DEPDIR)/%.d)


# BUILD DIRECTORY TREE
TREE := $(sort $(patsubst %/,%,$(dir -d $(OBJS_GFR) $(OBJS_GFE) $(OBJS_PGFG))))

.PHONY: all clean

all: pgfg gfe gfr

gfe:  $(OBJS_GFE)
	@echo "Making Gauge Field Explorer..."
	@$(CXX) $(LFLAGS) -o $(BLDDIR)/gfe $(OBJS_GFE) $(LIBS)

pgfg: $(OBJS_PGFG)
	@echo "Making Pure Gauge Field Generator..."
	@$(CXX) $(LFLAGS) -o $(BLDDIR)/pgfg $(OBJS_PGFG) $(LIBS)

gfr:  $(OBJS_GFR)
	@echo "Making Pure Gauge Field Generator..."
	@$(CXX) $(LFLAGS) -o $(BLDDIR)/gfr $(OBJS_GFR) $(LIBS)


.SECONDEXPANSION:
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $$(@D)
	@echo "Compiling $@...";
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<

$(TREE): %:
	@mkdir -p $@
	@mkdir -p $(@:$(OBJDIR)%=$(DEPDIR)%)

clean:
	@echo "Cleaning Up..."
	@rm -r $(BLDDIR)
