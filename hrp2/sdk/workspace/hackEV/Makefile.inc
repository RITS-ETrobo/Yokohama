mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

# Add objects that are generated from *.c
APPL_COBJS += 

# Add objects that are generated from *.cpp
# オブジェクトは、以下の順に追加する
# - 定義
# - Utilities
# - Sensors
# - Others
APPL_CXXOBJS += \
    logSettings.o \
    portSettings.o \
    parameterPID.o \
    scenarioRunning.o \
    wheelSettings.o \
    lcdSettings.o \
    utilities.o \
    Logger.o \
    SonarSensorController.o \
    gyroboy.o \
    ColorSensorController.o \
    LCDController.o \
    pid_controller.o \
    RunningModule.o \
    ArmModule.o

SRCLANG := c++

ifdef CONFIG_EV3RT_APPLICATION

# Include libraries
include $(EV3RT_SDK_LIB_DIR)/libcpp-ev3/Makefile

endif

APPL_DIR += \
    $(mkfile_path)sensors \
    $(mkfile_path)params \
    $(mkfile_path)params/machine

INCLUDES += \
    -I$(mkfile_path)sensors \
    -I$(mkfile_path)params \
    -I$(mkfile_path)params/machine
