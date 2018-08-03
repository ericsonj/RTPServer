# Project
TARGET ?= udpEcho
VERBOSE=n

# Declaration of variables
CC=gcc
CXX=g++
ASS=gcc
AS=as
SIZE=size

CFLAGS=-g -MMD -MP
CXXFLAGS=-g -MMD -MP
ASFLAGS=-g -MMD -MP

LDFLAGS=

BUILD_DIR ?= ./build
SRC_DIRS ?= .

CTT=$(CXX)

# Generic Makefile

SRCS := $(shell find $(SRC_DIRS) -regextype sed -regex ".*\.\(c\|cpp\|s\|S\)")
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
INC_DIRS := $(shell find $(SRC_DIRS) -regextype sed -regex ".*\.\(hh\|h\)" -exec dirname {} \; | uniq )
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS)

ifeq ($(VERBOSE),y)
Q=
else
Q=@
endif

size: $(BUILD_DIR)/$(TARGET)	
	@echo "SIZE\t" $(TARGET)
	@$(SIZE) $<

$(BUILD_DIR)/$(TARGET): $(OBJS) 
	$(Q)$(CTT) $(OBJS) -o $@ $(LDFLAGS)
	@echo $(CTT)"\t" $@

# assembly
$(BUILD_DIR)/%.s.o: %.s
	@$(MKDIR_P) $(dir $@)
	$(Q)$(AS) $(ASFLAGS) -c $< -o $@
	@echo "AS\t" $<

$(BUILD_DIR)/%.S.o: %.S
	@$(MKDIR_P) $(dir $@)
	$(Q)$(ASS) $(ASFLAGS) -c $< -o $@
	@echo "AS\t" $<

# c source
$(BUILD_DIR)/%.c.o: %.c
	@$(MKDIR_P) $(dir $@)
	$(Q)$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@ 
	@echo "CC\t" $<

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	@$(MKDIR_P) $(dir $@)
	$(Q)$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
	@echo "CXX\t" $<

.PHONY: clean

clean:
	$(Q)rm -r $(BUILD_DIR)
	@echo RM $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
