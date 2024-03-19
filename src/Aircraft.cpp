#include "Aircraft.h"
#include <iostream>


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
      _startSegmentTime(0), _telemetry{0, 0, 0, 0, 0, aircraftModel.passengerCount } {}

void AircraftInstance::addTelemetry(Stamp_t endTime, double flightTimeToRecordMinutes,
                                    double chargingTimeToRecordMinutes, int flightDistanceInMiles,
                                    int numFaults, bool incrementChargingStops) {
    _telemetry.totalChargingTime += chargingTimeToRecordMinutes;
    _telemetry.totalFlightHours += flightTimeToRecordMinutes / 60;
    _telemetry.totalFaults += numFaults;
    _telemetry.totalMilesTraveled += flightDistanceInMiles;
    if (incrementChargingStops) ++_telemetry.chargingStops;
}

// TODO a simpler, better model for this would be to passt in events t5hat just
// start/stop time3s and type. switch b type.
Stamp_t AircraftInstance::updateTelemetryForCharging(Stamp_t curTime, const ChargingEvent& recentEvent) {

    // includes waiting time into charging time.
    double chargingMin = recentEvent.chargingCompletionTime - recentEvent.startWaitingTime;

    addTelemetry(curTime, 0, chargingMin, 0, 0, true);
    Stamp_t nextChargeTime = static_cast<Stamp_t>(curTime + this->flightTimePerCharge());
#ifdef USE_DEBUG_INFO
    std::cout << "aircraft:" << _aircraftId << " updated charge telemetry to {" << _telemetry.passengers << "," << _telemetry.totalMilesTraveled
        << "," << _telemetry.totalFaults << "," << _telemetry.chargingStops << "," << _telemetry.totalChargingTime
        << "," << _telemetry.totalFlightHours << "}" << std::endl;
#endif
    return nextChargeTime;
}

Stamp_t AircraftInstance::updateTelemetryForFlight(Stamp_t curTime, Stamp_t endTime) {
    curTime = std::min(curTime, endTime); // clip to end of simulatin time.
    double flightMinToRecord = curTime - _startSegmentTime;
    int flightMiles = this->_model.cruiseSpeed * flightMinToRecord / 60;
    int numFaults = calc_faults(this->_model.faultsPerHour, flightMinToRecord);
    addTelemetry(curTime, flightMinToRecord, 0, flightMiles, numFaults, false);
    Stamp_t nextChargeTime = static_cast<Stamp_t>(curTime + this->flightTimePerCharge());
#ifdef USE_DEBUG_INFO
    std::cout << "aircraft:" << _aircraftId << " updated flight telemetry to {" << _telemetry.passengers << "," << _telemetry.totalMilesTraveled
        << "," << _telemetry.totalFaults << "," << _telemetry.chargingStops << "," << _telemetry.totalChargingTime
        << "," << _telemetry.totalFlightHours << "}" << std::endl;
#endif
    return nextChargeTime;
}
