CREATE TABLE AircraftModels (
    modelId INTEGER PRIMARY KEY,
    cruiseSpeed INTEGER NOT NULL,
    batteryLife INTEGER NOT NULL,
    timeToCharge REAL NOT NULL,
    cruisePowerDraw REAL NOT NULL,
    passengerCount INTEGER NOT NULL,
    faultsPerHour REAL NOT NULL,
    typeName TEXT NOT NULL
);
