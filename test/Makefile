ifeq ($(OS),Windows_NT)
  ifeq ($(shell uname -s),) # not in a bash-like shell
	CLEANUP = del /F /Q
	MKDIR = mkdir
  else # in a bash-like shell, like msys
	CLEANUP = rm -f
	MKDIR = mkdir -p
  endif
	TARGET_EXTENSION=exe
else
	CLEANUP = rm -f
	MKDIR = mkdir -p
	TARGET_EXTENSION=out
endif


.PHONY: clean
.PHONY: test
.PHONY: check_errors
.PHONY: test_ci

PATHU = ../tools/unity/src
#PATH_APP = ../src/app
#PATH_BSP = ../src/bsp
#PATH_COMMON = ../src/common
#PATH_DRIVERS = ../src/drivers
PATHS = ../src/common#$(PATH_COMMON) #$(PATH_APP) $(PATH_BSP) $(PATH_DRIVERS)
PATHT = .
PATHB = ../build/bin
#PATHD = ../build/depends
PATHO = ../build/obj_test
PATHR = ../build/results

BUILD_PATHS = $(PATHB) $(PATHD) $(PATHO) $(PATHR)

SRCT = $(wildcard $(PATHT)/*.c)

COMPILE= gcc -c
LINK= gcc
DEPEND= gcc -MM -MG -MF
CFLAGS= -I. -I $(PATHU) -I $(PATHS) -DTEST

RESULTS = $(patsubst $(PATHT)/test_%.c,$(PATHR)/test_%.txt,$(SRCT) )

PASSED = `grep -s PASS $(PATHR)/*.txt`
FAIL = `grep -s FAIL $(PATHR)/*.txt`
IGNORE = `grep -s IGNORE $(PATHR)/*.txt`

test: $(BUILD_PATHS) $(RESULTS)	
	@echo "-----------------------PASSED:-----------------------"
	@echo "$(PASSED)"
	@echo "-----------------------FAILURES:-----------------------"
	@echo "$(FAIL)"
	@echo "-----------------------IGNORES:-----------------------"
	@echo "$(IGNORE)"
	@echo "DONE"
	make -C . check_errors

$(PATHR)/%.txt: $(PATHB)/%.$(TARGET_EXTENSION)
	-./$< > $@ 2>&1

$(PATHB)/test_%.$(TARGET_EXTENSION): $(PATHO)/test_%.o $(PATHO)/%.o $(PATHO)/unity.o #$(PATHD)/test_%.d
	$(LINK) -o $@ $^

$(PATHO)/%.o:: $(PATHT)/%.c
	$(COMPILE) $(CFLAGS) $< -o $@

$(PATHO)/%.o:: $(PATHS)/%.c
	$(COMPILE) $(CFLAGS) $< -o $@

$(PATHO)/%.o:: $(PATHU)/%.c $(PATHU)/%.h
	$(COMPILE) $(CFLAGS) $< -o $@

$(PATHD)/%.d:: $(PATHT)/%.c
	$(DEPEND) $@ $<

$(PATHB):
	$(MKDIR) $(PATHB)

#$(PATHD):
#	$(MKDIR) $(PATHD)

$(PATHO):
	$(MKDIR) $(PATHO)

$(PATHR):
	$(MKDIR) $(PATHR)

check_errors: 
ifneq ( , $(findstring FAIL, $(shell grep -s FAIL $(PATHR)/*.txt)))
	$(error Fail found)
else
	@echo No fails found
endif

clean:
	$(CLEANUP) $(PATHO)/*.o
	$(CLEANUP) $(PATHB)/*.$(TARGET_EXTENSION)
	$(CLEANUP) $(PATHR)/*.txt

.PRECIOUS: $(PATHB)/test_%.$(TARGET_EXTENSION)
.PRECIOUS: $(PATHD)/%.d
.PRECIOUS: $(PATHO)/%.o
.PRECIOUS: $(PATHR)/%.txt
