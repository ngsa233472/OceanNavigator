// shipctl.cpp

#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
const std::string STATE_FILE = "ship_state.json";

json load_state() {
    std::ifstream in(STATE_FILE);
    if (!in.is_open()) {
        std::cerr << "State file not found.\n";
        exit(1);
    }
    json j;
    in >> j;
    return j;
}

void save_state(const json& j) {
    std::ofstream out(STATE_FILE);
    out << j.dump(4);
}

void status() {
    json state = load_state();
    std::cout << "Latitude:  " << state["latitude"] << "\n";
    std::cout << "Longitude: " << state["longitude"] << "\n";
    std::cout << "Speed:     " << state["speed_kmh"] << " km/h\n";
    std::cout << "Heading:   " << state["heading_deg"] << "°\n";
}

void set_speed(double speed) {
    json state = load_state();
    state["speed_kmh"] = speed;
    save_state(state);
    std::cout << "Speed updated to " << speed << " km/h.\n";
}

void set_heading(double heading) {
    json state = load_state();
    state["heading_deg"] = fmod(heading, 360.0);
    save_state(state);
    std::cout << "Heading updated to " << heading << "°.\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: shipctl status | set-speed <kmh> | set-heading <deg>\n";
        return 1;
    }

    std::string cmd = argv[1];

    if (cmd == "status") {
        status();
    } else if (cmd == "set-speed" && argc == 3) {
        set_speed(std::stod(argv[2]));
    } else if (cmd == "set-heading" && argc == 3) {
        set_heading(std::stod(argv[2]));
    } else {
        std::cerr << "Invalid command or arguments.\n";
    }
    return 0;
}