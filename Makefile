# ==================================================================
#  HOW TO RUN  (open MSYS2 / MinGW terminal in this folder, then):
#
#     mingw32-make run-fly     -> price the butterfly once
#     mingw32-make run-sens    -> price vs spot / vol / barrier  (writes the .csv files)
#     mingw32-make run-conv    -> convergence study (price + error vs number of paths)
#     mingw32-make run-bb      -> discrete vs Brownian-bridge monitoring
#     mingw32-make run-anti    -> antithetic variates comparison
#     mingw32-make clean       -> delete the built .exe files
#
#  (the Excel function is in the separate xlw / Visual Studio project)
# ==================================================================

CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall

# every folder that has a header in it
INCLUDES = -ISRC/Core -ISRC/Processes/GBM \
           -ISRC/Instruments -ISRC/Instruments/Options \
           -ISRC/Engines -ISRC/Engines/Analytic \
           -ISRC/Engines/MC -ISRC/Engines/MC/RNG -ISRC/Engines/MC/BarrierMonitoring \
           -ISRC/TermStructures -ISRC/Statistics

# all the library source files (everything except the App/ programs that have main())
SRC = SRC/Core/normals.cpp \
      SRC/Core/helpers.cpp \
      SRC/Processes/GBM/black_scholes_process.cpp \
      SRC/TermStructures/parameters.cpp \
      SRC/Instruments/instrument.cpp \
      SRC/Instruments/Options/payoff.cpp \
      SRC/Instruments/Options/EuropeanOption.cpp \
      SRC/Instruments/Options/BarrierOption.cpp \
      SRC/Instruments/Portfolio.cpp \
      SRC/Engines/Analytic/BlackScholes.cpp \
      SRC/Engines/Analytic/AnalyticEuropeanEngine.cpp \
      SRC/Engines/MC/MonteCarloBarrierEngine.cpp \
      SRC/Engines/MC/RNG/Mt19937.cpp \
      SRC/Engines/MC/RNG/AntiThetic.cpp \
      SRC/Engines/MC/RNG/ParkMiller.cpp \
      SRC/Engines/MC/BarrierMonitoring/DiscreteMonitor.cpp \
      SRC/Engines/MC/BarrierMonitoring/BrownianBridge.cpp \
      SRC/Statistics/conf_limits.cpp

# build a program = its App/ file (with main) + all the library sources above
butterfly.exe: App/test_butterfly.cpp $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

sensitivity.exe: App/butterfly_sensitivity.cpp $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

convergence.exe: App/convergence.cpp $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

bb.exe: App/test_brownian_bridge.cpp $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

anti.exe: App/test_anthitetic.cpp $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

barrier.exe: App/test_barrier.cpp $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

# build, then run
run-fly: butterfly.exe
	./butterfly.exe

run-sens: sensitivity.exe
	./sensitivity.exe

run-conv: convergence.exe
	./convergence.exe

run-bb: bb.exe
	./bb.exe

run-anti: anti.exe
	./anti.exe

run: barrier.exe
	./barrier.exe

clean:
	rm -f *.exe

.PHONY: run run-fly run-sens run-conv run-bb run-anti clean
