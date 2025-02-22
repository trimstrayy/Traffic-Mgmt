// TrafficManager.h
#pragma once
#include <vector>
#include <memory>
#include <map>
#include "core/Constants.h"
#include "core/Lane.h"
#include "core/TrafficLight.h"
#include "core/Vehicle.h"

struct VehicleState {
    std::shared_ptr<Vehicle> vehicle;
    float x, y;             // Current position
    float targetX, targetY; // Target position
    float speed;            // Movement speed
    bool isMoving;
    Direction direction;
    bool hasStartedTurn;
    float turnProgress;
    float waitTime;
    float turnAngle;        // Current turning angle
    float targetAngle;      // Final angle after turn
};

class TrafficManager {
private:
    std::vector<std::unique_ptr<Lane>> lanes;
    std::map<LaneId, TrafficLight> trafficLights;
    std::map<uint32_t, VehicleState> activeVehicles;
    bool inPriorityMode;
    float stateTimer;
    float lastUpdateTime;

    // Vehicle processing methods
    void updateVehiclePositions(float deltaTime);
    void updateVehicleQueuePosition(VehicleState& state, LaneId laneId, size_t queuePosition);
    void calculateTargetPosition(VehicleState& state, LaneId laneId);
    bool checkCollision(const VehicleState& state, float newX, float newY) const;
    float calculateTurningRadius(Direction dir) const;

    // Traffic control methods
    void updateTrafficLights(float deltaTime);
    void synchronizeTrafficLights();
    bool checkPriorityConditions() const;
    void processPriorityLane();
    void processNormalLanes(size_t vehicleCount);
    size_t calculateVehiclesToProcess() const;

public:
    TrafficManager();
    void update(float deltaTime);
    void addVehicleToLane(LaneId laneId, std::shared_ptr<Vehicle> vehicle);
    void addNewVehicleToState(std::shared_ptr<Vehicle> vehicle, LaneId laneId);
    size_t getLaneSize(LaneId laneId) const;

    // Getters for rendering
    bool isInPriorityMode() const { return inPriorityMode; }
    const std::vector<std::unique_ptr<Lane>>& getLanes() const { return lanes; }
    const std::map<LaneId, TrafficLight>& getTrafficLights() const { return trafficLights; }
    const std::map<uint32_t, VehicleState>& getActiveVehicles() const { return activeVehicles; }
};