#include <cstdlib>
#include <iostream>
#include <vector>

#include "src/Simulator.h"

/*
 * Ideas for building a simulation of a simplified electric vehicle. The
 * simulation will simulate the distance traveled, energy consumed stops,
 * refueling time, etc. I will model the vehicle using OOP and break it down in
 * the following manner.
 *
 * Possible ways to model this:
 * 1. AircraftModel is composed of classes for Engine, Battery, Cabin. This is
 * more like I'd expect it to be in a more complex, real world scenario since
 * you'd need to be able to simulate swapping out engines, batteries, etc.
 * 2. One AircraftModel class containing all oF he fields from the table. Each
 * model would be an instance of that. Can have each unique aircraft be a copy
 * of the model object (perhaps with extra id) or have some vector counter.
 *
 * thoughts: going with approach 2 since (1) over-complicates what we are
 * actually simulating by over encapsulating data and leaves the question of how
 * timing would work. Likely would need some simulated clock class and mutexes
 * guard the charger resources. We do not need to swap any components so (1) has
 * no upside.
 *
 * If we track everting in a queue waiting for the chargers, autoincrement their
 * milatge time when leaving chargers, and 
 * Need to track each aircraft separately since they will all need to deal with
 * resource contention separately which will result in different instances of
 * same model having separate charging times. I could have the data in the table
 * per model in one table and each aircraft will track its own totals + modelId
 * + aircraftId.
 * TODO write up the above. Add some diagrams where needed.
 * remember in case of #2 need to validate that the aircraft in the chargers of
 * the end of the sim time have correct time information since #2 will mostly
 * rely on an update to state at end of stay there.
 */

const Stamp_t END_TIME_IN_MINUTES = 3 * 60; // 3 hours in minutes
const int NUM_AIRCRAFT = 20;

int main() {

    // for testing, force a specific seed (gtest cases do this).
    // srand(1);

    Simulator sim;
    std::cout << "Running Simulation" << std::endl;
    sim.simulateUntil(NUM_AIRCRAFT, END_TIME_IN_MINUTES);
    std::cout << "Creating report" << std::endl;
    auto report = sim.generateSimulationReport();
    sim.writeReport(report);
    return 0;
}
