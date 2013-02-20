include Makefile.rules

TARGET := crawler

LIBS := -Lbsp -lbsp

INCLUDES = -Ibare_metal -Ibsp -Iexternal_devices

DIRS := bsp 

ifdef __BARE_METAL__
DIRS += bare_metal 
endif


OBJS += $(patsubst %.cpp, %.o, $(shell ls *.cpp))
#OBJS += $(patsubst %.c, %.o, $(shell ls *.c))
#OBJS += $(patsubst %.S, %.o, $(shell ls *.S))


# All Target
all: $(TARGET)

# Tool invocations
$(TARGET): DI $(OBJS) 
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC'
	$(LD)  -o $(TARGET) $(OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	for d in $(DIRS); do \
	cd $$d; make clean; \
	done
	-$(RM) *.o $(TARGET)
	-@echo ' '

DI:
	for d in $(DIRS); do \
	cd $$d; make CFLAGS=$(CFLAGS) CPPFLAGS=$(CPPFLAGS); \
	done

.PHONY: all clean dependents
