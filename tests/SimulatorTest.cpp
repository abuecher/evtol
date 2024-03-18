#include <gtest/gtest.h>
#include "src/Simulator.h"

#include <cstdlib>

// You may want to include "Simulator.h" if you're testing functions from it

TEST(SimulationTest, AlwaysPass) {
    EXPECT_TRUE(true); // This assertion will always pass 
}
//std::vector<AircraftInstance> Simulator::generateTestAircraft(int numAircraft)


TEST(SimulationTest, generateTestAircraft_5aircraft) {
    const int TEST_SEED{1};
    const int NUM_AIRCRAFT{9};

    srand(TEST_SEED); // seed    
    Simulator sim;
    auto aircraft = sim.generateTestAircraft(NUM_AIRCRAFT);
    EXPECT_EQ(NUM_AIRCRAFT, aircraft.size());

    // when seed = 1, then we expect:
    // aircraft[0].type = DELTA [1].type=ALPHA,
    // just validaatigg the first occurence of each for this seed.
    EXPECT_EQ(DELTA, aircraft[0].modelType());
    EXPECT_EQ(150, aircraft[0].flightTimePerCharge());
    EXPECT_EQ(100, aircraft[0].batteryChargeTimeMinutes());


    EXPECT_EQ(BRAVO, aircraft[1].modelType());
    EXPECT_EQ(CHARLIE, aircraft[2].modelType());
    EXPECT_EQ(ALPHA, aircraft[3].modelType());
    EXPECT_EQ(DELTA, aircraft[4].modelType());
    EXPECT_EQ(ECHO, aircraft[8].modelType());


}

TEST(SimulationTest, simulateUntil_5min) {
//    const Stamp_t TEST_PERIOD_MINUTES {5};
//    sim.simulateUntil(TEST_PERRIOD_MINUTES);
    EXPECT_TRUE(true); // This assertion will always pass
}

TEST(AircraftInstanceTest, updateTelemetry) {
    Simulator sim;
    auto aircraft = sim.generateTestAircraft(1);
    EXPECT_EQ(1, aircraft.size());
    EXPECT_EQ(DELTA, aircraft[0].modelType());
    EXPECT_EQ(150, aircraft[0].flightTimePerCharge());
    EXPECT_EQ(100, aircraft[0].batteryChargeTimeMinutes());
    EXPECT_EQ(0, aircraft[0].startSegmentTime());
    ChargingEvent testEvent {0, 10, static_cast<Stamp_t>(10 + aircraft[0].batteryChargeTimeMinutes()), 0};

    auto nextTime = aircraft[0].update_telemetry(100, testEvent);
    EXPECT_EQ(139, nextTime); // ADAM_DEBUG this may be the wrong number, placeholder value. need to verify..
}

