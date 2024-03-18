#include "Aircraft.h"

// if > 1 hour, then check separately for each hour and fration thereof.
int calc_faults(double faultsPerHour, Stamp_t time_minutes) {
    int numFaults = 0;
    while (time_minutes > 0) {
        Stamp_t curPeriod = time_minutes;
        if (time_minutes > 60) {
            curPeriod = 60;
        }
        time_minutes -= curPeriod;

        auto rate = 100 * faultsPerHour * static_cast<double>(curPeriod) / 60.0;
        if ((rand() % 100) <= rate) {
            ++numFaults;
        }
    }
    return numFaults;
}

AircraftInstance::AircraftInstance(AirCraftModel aircraftModel, int aircraftId,
                                   double flightMinPerCharge)
    : _model(aircraftModel), _aircraftId(aircraftId), _flightTimePerCharge(flightMinPerCharge),
      _startSegmentTime(0), _telemetry{0, 0, 0, 0, 0} {}

void AircraftInstance::addTelemetry(Stamp_t endTime, double flightTimeToRecordMinutes,
                                    double chargingTimeToRecordMinutes, int flightDistanceInMiles,
                                    int numFaults) {
    _telemetry.totalChargingTime += chargingTimeToRecordMinutes;
    _telemetry.totalFlightHours += flightTimeToRecordMinutes / 60;
    _telemetry.totalFaults += numFaults;
    _telemetry.totalMilesTraveled += flightDistanceInMiles;
    ++_telemetry.chargingStops;
}

// TODO a simpler, better model for this would be to passt in events t5hat just
// start/stop time3s and type. switch b type.
Stamp_t AircraftInstance::updateTelemetry(Stamp_t curTime, const ChargingEvent& recentEvent) {
    double flightMinToRecord = curTime - _startSegmentTime;
    // includes waiting time into charging time.
    double chargingMin = recentEvent.chargingCompletionTime - recentEvent.startWaitingTime;
    int flightMiles = this->_model.cruiseSpeed * flightMinToRecord / 60;
    int numFaults = calc_faults(this->_model.faultsPerHour, flightMinToRecord);
    addTelemetry(curTime, flightMinToRecord, chargingMin, flightMiles, numFaults);
    //    Stamp_t nextChargeTime = curTime + 60 * _model.batteryLife / _model.cruisePowerDraw;
    Stamp_t nextChargeTime = static_cast<Stamp_t>(curTime + this->flightTimePerCharge());
    return nextChargeTime;

    // TODO pondering a refactor:
    // if I refactor ChargingEvent to just be Event w/ State == CHARGING ||
    // FLYING || WAITING and segmentStartTime, segmentEndTime. just update
    // specific fields.
}

// added for unit testing.
ModelId_t AircraftInstance::modelType() { return _model.model; }
