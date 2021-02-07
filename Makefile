PROGRAM = a.out

CURRENT_DIR = $(shell pwd)
BUILD_DIR = $(CURRENT_DIR)/build
SRC_DIR = $(CURRENT_DIR)/src
MODEL_DIR = $(CURRENT_DIR)/src/model
VIEW_DIR = $(CURRENT_DIR)/src/view
CTRL_DIR = $(CURRENT_DIR)/src/controller


CXX = g++
WFLAGS 	= -Wall -Wextra -std=c++17
HINCLUDES = -I/usr/X11R6/include -I/usr/pkg/include
CFLAGS 	= $(WFLAGS) $(HINCLUDES)
LDFLAGS = -L/usr/X11R6/lib -L/usr/pkg/lib
LDLIBS 	= -lglut -lGLU -lGL

MAKE_OBJECT_FILE = 			\
	mkdir -p $(BUILD_DIR); 	\
	cd $(BUILD_DIR); 		\
	$(CXX) $(CFLAGS) $< -c $(LDLIBS);

# $(PROGRAM): $(BUILD_DIR)/*.o
$(PROGRAM): $(BUILD_DIR)/game.o $(BUILD_DIR)/platforms.o $(BUILD_DIR)/render.o $(BUILD_DIR)/image.o $(BUILD_DIR)/player.o $(BUILD_DIR)/textures.o $(BUILD_DIR)/window.o
	$(CXX) $(HINCLUDES) $(LDFLAGS) $^ $(LDLIBS) -o $(PROGRAM)

$(BUILD_DIR)/game.o: $(CTRL_DIR)/game.cpp $(MODEL_DIR)/constants.h $(VIEW_DIR)/window.h $(CTRL_DIR)/error_handling.h
	$(MAKE_OBJECT_FILE)

$(BUILD_DIR)/platforms.o: $(MODEL_DIR)/platforms.cpp $(MODEL_DIR)/platforms.h
	$(MAKE_OBJECT_FILE)

$(BUILD_DIR)/render.o: $(VIEW_DIR)/render.cpp $(VIEW_DIR)/render.h
	$(MAKE_OBJECT_FILE)

$(BUILD_DIR)/image.o: $(VIEW_DIR)/image.c $(VIEW_DIR)/image.h
	$(MAKE_OBJECT_FILE)

$(BUILD_DIR)/player.o: $(MODEL_DIR)/player.cpp $(MODEL_DIR)/player.h
	$(MAKE_OBJECT_FILE)

$(BUILD_DIR)/textures.o: $(VIEW_DIR)/textures.cpp $(VIEW_DIR)/textures.h
	$(MAKE_OBJECT_FILE)

$(BUILD_DIR)/window.o: $(VIEW_DIR)/window.cpp $(VIEW_DIR)/window.h
	$(MAKE_OBJECT_FILE)

.PHONY: clean

clean:
	rm -f *.o $(PROGRAM); \
	rm -rf build;

