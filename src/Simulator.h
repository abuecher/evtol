//
// Created by adam on 3/16/24.
//

#ifndef EVTOL_SIMULATOR_H
#define EVTOL_SIMULATOR_H

// #include <random>
#include <queue>
#include <string>
#include <vector>

#include "src/Aircraft.h"
#include "src/Report.h"

// Comparison operator for the waiting queue
struct WaitingChargingEventComparator {
    bool operator()(const ChargingEvent &a, const ChargingEvent &b) const {
        // Prioritize events with earlier startWaitingTime
        return a.startWaitingTime > b.startWaitingTime;
    }
};

// Comparison operator for the active queue (the ones on the chargers)
struct ActiveChargingEventComparator {
    bool operator()(const ChargingEvent &a, const ChargingEvent &b) const {
        // Prioritize events with earlier startWaitingTime
        return a.chargingCompletionTime > b.chargingCompletionTime;
    }
};

using WaitingQueue_T =
    std::priority_queue<ChargingEvent, std::deque<ChargingEvent>,
                        WaitingChargingEventComparator>;

using ActiveChagingQueue_t =
    std::priority_queue<ChargingEvent, std::deque<ChargingEvent>,
                        ActiveChargingEventComparator>;



class SimulatorTest; // fed decl for testing.

class Simulator {

  private:
    std::vector<AircraftInstance> _aircrafts;
    // long _seed; // for simplicity sake using srand erather than <random>

    ModelId_t pick_random_model() const;
    // bool check_fault(const AircraftInstance &aircraft,
    //                  Stamp_t time_span) const;

  public:
    /**
     * Populate _aircrqafts w/ 20 random aircraft and populate waitingQAueue
     * with them
     */
    std::vector<AircraftInstance> generateTestAircraft(int numAircraft) const;
    void initChargerWaitingQueue(WaitingQueue_T &waitingQueue);
    void simulateUntil(Stamp_t endTime);
    Report generate_report() const; // generate report for all aircraft
};

#endif // EVTOL_SIMULATOR_H
