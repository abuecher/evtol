#ifndef EVTOL_AIRCRAFT_H
#define EVTOL_AIRCRAFT_H

#include "common_defs.h"
#include <limits>
#include <string>


using Stamp_t = double;
const Stamp_t ILLEGAL_TIME = std::numeric_limits<double>::max();

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
 * Model the provided table for models of aircraft.
 */
struct AirCraftModel {
    ModelId_t model;
    int cruiseSpeed;        // MPH
    int batteryLife;        // kWh
    double timeToCharge;    // hrs -
    double cruisePowerDraw; // kWh / mile
    int passengerCount;
    double faultsPerHour; // probability if faults per hour in decimal format.
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
    /**
     * although ctor is overriden it is only setting vars so no need for "rule of 5" here.
     */
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
    inline double batteryChargeTimeMinutes() const { return 60 * _model.timeToCharge; }

    inline Stamp_t startSegmentTime() const { return _startSegmentTime; }

    void startSegmentTime(Stamp_t nextSegmentTime) { _startSegmentTime = nextSegmentTime; }

private:
    void addTelemetry(Stamp_t endTime, double flightTimeToRecordMinutes,
                      double chargingTimeToRecordMinutes, int flightDistanceInMiles, int numFaults,
                      bool incrementChargingStops);
    // internal state
private:
    AirCraftModel _model; // copy of the model.
    int _aircraftId;
    double _flightTimePerCharge;
    Stamp_t _startSegmentTime;
    Telemetry _telemetry;
};

#endif // EVTOL_AIRCRAFT_H