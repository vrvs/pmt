CC = g++

CC_FLAGS = -std=c++14 -Wall

SOURCE_DIR = src/
INSTALL_DIR = bin/
SOURCES = $(wildcard $(SOURCE_DIR)*.cpp)
OBJECTS = $(SOURCES:$(SOURCE_DIR)%.cpp=$(INSTALL_DIR)%.o)
EXEC = install
COMMAND = pmt

$(EXEC): setup $(OBJECTS)
	@$(CC) $(CC_FLAGS) $(OBJECTS) -o $(INSTALL_DIR)$(COMMAND)
	@echo "Building" $(COMMAND) "in" $(INSTALL_DIR) "..."

setup:
	@mkdir -p $(INSTALL_DIR)
	
$(INSTALL_DIR)%.o: $(SOURCE_DIR)%.cpp
	@$(CC) $(CC_FLAGS) -c $< -o $@
	@echo "Compiling " $< "..."

clean: 
	@rm -rf $(INSTALL_DIR)
	@echo "Cleaning project ..."

