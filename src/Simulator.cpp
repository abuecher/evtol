#include "Simulator.h"

#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>

const int NUM_MODELS = 5;
const int NUM_CHARGERS = 3;
const int HARDCODED_SEED = 1;
const char* REPORT_FILENAME = "results.csv";


static const AirCraftModel models[] = {{ALPHA, 120, 320, 0.6, 1.6, 4, 0.25, "ALPHA"},
                                       {BRAVO, 100, 100, 0.2, 1.5, 5, 0.10, "BRAVO"},
                                       {CHARLIE, 160, 220, 0.8, 2.2, 3, 0.05, "CHARLIE"},
                                       {DELTA, 90, 120, 0.62, 0.8, 2, 0.22, "DELTA"},
                                       {ECHO, 30, 150, 0.3, 5.8, 2, 0.61, "ECHO"},
                                       // add new ones here.
                                       {NONE, 0, 0, 0.0, 0.0, 0, 0.0, "(NULL ENTRY)"}};


ModelId_t Simulator::pickRandomModel() const {
    int n = rand() % NUM_MODELS;
    ModelId_t model = static_cast<ModelId_t>(n);
    //    assert(model < NONE);
    return model;
}

std::vector<AircraftInstance> Simulator::generateTestAircraft(int numAircraft) const {
    std::vector<AircraftInstance> aircraft;
    for (int i = 0; i < numAircraft; ++i) {
        int aircraftId = i;
        auto aircraftType = pickRandomModel();
        double milesFlightPerCharge =
            models[aircraftType].batteryLife / models[aircraftType].cruisePowerDraw;
        double hoursFlightPerCharge = milesFlightPerCharge / models[aircraftType].cruiseSpeed;
        double minutesFlightPerCharge = 60 * hoursFlightPerCharge;
        aircraft.emplace_back(models[aircraftType], aircraftId, minutesFlightPerCharge);
    }
    return std::move(aircraft);
}


void Simulator::initChargerWaitingQueue(WaitingQueue_T& waitingQueue, Stamp_t simDuration) {
    // choose 20 aircraft of random model id in range 0 - 4, inclusive. Add an
    // instance of that to _aircrafts.
    for (int i = 0; i < _aircrafts.size(); ++i) {
        // note - i will become the aircraftId.
        int aircraftId = i;

        Stamp_t flightEndTime = _aircrafts[i].flightTimePerCharge();
        flightEndTime = std::min(flightEndTime, simDuration);
        // record the first leg of flight. After this, telemetry is recorded when they enter/leave
        // charging station.
        _aircrafts[i].updateTelemetryForFlight(flightEndTime, simDuration);
        waitingQueue.push({flightEndTime, ILLEGAL_TIME, ILLEGAL_TIME, aircraftId});
    }
}

// std::priority_queue<ChargingEvent, std::vector<ChargingEvent>,
// std::greater<ChargingEvent>> waiting_queue;
void Simulator::simulateUntil(int numAircraft, Stamp_t endTime) {
    std::cout << "started sim until " << endTime << " minutes" << std::endl;
    WaitingQueue_T waiting_queue;
    ActiveChagingQueue_t active_queue;
    Stamp_t curTime = 0;
    // Stamp_t endTime = 3 * 60; // 3 hours in munits.
    this->_aircrafts = generateTestAircraft(numAircraft);
    initChargerWaitingQueue(waiting_queue, endTime);

    // loop over time. When an aircraft has been charged, it gets removed from
    // active queue, the next flight miles arc cal'd and updated hen added
    // to waiting queue time for time after flight
    Stamp_t lowestTime = ILLEGAL_TIME;
    int lowestIndex;
    while (curTime < endTime) {
        // update charging queues 1 aircraft at a time, more or less (can pop mulitple items off
        // active_queue)

        auto waitingEvent = waiting_queue.top();
        waiting_queue.pop();

        // if there are aircraft that need to leave a charger do so now. Also, if all charegers
        // occupied skip to time where next one leaves.
        while (active_queue.size() == 3 ||
               !active_queue.empty() && active_queue.top().chargingCompletionTime <= curTime) {
            // first remove top() of actively charging if all chargers busy.
            // Advance clock to match when active.top() will stop charging.
            auto outgoingEvent = active_queue.top();
            active_queue.pop();
            curTime = std::max(curTime, outgoingEvent.chargingCompletionTime);
            curTime = std::min(curTime, endTime);
            // determine next time it needs to charge and put it back
            // into waiting.
            std::cout << "processing curTime = " << curTime << std::endl;
            Stamp_t nextChargeTime =
                _aircrafts[outgoingEvent.aircraftId].updateTelemetryForCharging(curTime,
                                                                                outgoingEvent);
            nextChargeTime = std::min(nextChargeTime, endTime);
            waiting_queue.push(
                {nextChargeTime, ILLEGAL_TIME, ILLEGAL_TIME, outgoingEvent.aircraftId});
            Stamp_t nextTime = _aircrafts[outgoingEvent.aircraftId].updateTelemetryForFlight(
                curTime, nextChargeTime);
        }
        curTime = std::max(curTime, waitingEvent.startWaitingTime);
        int chargeTime = _aircrafts[waitingEvent.aircraftId].batteryChargeTimeMinutes();
        waitingEvent.startChargingTime = curTime;
        waitingEvent.chargingCompletionTime = curTime + chargeTime;
        active_queue.push(waitingEvent);
    }

    // finish up add charging times for the items still on chargers.
    while (!active_queue.empty()) {
        auto chargingItem = active_queue.top();
        active_queue.pop();
        _aircrafts[chargingItem.aircraftId].updateTelemetryForCharging(curTime, chargingItem);
    }
    // update charging times for all that were waiting for chargers that would not still be in the
    // air at end time (if still in air the previous flight update will include their time).
    while (!waiting_queue.empty() && waiting_queue.top().startChargingTime < endTime) {
        auto waitingToCharge = waiting_queue.top();
        waiting_queue.pop();
        _aircrafts[waitingToCharge.aircraftId].updateTelemetryForCharging(curTime, waitingToCharge);
    }
}

/**
 * Generate CSV output file "results.csv"
 * @return
 */
Report Simulator::generateSimulationReport() const {
    Report report;
    for (auto& aircraft : _aircrafts) {
        report.addAircraftTelemetry(aircraft.aircraftId(), aircraft.modelName(),
                                    aircraft.getTelemetry());
    }

    return report;
}

void Simulator::writeReport(const Report& report) const {
#ifdef USE_DEBUG_INFO
    report.writeReport(std::cout);
#endif
    std::cout << std::endl;
    std::cout << "saving report as CSV to " << REPORT_FILENAME << std::endl;
    std::ofstream out(REPORT_FILENAME);
    if (!out) {
        throw std::runtime_error("Unable to create report file");
    }
    report.writeReport(out, ',');
    out.close();
}
