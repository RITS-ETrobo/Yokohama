# Add objects that are generated from *.c
APPL_COBJS += 

# Add objects that are generated from *.cpp
APPL_CXXOBJS += \
    LCDController.o \
    pid_controller.o \
    gyroboy.o \
    utilities.o \
    SonarSensorController.o \
    RunningModule.o

SRCLANG := c++

ifdef CONFIG_EV3RT_APPLICATION

# Include libraries
include $(EV3RT_SDK_LIB_DIR)/libcpp-ev3/Makefile

endif

