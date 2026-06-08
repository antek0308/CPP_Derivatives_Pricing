# ============================================================
#  Makefile for the butterfly pricer (QL-style engine)
#
#  Usage:
#     mingw32-make          build ./pricer.exe
#     mingw32-make run      build, then run it
#     mingw32-make clean    delete the executable
# ============================================================

CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall

# one -I per folder that contains a header
INCLUDES := -ISRC/Core \
            -ISRC/Processes/GBM \
            -ISRC/Instruments \
            -ISRC/Instruments/Options \
            -ISRC/Engines \
            -ISRC/Engines/Analytic \
            -ISRC/Engines/MC

# every .cpp in the program (App entry point + library sources)
SRCS := App/test_engine.cpp \
        SRC/Core/normals.cpp \
        SRC/Processes/GBM/black_scholes_process.cpp \
        SRC/Instruments/instrument.cpp \
        SRC/Instruments/Options/payoff.cpp \
        SRC/Instruments/Options/EuropeanOption.cpp \
        SRC/Engines/Analytic/BlackScholes.cpp \
        SRC/Engines/Analytic/AnalyticEuropeanEngine.cpp \
        SRC/Engines/MC/monte_carlo_engine.cpp

TARGET := pricer.exe

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRCS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: run clean
