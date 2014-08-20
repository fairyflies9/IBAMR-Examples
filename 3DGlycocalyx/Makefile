######################################################################
## Here specify the location of the IBAMR source and the location
## where IBAMR has been built.
IBAMR_SRC_DIR = /nas02/apps/ibamr-3803/OFED-1.5.2/IBAMR2
IBAMR_BUILD_DIR = /nas02/apps/ibamr-3803/ibamr-objs-opt

######################################################################
## Include variables specific to the particular IBAMR build.
include $(IBAMR_BUILD_DIR)/config/make.inc

######################################################################
## Build the IB tester application.
SOURCES = main.C update_target_point_positions.C update_target_point_positions.h
OBJS = main.o update_target_point_positions.o

default:
	@echo "make one of: main2d, main3d"

main2d:
	if (test -f stamp-3d); then $(MAKE) clean; fi
	touch stamp-2d
	$(MAKE) PDIM=2 main-2d

main3d:
	if (test -f stamp-2d); then $(MAKE) clean; fi
	touch stamp-3d
	$(MAKE) PDIM=3 main-3d

main-2d: $(IBAMR_LIB_2D) $(IBTK_LIB_2D) $(OBJS) $(SOURCES)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) \
	$(IBAMR_LIB_2D) $(IBTK_LIB_2D) $(LIBS) -DNDIM=$(PDIM) -o main2d

main-3d: $(IBAMR_LIB_3D) $(IBTK_LIB_3D) $(OBJS) $(SOURCES)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) \
	$(IBAMR_LIB_3D) $(IBTK_LIB_3D) $(LIBS) -DNDIM=$(PDIM) -o main3d

clean:
	$(RM) main2d main3d core
	$(RM) *.o *.lo *.objs *.ii *.int.c stamp-[23]d
	$(RM) -r .libs
