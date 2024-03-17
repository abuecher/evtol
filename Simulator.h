//
// Created by adam on 3/16/24.
//

#ifndef EVTOL_SIMULATOR_H
#define EVTOL_SIMULATOR_H

#include <vector>
#include <string>

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
    ModelId_t model;
    int cruise_speed; // MPH
    int battery_life; // kWh
    float time_to_charge; // hrs - TODO assumption: This includes time spent waiting for a charger. Find out if assumption valid.
    float cruise_power_draw; // kWh / mile
    int passenger_count;
    float faults_per_hour; // probability if faults per hour in decimal format.
    std::string typeName; // company name
};

enum ModelId_t {
    ALPHA,
    BRAVO,
    CHARLIE,
    DELTA,
    ECHO,

    NONE, // this must always stay last to iterate over models
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

class Report {
public:
    std::string toString() const;
};


class Simulator {
private:
    std::vector<AircraftInstance> _aircrafts;
public:
    void init_aircrafts(); ///< populate _aircrafts with 20 random aircraft.
    void run_simulation();
    Report generate_report() const; // generate report for all aircraft
};


#endif //EVTOL_SIMULATOR_H
