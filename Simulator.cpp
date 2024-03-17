//
// Created by adam on 3/16/24.
//

#include "Simulator.h"
#include <random>
#include <array>
#include <queue>

std::string Report::toString() const {
    return "ADAM_DEBUG placeholder";
}

static AirCraftModel models[] = {
    {ALPHA, 120, 320, 0.6, 1.6, 4, 0.25, "ALPHA"},
    {BRAVO, 120, 320, 0.6, 1.6, 4, 0.25, "BRAVO"},
    {CHARLIE, 120, 320, 0.6, 1.6, 4, 0.25, "CHARLIE"},
    {DELTA, 120, 320, 0.6, 1.6, 4, 0.25, "DELTA"},
    {ECHO, 120, 320, 0.6, 1.6, 4, 0.25, "ECHO"},
    // add new ones here.
    {NONE, 0, 0, 0.0, 0.0, 0, 0.0, "(NULL ENTRY)"}
};

void Simulator::init_aircrafts() {
    // choose 20 aircraft of random model id in range 0 - 4, inclusive. Add an instance of that to _aircrafts.
}

struct ChargingEvent {
    Stamp_t startWaitingTime;
    Stamp_t startChargingTime;
    Stamp_t chargingCompletionTime;//
    int aircraftId;
//    AircraftInstance& aircraft; // do I need to paralleize? if so use int. If not can suie AircraftInstance& and eliminate a redirect.
};
void Simulator::run_simulation() {
    std::priority_queue<ChargingEvent> waiting_queue; //TODO need a comparator to invert to low-pq since c++ is backassward here.
    std::array<ChargingEvent, 3> active_queue;
}

Report Simulator::generate_report() const {
    return Report();
}
