# evtol problem


## Tenets
- Use good OOP design
- Must be coded in C++
- Must include unit tests (only a few different types of unit tests needed, does not need full coverage) - I should also include some stuff like static analyzers, and threading tests, etc
- TODO and NOTE are OK for things that might take too long to properly implement.

# Concepts

The table provided can help determine what components I need to model:

```
|----------------------|-------|-------|---------|-------|------|
| Company Name         | Alpha | Bravo | Charlie | Delta | Echo |
|----------------------|-------|-------|---------|-------|------|
| Cruise Speed (MPH)   | 120   | 100   | 160     | 90    | 30   | 
|----------------------|-------|-------|---------|-------|------|
| Battery (kWh)        | 320   | 100   | 220     | 120   | 150  |
|----------------------|-------|-------|---------|-------|------|
| Time to Charge (hrs) | 0.6   | 0.2   | 0.8     | 0.62  | 0.3  |
|----------------------|-------|-------|---------|-------|------|
| Energy use at Cruise | 1.6   | 1.5   | 2.2     | 0.8   | 5.8  |
| (kWh/mile)           |       |       |         |       |      |
|----------------------|-------|-------|---------|-------|------|
| Passenger Count      | 4     | 5     | 3       | 2     | 2    |
|----------------------|-------|-------|---------|-------|------|
| Probability of       | 0.25  | 0.10  | 0.05    | 0.22  | 0.61 |
| fault per hour       |       |       |         |       |      |
|                      |       |       |         |       |      |
|                      |       |       |         |       |      |
|                      |       |       |         |       |      |
|----------------------|-------|-------|---------|-------|------|
```
# Design Ideas

Decomposing what is needed based on above data table:
Things on or about Aircraft:
- Aircraft - wraps all other components. Instances of this are what are measured.
- Battery - stats: charging_rate, total_charge
- Engine - cruiseSpeed, battery_drain,
- Cabin - Where the passengers are. Limited utility. Should I just make this a single int in the Aircraft class


External to the aircraft
- Charger - Limited resource. Have a Mutex or somesuch.
- Clock. Do I have an actual ticking clock or do I have some centralized event queue set current time to the next event trigger time (track the time based on that).
  - Seems pretty obvious once modeling the table data as a struct-per-model that I will just simulate clock with events, No need to build a ticking clock.


# Deliverables
From the word doc:

You will simulate using these vehicle for 3 hours. Of course your simulation should take much less time than that. 20 total vehicles should be deployed, and a random number of each type of vehicle should be used (with the total between all five types being 20).

There are only three chargers available for all 20 vehicles! A single charger can only be used by one vehicle at a time. Assume the chargers can charge the batteries in the Time to Charge time listed for each vehicle.

Keep track of the following statistics per vehicle type:
- average flight time per flight
- average distance traveled per flight
- average time charging per charge session
- total number of faults
- total number of passenger miles.
  For example, if there are 2 vehicles carrying 4 passengers on a vehicle that cruises    for 1 hour at 100 mph, total number of passenger miles is 2 * 4 * 1 * 100 = 800.

Assume that:
- Each vehicle starts the simulation with a fully-charged battery
- Each vehicle instantaneously reaches Cruise Speed
- Each vehicle is airborne for the full use of the battery, and is immediately in line for the charger after running out of battery power.

Please include the statistics recorded during at least one run of the simulation in your submission.

Please do not hesitate to reach out to ask any questions about the problem! However, if you find yourself making an assumption that we haven’t explained explicitly, you may simply document your assumption. It’s interesting to us to hear what assumptions you made and why.

## questions
- faults. Is there any downtime associated with those? I will assume not and that I should just count them, but need to verify that is correct.


# Implementation Notes

Originally, I was thinking of checking the states of various aircraft and comparing the "current time".

roughly the idea was this though there were some gaps.

```
foreach aircraft:
          if aircraft.state == FLYING
              aircraft.empty_battery()
              aircraft.state = WAITING_TO_CHARGE
              charging_queue.push(Event(eventStartTime, chargingeEndTime, aircraft))
          else if aircraft.state == CHARGING
              aircraft.power = aircraft.maxpower
              chargeEndime = aircraft.telemetry.charge_start + aircraft.charge_duration
              aircraft.telemetry += aircraft.charge_duration
              aircraft.state = FLYING
              active_charging.pop(aircraft.id)
          else  // state == WAITING-TO-CHARGE
              if active_charging.size < 3
                active_charging.push(Event(aircraft)
```

Filling those gaps led me to the much simpler simulation mode that all aircraft are in one of 2 queues: 
waiting-to-charge and actively-charging

waiting-to-charge is a priority queue sorted by charge-wait-start time. lowest is next.
active-charging is a priority queue of up to 3 event objects which store the charge-wait-start, charge-start, charge-end time, and the aircraft id or object ref. 

After the end time has been reached in the main loop, some additional post
processing is performed


# How to run it

Built and tested using Ubuntu-22.04.
dependencies: gtest (cmake should download that during config).

Built and testing using gcc and make. It should work just as well with any combo of those and LLVM and ninja. Works just fine with ninja, but I have not tested clang beyond clang-tidy and clang-format. Not tested on msvc

```bash
rm -rf build
mkdir build && cd build
cmake ..
make
make test

# to see more detail on each test case run use:
build/tests/simulator_tests.

# to run the actual simulation
build/evol
```

Results are written to stdout as well as file results.csv.

Note - My previous commit "Purely Cosmetic" appears to have switched the account name on all of he commits from abuecher to my other account segfaultomatic. Weird.
