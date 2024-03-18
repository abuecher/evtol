#include <cstdlib>
#include <iostream>
#include <vector>

#include "src/Simulator.h"

// ADAM_DEBUG sketching out basic ideas.
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
 * 2. One AircraftModel class containing all og he fields from the table. Each
 * model would be an instance of that. Can have each unique aircraft be a copy
 * of the model object (perhaps with extra id) or have some vector counter.
 *
 * thoughts: going with approach 23 since (1) over-complicates what we are
 * actually simulating by over encapsulating data. We do not need to swap any
 * components so (1) has no upside.
 *
 * Need to track each aircraft separately since they will all need to deal with
 * resource contention separately which will result in different instances of
 * same model having separate charging times. I could have the data in the table
 * per model in one table and each aircraft will track its own totals + modelId
 * + aircraftId.
 * TODO write up the above. Add some diagrams where needed.
 */

// ~ADAM_DEBUG - end of rough OOD sketch. I did iterate a little on it at first.
const Stamp_t END_TIME_IN_MINUTES = 3 * 60; // 3 hours in minutes

int main() {

    // for testing, force a specific seed.
    srand(1);

    // std::cout << "Preparing Simulation" << std::endl;
    Simulator sim;
    // sim.initChargerWaitingQueue();
    std::cout << "Running Simulation" << std::endl;
    sim.simulateUntil(END_TIME_IN_MINUTES);
    std::cout << "Running Simulation" << std::endl;
    auto report = sim.generate_report();
    std::cout << "completed stats:\n" << report.toString() << std::endl;
    return 0;
}
