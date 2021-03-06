mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

# Add objects that are generated from *.c
APPL_COBJS += 

# Add objects that are generated from *.cpp
# オブジェクトは、以下の順に追加する
# - 定義
# 1. common
# 2. 機種
# それぞれ、以下の順に追加する
# - Utilities
# - Sensors
# - Motors
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
    ColorSensorController.o \
    GyroSensorController.o \
    CourseInformation.o \
    CourseInformationLeft.o \
    CourseInformationRight.o \
    MotorWheel.o \
    DriveController.o \
    EV3Position.o \
    SpeedCalculator.o \
    LCDController.o \
    RunningModule.o \
    ArmModule.o \
    coordinateScenario.o \
    user_function.o

SRCLANG := c++

ifdef CONFIG_EV3RT_APPLICATION

# Include libraries
include $(EV3RT_SDK_LIB_DIR)/libcpp-ev3/Makefile

endif

APPL_DIR += \
    $(mkfile_path)../common \
    $(mkfile_path)../common/params \
    $(mkfile_path)sensors \
    $(mkfile_path)motors \
    $(mkfile_path)params \
    $(mkfile_path)params/machine

INCLUDES += \
    -I$(mkfile_path)../common \
    -I$(mkfile_path)../common/params \
    -I$(mkfile_path)sensors \
    -I$(mkfile_path)motors \
    -I$(mkfile_path)params \
    -I$(mkfile_path)params/machine
