#include <iostream>
#include <vector>

// ADAM_DEBUG sketching out basic ideas.
/*
 * Ideas for building a simulation of a simplified electric vehicle. The simulation will simulate the distance traveled,
 * energy consumed stops, refueling time, etc. I will model the vehicle using OOP and break it down in the following
 * manner.
 *
 * Possible ways to model this:
 * 1. AircraftModel is composed of classes for Engine, Battery, Cabin. This is more like I'd expect it to be in a more
 *     complex, real world scenario since you'd need to be able to simulate swapping out engines, batteries, etc.
 * 2. One AircraftModel class containing all og he fields from the table. Each model would be an instance of that. Can
 *     have each unique aircraft be a copy of the model object (perhaps with extra id) or have some vector counter.
 *
 * thoughts: going with approach 23 since (1) over-complicates what we are actually simulating by over encapsulating data.
 * We do not need to swap any components so (1) has no upside.
 *
 * Need to track each aircraft separately since they will all need to deal with resource contention separately which will
 * result in different instances of same model having separate charging times. I could have the data in the table per model
 * in one table and each aircraft will track its own totals + modelId + aircraftId.
 * TODO write up the above. Add some diagrams where needed.
 */

typedef uint32_t Stamp_t;

/**
 * Modelling each aircraft as an object with its own state machine. May or may not break it down
 * into components (breaking it down provides a more OOD that might be more like how one would
 * model a more complex real-world design. On the other hand it actually complicates the simulation
 * without much benefit since the problem does not ask one to do things like swap out batteries or
 * repair components).
 */
enum States {
    STATE_INIT,
    STATE_FLYING,
    // STATE_WAITING_FOR_CHARGER, // I can omit this and include the wait as charging time id the object is actively waiting for a slot.
    STATE_CHARGING,
};


/**
 * Model the provided table for models of aircraft. Since the simulation is basic, we can reuse this
 * for the aircraft instances not just models. Either copy base model stats or something. Once
 * airborne each aircraft will separately need to deal with resource contention for chargers even
 * if they started at same time.
 */
struct AirCraftModel {
    int passenger_count;
    int cruise_speed; // MPH
    int battery_life; // kWh
    float time_to_charge; // hrs - TODO assumption: This includes time spent waiting for a charger. Find out if assumption valid.
    float cruise_power_draw; // kWh / mile
    float faults_per_hour; // probability if faults per hour in decimal format.
    std::string typeName; // company name
};

enum ModelId_t {
    ALPHA,
    BRAVO,
    CHARLIE,
    DELTA,
    ECHO,
};

/**
 * Things each aircraft instance needs to track.
 */
struct Telemetry {
    long total_flight_hours;
    float total_miles_traveled;
    float total_charging_time; // includes time spent waiting for a charger. TODO find out if that is a valid assumption. If not I need adjust the calculation
    int total_faults;
    // int num_passengers;// passengers are always max so no need to track that per instance.
};

/**
 * This is a class rather than a struct  since I plan on using for more than passing raw data.
 */
struct AircraftInstance {
    ModelId_t modelId;
    Telemetry telemetry;
};


class Simulation {
private:
    std::vector<AircraftInstance> _aircrafts;

public:
    void create_aircrafts(); ///< populate _aircrafts with 20 random aircraft.
    void run_simulation();
    void generate_report() const; // generate report for all aircraft
};

// ~ADAM_DEBUG - end of rough OOD sketch. I did iterate a little on it at first.

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
