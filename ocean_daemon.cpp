// ocean_daemon.cpp

#include <iostream>
#include <fstream>
#include <cmath>
#include <chrono>
#include <thread>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
const std::string STATE_FILE = "ship_state.json";
const int UPDATE_INTERVAL = 5; // seconds
const double EARTH_RADIUS_KM = 6371.0;

json load_state() {
    std::ifstream in(STATE_FILE);
    if (in.is_open()) {
        json j;
        in >> j;
        return j;
    }

    return json{
        // Starting point is just off Key West
        {"latitude", 24.528197830323617},
        {"longitude", -81.80947159236209},
        {"speed_kmh", 20.0},
        {"heading_deg", 90.0}
    };
}

void save_state(const json& j) {
    std::ofstream out(STATE_FILE);
    out << j.dump(4);
}

void update_position(json& state) {
    double lat = state["latitude"];
    double lon = state["longitude"];
    double speed = state["speed_kmh"];
    double heading = state["heading_deg"];
    double distance_km = speed * UPDATE_INTERVAL / 3600.0;
    double heading_rad = heading * M_PI / 180.0;
    double lat_rad = lat * M_PI / 180.0;
    double lon_rad = lon * M_PI / 180.0;
    double new_lat_rad = asin(sin(lat_rad) * cos(distance_km / EARTH_RADIUS_KM) + cos(lat_rad) * sin(distance_km / EARTH_RADIUS_KM) * cos(heading_rad));
    double new_lon_rad = lon_rad + atan2(sin(heading_rad) * sin(distance_km / EARTH_RADIUS_KM) * cos(lat_rad),cos(distance_km / EARTH_RADIUS_KM) - sin(lat_rad) * sin(new_lat_rad));
    double new_lat = new_lat_rad * 180.0 / M_PI;
    double new_lon = fmod((new_lon_rad * 180.0 / M_PI) + 540.0, 360.0) - 180.0;
    state["latitude"] = new_lat;
    state["longitude"] = new_lon;
}

int main() {

   while (true) {
    json state = load_state();
    update_position(state);
    save_state(state);//
    std::this_thread::sleep_for(std::chrono::seconds(UPDATE_INTERVAL));
    }

    return 0;
}