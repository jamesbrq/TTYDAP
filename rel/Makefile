#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>/devkitPPC")
endif

ifeq ($(shell which elf2rel),)
$(error "Please install pyelf2rel. pip install pyelf2rel")
endif

include $(DEVKITPPC)/gamecube_rules

# This will be the resulting .rel file - No spaces
OUTPUT_FILENAME := mod

# DON'T TOUCH UNLESS YOU KNOW WHAT YOU'RE DOING
export ALL_SHARED := all_source

export ROOT_DIR ?= $(CURDIR)
export JSON_READ := python $(realpath ../../bin/json_read.py)
export ELF2REL := elf2rel

MAPPER := python $(realpath ../../bin/relmapper.py)

ifeq ($(VERSION),)
all: us jp eu
us:
	@$(MAKE) --no-print-directory VERSION=us
jp:
	@$(MAKE) --no-print-directory VERSION=jp
eu:
	@$(MAKE) --no-print-directory VERSION=eu

clean:
	@$(MAKE) --no-print-directory clean_target VERSION=us
	@$(MAKE) --no-print-directory clean_target VERSION=jp
	@$(MAKE) --no-print-directory clean_target VERSION=eu

.PHONY: all clean us jp eu
else

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
TARGET		:=	$(OUTPUT_FILENAME).$(VERSION)
BUILD		:=	build.$(VERSION)
SOURCES		:=	source $(wildcard source/*) $(ALL_SHARED) $(wildcard $(ALL_SHARED)/*)
DATA		:=	data
INCLUDES	:=	$(CURDIR)/include

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------

MACHDEP		= -mno-sdata -mgcn -DGEKKO -mcpu=750 -meabi -mhard-float

CFLAGS		= -nostdlib -ffreestanding -ffunction-sections -fdata-sections -g -Oz -fvisibility=hidden -Wall -Wextra -Wno-uninitialized $(MACHDEP) $(INCLUDE)
CXXFLAGS	= -fno-exceptions -fno-rtti -std=gnu++23 $(CFLAGS)

LDFLAGS		= -r -e _prolog -u _prolog -u _epilog -u _unresolved -Wl,--gc-sections,--gc-keep-exported -nostdlib -g $(MACHDEP) -Wl,-Map,$(notdir $@).map

# Platform options
ifeq ($(VERSION),us)
	CFLAGS += -DTTYD_US
	ASFLAGS += -DTTYD_US
	GAMECODE = "G8ME"
else ifeq ($(VERSION),jp)
	CFLAGS += -DTTYD_JP
	ASFLAGS += -DTTYD_JP
	GAMECODE = "G8MJ"
else ifeq ($(VERSION),eu)
	CFLAGS += -DTTYD_EU
	ASFLAGS += -DTTYD_EU
	GAMECODE = "G8MP"
endif


#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
# LIBS	:= -lgcc
LIBS	:= 

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(TARGET)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
			$(foreach dir,$(DATA),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

#---------------------------------------------------------------------------------
# automatically build a list of object files for our project
#---------------------------------------------------------------------------------
CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
sFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.S)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
	export LD	:=	$(CC)
else
	export LD	:=	$(CXX)
endif

export OFILES_BIN	:=	$(addsuffix .o,$(BINFILES))
export OFILES_SOURCES := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(sFILES:.s=.o) $(SFILES:.S=.o)
export OFILES := $(OFILES_BIN) $(OFILES_SOURCES)

export HFILES := $(addsuffix .h,$(subst .,_,$(BINFILES)))

# For REL linking
export LDFILES		:= $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.ld)))
export MAPFILE		:= $(ROOT_DIR)/$(BUILD)/temp.$(VERSION).lst
export MODIDS		:= $(abspath ../bin/modids.json)
export BASE_MAPFILE	:= $(realpath include/ttyd.$(VERSION).lst)

export ROOT_BUILD	:= $(abspath $(BUILD))
export SUBREL		:= $(notdir $(wildcard subrels/*))
export SUBREL_CLEAN	:= $(addsuffix /clean,$(SUBREL))
export SUBRELS		:= $(foreach dir,$(SUBREL),$(ROOT_DIR)/subrels/$(dir)/$(dir).$(VERSION).rel)

#---------------------------------------------------------------------------------
# build a list of include paths
#---------------------------------------------------------------------------------
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			-I$(ROOT_DIR)/$(BUILD) \
			-I$(LIBOGC_INC)

#---------------------------------------------------------------------------------
# build a list of library paths
#---------------------------------------------------------------------------------
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib) \
			-L$(LIBOGC_LIB)

export OUTPUT	:=	$(CURDIR)/$(TARGET)
.PHONY: $(BUILD) clean_target

#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@mkdir -p $@/rels
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean_target: $(SUBREL_CLEAN)
	@echo clean ... $(OUTPUT_FILENAME) $(VERSION)
	@rm -fr $(BUILD) $(OUTPUT).elf $(OUTPUT).dol $(OUTPUT).rel

$(SUBREL_CLEAN):
	@$(MAKE) -C $(abspath subrels/$(patsubst %/,%,$(dir $@))) clean_target

#---------------------------------------------------------------------------------
else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT).rel: $(OUTPUT).elf $(MAPFILE) $(SUBREL)
$(OUTPUT).elf: $(LDFILES) $(OFILES)

$(OFILES_SOURCES) : $(HFILES)

$(MAPFILE): $(OUTPUT).elf $(BASE_MAPFILE) $(MODIDS)
	@echo module mapping ... $(notdir $@)
	@$(MAPPER) -o $(MAPFILE) -i $(BASE_MAPFILE) -m $(MODIDS) $(OUTPUT).elf

$(SUBREL): $(MAPFILE)
	@echo making $@ ...
	@$(MAKE) -C $(abspath $(ROOT_DIR)/subrels/$@)
	@echo $@ ... Done

# REL linking
%.rel: %.elf
	@echo output ... $(notdir $@)
	@$(ELF2REL) $< $(abspath $(MAPFILE)) $@ --rel-id $(shell $(JSON_READ) $(MODIDS) "$(OUTPUT_FILENAME)")
	@echo renaming $(notdir $@) to $(OUTPUT_FILENAME).rel
	@cp $@ $(ROOT_BUILD)/rels/$(OUTPUT_FILENAME).rel
	
#---------------------------------------------------------------------------------
# This rule links in binary data with the .jpg extension
#---------------------------------------------------------------------------------
%.jpg.o	%_jpg.h :	%.jpg
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	$(bin2o)

-include $(DEPENDS)

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------
endif