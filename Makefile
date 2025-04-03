CXX = g++
# Regular optimized build flags
CXXFLAGS = -std=c++17 -Wall -Wextra -I. \
           -O3 -march=native -flto \
           -ffast-math -funroll-loops \
           -ftree-vectorize -fomit-frame-pointer

# Profile build flags (for gperftools)
PROFILE_CXXFLAGS = -std=c++17 -Wall -Wextra -I. \
                   -O2 -g -fno-omit-frame-pointer
PROFILE_LDFLAGS = -L/opt/homebrew/lib -lprofiler

PROFILE_TARGET = itch_parser_profile

BUILD_DIR = build
TARGET = itch_parser

# List all source files including handlers
SOURCES = main.cpp messageDispatcher.cpp \
          handlers/systemEvent.cpp \
          handlers/addOrderNoMPID.cpp \
          handlers/addOrderWithMPID.cpp \
          handlers/brokenTrade.cpp \
          handlers/dlcr.cpp \
          handlers/ipoQuotingPeriodUpdate.cpp \
          handlers/luldAuction.cpp \
          handlers/marketParticipantPosition.cpp \
          handlers/mwcbDeclineLevel.cpp \
          handlers/mwcbStatus.cpp \
          handlers/noii.cpp \
          handlers/operationalHalt.cpp \
          handlers/cancelOrder.cpp \
          handlers/deleteOrder.cpp \
          handlers/orderExecuted.cpp \
          handlers/orderExecutedWithPrice.cpp \
          handlers/replaceOrder.cpp \
          handlers/regSHO.cpp \
          handlers/rpii.cpp \
          handlers/stockDirectory.cpp \
          handlers/stockTradingAction.cpp \
          handlers/tradeCross.cpp \
          handlers/tradeNonCross.cpp

# Transform SOURCES into object files in the build directory
OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

# Default target
all: $(TARGET)

# Create the build directory and handlers subdirectory if they don't exist
$(BUILD_DIR):
	mkdir -p $@/handlers

# The target depends on the object files in the build directory
$(TARGET): $(BUILD_DIR) $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

# Rule to compile .cpp files from the main directory into the build directory
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Rule to compile handler files
$(BUILD_DIR)/handlers/%.o: handlers/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Update profile target
profile: override CXXFLAGS = $(PROFILE_CXXFLAGS)
profile: clean $(BUILD_DIR) $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(PROFILE_TARGET) $(OBJECTS) $(PROFILE_LDFLAGS)

# Update clean target to remove profile artifacts
clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(PROFILE_TARGET) gmon.out profile.txt
