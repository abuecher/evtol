#include "src/Simulator.h"
#include <gtest/gtest.h>

#include <cstdlib>


// run all tests with seed for srand = 1.
static const int TEST_SEED{1};


TEST(SimulationTest, AlwaysPass) {
    EXPECT_TRUE(true); // This assertion will always pass
}

TEST(SimulationTest, generateTestAircraft_5aircraft) {
    const int NUM_AIRCRAFT{9};

    srand(TEST_SEED); // seed
    Simulator sim;
    auto aircraft = sim.generateTestAircraft(NUM_AIRCRAFT);
    EXPECT_EQ(NUM_AIRCRAFT, aircraft.size());

    // when seed = 1, then we expect:
    // aircraft[0].type = DELTA [1].type=ALPHA,
    // just validating the first occurrence of each for this seed.
    EXPECT_EQ(DELTA, aircraft[0].modelType());
    EXPECT_EQ(100, aircraft[0].flightTimePerCharge());
    EXPECT_EQ(37.2, aircraft[0].batteryChargeTimeMinutes());


    EXPECT_EQ(BRAVO, aircraft[1].modelType());
    EXPECT_EQ(CHARLIE, aircraft[2].modelType());
    EXPECT_EQ(ALPHA, aircraft[3].modelType());
    EXPECT_EQ(DELTA, aircraft[4].modelType());
    EXPECT_EQ(ECHO, aircraft[8].modelType());

    // init will also set initial flight miles (no charging). validate aircraft .00.
    Telemetry t = aircraft[0].getTelemetry();
    EXPECT_EQ(0, t.chargingStops);
    EXPECT_EQ(0.0, t.totalChargingTime); // TODO need to verify this number w/calc.
    EXPECT_EQ(0, t.totalFaults);


    EXPECT_EQ(0, t.totalFlightHours);
    EXPECT_EQ(0, t.totalMilesTraveled);

    // TODO create another test to validate updateFlightTelemetry.
    // double expectedFlightHours = 5.0 / 3.0;
    // EXPECT_EQ(expectedFlightHours, t.totalFlightHours);
    // EXPECT_EQ(150.0, t.totalMilesTraveled);
}


TEST(SimulationTest, simulateUntil_180min) {
    srand(TEST_SEED);
    int numTestAircraft = 20;
    int simTime = 180;
    // run the full.
    Simulator sim;
    auto a = sim.generateTestAircraft(numTestAircraft);
    EXPECT_EQ(numTestAircraft, a.size());

    sim.simulateUntil(numTestAircraft, simTime);
    auto report = sim.generateSimulationReport();

    auto modelList = report.getListOfModels();
    EXPECT_EQ(5, modelList.size());

    auto telemetryMap = report.getCopyOfTelemetry();
    EXPECT_EQ(5, telemetryMap.size());
}

TEST(AircraftInstanceTest, updateTelemetryForCharging) {
    srand(TEST_SEED);
    Simulator sim;
    auto aircraft = sim.generateTestAircraft(1);
    EXPECT_EQ(1, aircraft.size());
    EXPECT_EQ(DELTA, aircraft[0].modelType());
    EXPECT_EQ(100, aircraft[0].flightTimePerCharge());
    EXPECT_TRUE(37.2 == aircraft[0].batteryChargeTimeMinutes()) << "batteryChargeTimeInMinutes did not match";
    EXPECT_EQ(0, aircraft[0].startSegmentTime());
    ChargingEvent testEvent{0, 10,
                            static_cast<Stamp_t>(10 + aircraft[0].batteryChargeTimeMinutes()), 0};

    auto nextTime = aircraft[0].updateTelemetryForCharging(100, testEvent);
    EXPECT_EQ(
        200,
        nextTime);

    Telemetry t = aircraft[0].getTelemetry();
    EXPECT_EQ(1, t.chargingStops);
    EXPECT_EQ(47.2, t.totalChargingTime); // TODO need to verify this number w/calc.
    EXPECT_EQ(0, t.totalFaults);

    // test this after runnint update flight telemetry.
//    double expectedFlightHours = 5.0 / 3.0;
//    EXPECT_EQ(expectedFlightHours, t.totalFlightHours);
//    EXPECT_EQ(150.0, t.totalMilesTraveled);

}
