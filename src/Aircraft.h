#ifndef EVTOL_AIRCRAFT_H
#define EVTOL_AIRCRAFT_H

#include <limits>
#include <string>
#include "common_defs.h"


using Stamp_t = double;
const Stamp_t ILLEGAL_TIME = std::numeric_limits<double>::max(); // maxint 32

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
    int cruiseSpeed; // MPH
    int batteryLife; // kWh

    // TODO assumption: This includes time spent waiting for a charger. Find out
    // if assumption valid.
    double timeToCharge;    // hrs -
    double cruisePowerDraw; // kWh / mile
    int passengerCount;
    double faultsPerHour;  // probability if faults per hour in decimal format.
    std::string typeName; // company name
};

/**
 * Things each aircraft instance needs to track.
 */
struct Telemetry {
    double totalFlightHours;
    double totalMilesTraveled;

    // Includes time spent waiting for a charger. TODO is that assumption valid?
    double totalChargingTime;
    int chargingStops;
    int totalFaults;
    int passengers; // this is always maxed out for this exercise.
};

struct ChargingEvent {
    Stamp_t startWaitingTime;
    Stamp_t startChargingTime;
    Stamp_t chargingCompletionTime;
    int aircraftId;
};

class AircraftInstance {
public:
    // although ctor is overriden it is only setting vars so no need for "rule of
    // 5" here.
    AircraftInstance(AirCraftModel aircraftModel, int aircraftId, double flightMinPerCharge);

    /**
     * Update telemetry after finished charging and return next time it will
     * need to charge.
     */
    Stamp_t updateTelemetryForCharging(Stamp_t curTime, const ChargingEvent& recentEvent);

    /**
     * Update aircraft telemetry for a flight segment. Returns time flight
     * segment ends.
     */
    Stamp_t updateTelemetryForFlight(Stamp_t curTime, Stamp_t endTime);

    // getters
    inline int aircraftId() const { return _aircraftId; }
    inline double flightTimePerCharge() const { return _flightTimePerCharge; }
    inline Telemetry getTelemetry() const { return _telemetry; }
    inline ModelId_t modelType() const { return _model.model; }
    inline std::string modelName() const { return _model.typeName; }

    // Number of hours to recharge the battery.
    inline double batteryChargeTimeMinutes() const {
        return 60 * _model.timeToCharge;
    }

    inline Stamp_t startSegmentTime() const { return _startSegmentTime; }

    void startSegmentTime(Stamp_t nextSegmentTime) { _startSegmentTime = nextSegmentTime; }

private:
    void addTelemetry(Stamp_t endTime, double flightTimeToRecordMinutes,
                      double chargingTimeToRecordMinutes, int flightDistanceInMiles, int numFaults, bool incrementChargingStops);
//internal state
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