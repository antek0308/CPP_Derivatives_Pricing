# ============================================================
#  Makefile for the butterfly pricer (QL-style engine)
#
#  Usage:
#     mingw32-make            build ./barrier.exe (default)
#     mingw32-make run        build, then run the barrier test
#     mingw32-make run-eu     build, then run the European test
#     mingw32-make clean      delete the executables
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
            -ISRC/Engines/MC \
            -ISRC/Engines/MC/RNG \
            -ISRC/Statistics

# library sources shared by both executables (no main())
COMMON_SRCS := SRC/Core/normals.cpp \
               SRC/Core/helpers.cpp \
               SRC/Processes/GBM/black_scholes_process.cpp \
               SRC/Instruments/instrument.cpp \
               SRC/Instruments/Options/payoff.cpp \
               SRC/Instruments/Options/EuropeanOption.cpp \
               SRC/Instruments/Options/BarrierOption.cpp \
               SRC/Instruments/Portfolio.cpp \
               SRC/Engines/Analytic/BlackScholes.cpp \
               SRC/Engines/Analytic/AnalyticEuropeanEngine.cpp \
               SRC/Engines/MC/RNG/Mt19937.cpp \
               SRC/Engines/MC/RNG/AntiThetic.cpp \
               SRC/Engines/MC/RNG/ParkMiller.cpp \
               SRC/Statistics/conf_limits.cpp

barrier.exe: App/test_barrier.cpp $(COMMON_SRCS) SRC/Engines/MC/MonteCarloBarrierEngine.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

pricer.exe: App/test_engine.cpp $(COMMON_SRCS) SRC/Engines/MC/MonteCarloEngine.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

anti.exe: App/test_anthitetic.cpp $(COMMON_SRCS) SRC/Engines/MC/MonteCarloBarrierEngine.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

butterfly.exe: App/test_butterfly.cpp $(COMMON_SRCS) SRC/Engines/MC/MonteCarloBarrierEngine.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

run: barrier.exe
	./barrier.exe

run-eu: pricer.exe
	./pricer.exe

run-anti: anti.exe
	./anti.exe

run-fly: butterfly.exe
	./butterfly.exe

clean:
	rm -f barrier.exe pricer.exe anti.exe butterfly.exe

.PHONY: run run-eu run-anti run-fly clean
