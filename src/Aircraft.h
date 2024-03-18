#ifndef EVTOL_AIRCRAFT_H
#define EVTOL_AIRCRAFT_H

#include <string>
#include <limits>

using Stamp_t = uint32_t;
const Stamp_t ILLEGAL_TIME = std::numeric_limits<uint32_t>::max(); // maxint 32

/**
 * Modelling each aircraft as an object with its own state machine. May or may
 * not break it down into components (breaking it down provides a more OOD that
 * might be more like how one would model a more complex real-world design. On
 * the other hand it actually complicates the simulation without much benefit
 * since the problem does not ask one to do things like swap out batteries or
 * repair components).
 */
enum States {
    STATE_INIT,
    STATE_FLYING,
    STATE_WAITING_FOR_CHARGER, // can this be omitted as a state/consolidated w
                               // charging?
    STATE_CHARGING,
};

enum ModelId_t {
    ALPHA,
    BRAVO,
    CHARLIE,
    DELTA,
    ECHO,
    // add new ones here.
    NONE, // this must always stay last to iterate over models
};

/**
 * Model the provided table for models of aircraft. Since the simulation is
 * basic, we can reuse this for the aircraft instances not just models. Either
 * copy base model stats or something. Once airborne each aircraft will
 * separately need to deal with resource contention for chargers even if they
 * started at same time.
 */
struct AirCraftModel {
    ModelId_t model;
    int cruise_speed; // MPH
    int battery_life; // kWh

    // TODO assumption: This includes time spent waiting for a charger. Find out
    // if assumption valid.
    float time_to_charge;    // hrs -
    float cruise_power_draw; // kWh / mile
    int passenger_count;
    float faults_per_hour; // probability if faults per hour in decimal format.
    std::string typeName;  // company name
};

/**
 * Things each aircraft instance needs to track.
 */
struct Telemetry {
    long total_flight_hours;
    float total_miles_traveled;

    // TODO includes time spent waiting for a charger. find out if that is a
    // valid assumption. If not I need adjust the calculation
    float total_charging_time;
    int total_faults;

    // int num_passengers; // passengers are always max so no need to track that
    // per instance.
};

struct ChargingEvent {
    Stamp_t startWaitingTime;
    Stamp_t startChargingTime;
    Stamp_t chargingCompletionTime;
    int aircraftId;
    //    AircraftInstance& aircraft; // do I need to paralleize? if so use
    //    int. If not can suie AircraftInstance& and eliminate a redirect.

    // States state;   // TODO I can clean up some issues if an aircraft just
    // holds the most recent Event?
};


class AircraftInstance {

  public:
    // although ctror overriden it is only setting vars so no need
    // for "rule of 5" here.
    AircraftInstance(AirCraftModel aircraftModel, int aircraftId,
                     double flight_time_per_charge);

    void add_telemetry(Stamp_t end_time, double flight_time_to_record_minutes,
                       double chargingTimeToRecordMinutes,
                       int flightDistanceInMiles, int numFaults);

    // TODO combine w/add_telem

    /**
     * Update telemetry and return next time it will need to charge.
    */
    Stamp_t update_telemetry(Stamp_t curTime, const ChargingEvent &recentEvent);
    Stamp_t get_startFlightSegmentTime() const { return _startSegmentTime; }

    // getters
    inline int aircraftId() const { return _aircraftId; }
    inline double flightTimePerCharge() const { return _flightTimePerCharge; }
    inline Telemetry get_telemetry() const { return _telemetry; }

    // Number of hours to recharge the battery.
    inline float batteryChargeTimeMinutes() const { return 60 * _model.time_to_charge; } // TODO this is in hours Most others in min.
    inline Stamp_t startSegmentTime() const { return _startSegmentTime; }
    void startSegmentTime(Stamp_t nextSegmentTime) {
        _startSegmentTime = nextSegmentTime;
    }

    ModelId_t modelType();

private:
    // these are consts so no need to encapsulate.
    AirCraftModel _model; // copy of the model.
    int _aircraftId;
    double _flightTimePerCharge; // TODO move this into model.

    // internal state. start of next flight segment. if actively charging this
    // is start of charging otherwise it is start for flight time.
    Stamp_t _startSegmentTime; // start time of current flight leg if not in //
                               // TODO this is doing double-dutyt cleanm it up.
    Telemetry _telemetry;
};



#endif // EVTOL_AIRCRAFT_H