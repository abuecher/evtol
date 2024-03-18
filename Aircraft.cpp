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
        if ((rand() % 100) > rate) {
            ++numFaults;
        }
    }
    return numFaults;
}

AircraftInstance::AircraftInstance(AirCraftModel aircraftModel, int aircraftId,
                                   double flight_time_per_charge_min)
    : _model(aircraftModel), _aircraftId(aircraftId),
      _flightTimePerCharge(flight_time_per_charge_min),
      _startSegmentTime(0), _telemetry({0, 0, 0, 0}) {}

void AircraftInstance::add_telemetry(Stamp_t end_time,
                                     double flight_time_to_record_minutes,
                                     double chargingTimeToRecordMinutes,
                                     int flightDistanceInMiles, int numFaults) {
    _telemetry.total_charging_time += chargingTimeToRecordMinutes;
    _telemetry.total_flight_hours += flight_time_to_record_minutes / 60;
    _telemetry.total_faults += numFaults;
    _telemetry.total_miles_traveled += flightDistanceInMiles;
}

// TODO a simpler, better model for this would be to passt in events t5hat just
// start/stop time3s and type. switch b type.
Stamp_t AircraftInstance::update_telemetry(Stamp_t curTime,
                                        const ChargingEvent &recentEvent) {
    double flightMinToRecord = curTime - _startSegmentTime;
    // includes waiting time into charging time.
    double chargingMin =
        recentEvent.chargingCompletionTime - recentEvent.startWaitingTime;
    int flightMiles = this->_model.cruise_speed * flightMinToRecord / 60;
    int numFaults =
        calc_faults(this->_model.faults_per_hour, flightMinToRecord);
    add_telemetry(curTime, flightMinToRecord, chargingMin, flightMiles,
                  numFaults);
//    Stamp_t nextChargeTime = curTime + 60 * _model.battery_life / _model.cruise_power_draw;
    Stamp_t nextChargeTime = static_cast<Stamp_t>(curTime + this->flightTimePerCharge());
    return nextChargeTime;

    // TODO pondering a refactor: 
    // if I refactor ChargingEvent to just be Event w/ State == CHARGING ||
    // FLYING || WAITING and segmentStartTime, segmentEndTime. just update
    // specific fields.
}

// added for unit testing.
ModelId_t AircraftInstance::modelType() {
    return _model.model;
}
