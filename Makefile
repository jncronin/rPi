RM := rm -rf

TARGET := crawler

LIBS := 

ifdef __BARE_METAL__
PREFIX := arm-none-eabi-
endif

CPP := $(PREFIX)g++

CPPOPTS := -std=c++0x -O0 -g3 -Wall -fmessage-length=0

AR := ar # arm-none-eabi-ar

CPP_SRCS += $(shell ls *.cpp) 

DIRS := bsp bare_metal 

OBJS += $(patsubst %.cpp, %.o, $(CPP_SRCS))

# All Target
all: $(TARGET)

# Tool invocations
$(TARGET): $(DIRS) $(OBJS) 
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC'
	$(CPP)  -o "rPi_devices_test" $(OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

%.o: %.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	$(CPP) $(CPPOPTS)  -o "$@" -c "$<"
	@echo 'Finished building: $<'
	@echo ' '

# Other Targets
clean:
	-$(RM) *.o $(TARGET)
	-@echo ' '

bare_metal:
	cd "$@"; make

bsp:
	cd "$@"; make

.PHONY: all clean dependents
