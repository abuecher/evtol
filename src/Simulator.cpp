//
// Created by adam on 3/16/24.
//

#include "Simulator.h"

#include <array>
#include <cstdlib>
#include <map>
#include <queue>
#include <iostream>
#include <fstream>

const int NUM_AIRCRAFT = 20;
const int NUM_MODELS = 5;
const int NUM_CHARGERS = 3;
const int HARDCODED_SEED = 1;


static const AirCraftModel models[] = {
    {ALPHA, 120, 320, 0.6, 1.6, 4, 0.25, "ALPHA"},
    {BRAVO, 100, 100, 0.2, 1.5, 5, 0.10, "BRAVO"},
    {CHARLIE, 160, 220, 0.8, 2.2, 3, 0.05, "CHARLIE"},
    {DELTA, 90, 120, 0.62, 0.8, 2, 0.22, "DELTA"},
    {ECHO, 30, 150, 0.3, 5.8, 2, 0.61, "ECHO"},
    // add new ones here.
    {NONE, 0, 0, 0.0, 0.0, 0, 0.0, "(NULL ENTRY)"}};


ModelId_t Simulator::pick_random_model() const {
    int n = rand() % NUM_MODELS;
    ModelId_t model = static_cast<ModelId_t>(n);
    //    assert(model < NONE);
    return model;
}

std::vector<AircraftInstance> Simulator::generateTestAircraft(int numAircraft) const {
    std::vector<AircraftInstance> aircraft;
    for (int i = 0; i < numAircraft; ++i) {
        int aircraftId = i;
        auto aircraftType = pick_random_model();
        double milesFlightPerCharge = models[aircraftType].battery_life / models[aircraftType].cruise_power_draw;
        double hoursFlightPerCharge = milesFlightPerCharge / models[aircraftType].cruise_speed;
        double minutesFlightPerCharge = 60 * hoursFlightPerCharge;
        aircraft.emplace_back(models[aircraftType], aircraftId,
                                minutesFlightPerCharge);
    }
    return std::move(aircraft);
}


void Simulator::initChargerWaitingQueue(
        WaitingQueue_T &waitingQueue) {
    // this->_aircrafts.reserve(NUM_ARCRAFT);
    // choose 20 aircraft of random model id in range 0 - 4, inclusive. Add an
    // instance of that to _aircrafts.
    for (int i = 0; i < NUM_AIRCRAFT; ++i) {
        // note - i will become the aircraftId.
        int aircraftId = i;
//        auto aircraftType = pick_random_model();
//        double milesFlightPerCharge = models[aircraftType].battery_life / models[aircraftType].cruise_power_draw;
//        double minutesFlightPerCharge = 60 * milesFlightPerCharge / models[aircraftType].cruise_power_draw;
//        _aircrafts.emplace_back(models[aircraftType], aircraftId,
//                                minutesFlightPerCharge);

        Stamp_t flightEndTime = _aircrafts[i].flightTimePerCharge();
        waitingQueue.push(
            {flightEndTime, ILLEGAL_TIME, ILLEGAL_TIME, aircraftId});
    }
}

// std::priority_queue<ChargingEvent, std::vector<ChargingEvent>,
// std::greater<ChargingEvent>> waiting_queue;
void Simulator::simulateUntil(Stamp_t endTime) {
    WaitingQueue_T waiting_queue;
    ActiveChagingQueue_t active_queue;
    Stamp_t curTime = 0;
    // Stamp_t endTime = 3 * 60; // 3 hours in munits.
    this->_aircrafts = generateTestAircraft(NUM_AIRCRAFT);
    initChargerWaitingQueue(waiting_queue);

    // loop over time. When an aircraft has been charged, it gets removed from
    // active queue, the next next flight miles arc cal'd and updated hen added
    // to waiting queue time for time after flight
    Stamp_t lowestTime = ILLEGAL_TIME;
    int lowestIndex;
    while (curTime < endTime) {
        // update charging queues 1 aircraft at a time. TODO optimize.

        auto waitingEvent = waiting_queue.top();
        waiting_queue.pop();

        // if actively charging is full, them pop out the one which is next to finish
        // ADAM_DEBUG
        if (!active_queue.empty()) {
            std::cout << "aq.top. carge complete eta: " << active_queue.top().chargingCompletionTime << std::endl;
        }

        // if there are aircraft that need to leave a charger do so now. Also, if all charegers occupied skip to time where next one leaves.
        while (active_queue.size() == 3 || !active_queue.empty() && active_queue.top().chargingCompletionTime <= curTime) {
            // first remove top() of actively charging if all chargers busy.
            // Advance clock to match when active.top() will stop charging.
            auto outgoingEvent = active_queue.top();
            active_queue.pop();
            curTime = std::max(curTime, outgoingEvent.chargingCompletionTime);

            // determine next time it needs to charge and put it back
            // into waiting.
            Stamp_t nextChargeTime =
                _aircrafts[outgoingEvent.aircraftId].update_telemetry(
                    curTime, outgoingEvent);
//            nextChargeTime = std::max(nextChargeTime, endTime);
            waiting_queue.push({nextChargeTime, ILLEGAL_TIME, ILLEGAL_TIME,
                                outgoingEvent.aircraftId});
        }
        curTime = std::max(curTime, waitingEvent.startWaitingTime);
        int chargeTime = _aircrafts[waitingEvent.aircraftId].batteryChargeTimeMinutes();
        waitingEvent.startChargingTime = curTime;
        waitingEvent.chargingCompletionTime = curTime + chargeTime;
        active_queue.push(waitingEvent);
    }
}

/**
 * Generate CSV output file "results.csv"
 * @return
 */
Report Simulator::generate_report() const {
    // at the end of 3 hours all aircraft except the actively charging ones will
    // need to updatge their final flight miles. It will be a partial (not full
    // battery charge).

    // std::map<ModelId_t, Report> modelReports;
    // for (auto &aircraft : _aircrafts) {
    //     modelReports[aircraft.modelId()].add_telemetry(aircraft, true);
    // }

    // // calc avs for each model
    // for (auto& model : modelTelemetry) {
    //     model.second.total_charging_time /= NUM_ARCRAFT;
    //     model.second.total_faults /= NUM_ARCRAFT;
    //     model.second.total_flight_hours /= NUM_ARCRAFT;
    //     model.second.total_miles_traveled /= NUM_ARCRAFT;
    // }

    std::ofstream out("results.csv");
    if (!out) {
        throw std::runtime_error("Unable to create report results.csv");
    }

    // table header
    out << "Model,avg-flight-time,avg-distance-per-flight,avs-time-chaging-per-charge-session,total-faults,total-passenger-miles\n";

    // generate 1 row (line) per model.


    return Report();
}
