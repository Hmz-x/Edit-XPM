# set your compiler of choice here. Change the flags below accordingly if you should switch from GCC
CXX=gcc
# set flags for object creation. -o must be last
CXX_FLAGS=-c -Wall -o
# set flags for linking. -o must be last
CXX_LFLAGS=-o

# declare all source files here
SOURCES=lex.yy.c edit_xpm.c
# list all required libraries here
LIBRARIES=

# this automatically creates all the names of the objects required for the linking rule
OBJECTS=$(SOURCES:.c=.o)

# set the name of the resulting executable here
EXEC=test

# the all-target will be called by default when executing make without arguments.
# List the dependencies which should trigger a re-compilation when any changes occured.
all: $(SOURCES) $(EXEC)

# link all the object files and libraries together
$(EXEC): $(OBJECTS)
	$(CXX) $(CXX_LFLAGS) $@ $< $(addprefix -l,$(LIBRARIES))

# this rule creates object files from all the source files. Note the %-symbol as a placeholder for the filename
%.o: %.c
# $@ holds the target-file's name (%.o)
# $< holds the input-file's name/name of the dependency of the rule (%c)
	$(CXX) $(CXX_FLAGS) $@ $<

%.c: %.l
	lex $< $@

clean: clean_objects clean_exec
clean_objects:
	rm -f $(OBJECTS)
clean_exec:
	rm -f $(EXEC)

# declare the following targets as phony, as they will not generate files of the same name.
.PHONY: clean clean_exec clean_objects