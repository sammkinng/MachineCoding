#include <bits/stdc++.h>

using namespace std;

enum VehicleType
{
    CAR,
    BIKE,
    TRUCK
};

unordered_map<string, VehicleType> map_vehicle_type;

class Vehicle
{
    VehicleType type;
    string registrationNumber;
    string color;

public:
    Vehicle(VehicleType t,
            string r,
            string c) : registrationNumber(r), color(c), type(t) {}

    VehicleType getType() const { return type; }

    string getRegistrationNumber() const { return registrationNumber; }

    string getColor() const { return color; }
};

class Ticket
{
    string id;
    int floor_no;
    int slot_no;
    Vehicle vehicle;

public:
    Ticket(string i,
           int f,
           int s, Vehicle v) : id(i), floor_no(f), slot_no(s), vehicle(v)
    {
    }
    string getId() const { return id + "_" + to_string(floor_no) + "_" + to_string(slot_no); }

    Vehicle getVehicle() const { return vehicle; }

    int floorNo() const { return floor_no; }

    int slotNo() const { return slot_no; }
};

class ParkingSlot
{
    long id;
    string ticketId;
    bool occupied;
    int floorId;
    VehicleType vehicleType;

public:
    ParkingSlot(int f,
                VehicleType v, long i) : floorId(f), vehicleType(v), id(i)
    {
    }

    string getTicketId() const { return ticketId; }
    void setTicketId(const string &ticketId_) { ticketId = ticketId_; }

    bool getOccupied() const { return occupied; }
    void setOccupied(bool occupied_) { occupied = occupied_; }

    VehicleType getVehicleType() const { return vehicleType; }

    long getId() const { return id; }

    int getFloorId() const { return floorId; }
};

class Floor
{
    int id;
    vector<ParkingSlot *> slots;

public:
    Floor(int id_, int number_of_slots) : id(id_)
    {
        if (number_of_slots < 1)
            throw invalid_argument("Number of slots should be at least 1.");
        slots.push_back(new ParkingSlot(id_, TRUCK, 1));

        if (number_of_slots > 1)
            slots.push_back(new ParkingSlot(id_, BIKE, 2));
        if (number_of_slots > 2)
            slots.push_back(new ParkingSlot(id_, BIKE, 3));
        int count = 4;
        while (number_of_slots > 3)
        {
            slots.push_back(new ParkingSlot(id_, CAR, count));
            number_of_slots--;
            count++;
        }
    }

    vector<ParkingSlot *> getSlots() const { return slots; }

    vector<ParkingSlot *> getFreeSlots(VehicleType type) const
    {
        vector<ParkingSlot *> res;
        for (auto &slot : slots)
            if (!slot->getOccupied() && slot->getVehicleType() == type)
                res.push_back(slot);

        return res;
    }

    vector<ParkingSlot *> getOccupiedSlots(VehicleType type) const
    {
        vector<ParkingSlot *> res;
        for (auto &slot : slots)
            if (slot->getOccupied() && slot->getVehicleType() == type)
                res.push_back(slot);

        return res;
    }

    int countFreeSlots(VehicleType type) const
    {
        int res = 0;
        for (auto &slot : slots)
            if (!slot->getOccupied() && slot->getVehicleType() == type)
                res++;
        return res;
    }

    ParkingSlot *getFirstFreeSlot(VehicleType type)
    {
        for (auto &slot : slots)
            if (!slot->getOccupied() && slot->getVehicleType() == type)
                return slot;
        return NULL;
    }

    int getId() const { return id; }
};

class ParkingLot
{
    string id;
    static ParkingLot *instance;
    vector<Floor *> floorList;
    unordered_map<string,Ticket*>tickets;

    ParkingLot(int fs, int ns, string i) : id(i)
    {
        int id_ = 1;
        while (fs)
        {
            floorList.push_back(new Floor(id_, ns));
            id_++;
            fs--;
        }
    }

public:
    static ParkingLot *getInstance(int fs, int ns, string i)
    {
        if (instance == nullptr)
        {
            instance = new ParkingLot(fs, ns, i);
        }
        return instance;
    }

    vector<Floor *> getFloorList() const { return floorList; }

    string getId() const { return id; }

    void addNewTicket(Ticket *tk)
    {
        tickets.insert({tk->getId(),tk});
    }

    void removeTicket(string id){
        auto it = tickets.find(id);
        
        if (it != tickets.end())
        {
            int f = it->second->floorNo();
            int s = it->second->slotNo();
            instance->getFloorList()[f-1]->getSlots()[s-1]->setOccupied(false);
            cout << "Unparked vehicle with Registration Number: " << it->second->getVehicle().getRegistrationNumber() << " and Color: " << it->second->getVehicle().getColor() << endl;
            
            tickets.erase(it);
        }
        else
        {
            cout << "Invalid Ticket" << endl;
        }
    }


    unordered_map<string,Ticket *> getTickets() const { return tickets; }
};

class CommandPanel
{
    ParkingLot *lot;

public:
    CommandPanel()
    {
        map_vehicle_type.insert({"CAR", CAR});
        map_vehicle_type.insert({"BIKE", BIKE});
        map_vehicle_type.insert({"TRUCK", TRUCK});
    }

    void create_parking_lot(stringstream &ss)
    {
        string s1;
        int x, y;
        ss >> s1 >> x >> y;
        lot = ParkingLot::getInstance(x, y, s1);
        cout << "Created parking lot with " << x << " floors and " << y << " slots per floor" << endl;
    }

    void free_slots(int type, string s)
    {
        if (map_vehicle_type.find(s) != map_vehicle_type.end())
        {
            for (auto &floor : lot->getFloorList())
            {
                if (type == 0)
                    cout << "No. of free slots for " << s << " on Floor " << floor->getId() << ": " << floor->countFreeSlots(map_vehicle_type[s]) << endl;
                else if (type == 1)
                {
                    cout << "Free slots for " << s << " on Floor " << floor->getId() << ": ";
                    for (auto &slot : floor->getFreeSlots(map_vehicle_type[s]))
                        cout << slot->getId() << " ";
                    cout << endl;
                }
                else
                {
                    cout << "Occupied slots for " << s << " on Floor " << floor->getId() << ": ";
                    for (auto &slot : floor->getOccupiedSlots(map_vehicle_type[s]))
                        cout << slot->getId() << " ";
                    cout << endl;
                }
            }
        }
        else
            throw invalid_argument("Invalid Vehicle type.");
    }
    void display(stringstream &ss)
    {
        string s1, s2;
        ss >> s1 >> s2;

        if (s1 == "free_count")
            free_slots(0, s2);
        else if (s1 == "free_slots")
            free_slots(1, s2);
        else if (s1 == "occupied_slots")
            free_slots(2, s2);
        else
            throw invalid_argument("Display command has inavlid argument.");
    }

    void take_input()
    {
        string s;

        while (1)
        {
            getline(cin, s);
            if (s == "exit")
                break;
            else
            {
                stringstream ss(s);
                string s1;
                ss >> s1;

                if (s1 == "create_parking_lot")
                    create_parking_lot(ss);
                else if (s1 == "display")
                    display(ss);
                else if (s1 == "park_vehicle")
                    park(ss);
                else if (s1 == "unpark_vehicle")
                    unpark(ss);
                else
                    throw invalid_argument("Invalid input");
            }
        }
    }

    void park(stringstream &ss)
    {
        string s1, s2, s3;
        ss >> s1 >> s2 >> s3;

        Vehicle vehicle(map_vehicle_type[s1], s2, s3);

        if (map_vehicle_type.find(s1) != map_vehicle_type.end())
        {
            ParkingSlot *slot = NULL;
            for (auto &floor : lot->getFloorList())
            {
                slot = floor->getFirstFreeSlot(map_vehicle_type[s1]);
                if (slot != NULL)
                    break;
            }
            if (slot != NULL)
            {
                slot->setOccupied(true);

                Ticket* ticket=new Ticket(lot->getId(), slot->getFloorId(), slot->getId(), vehicle);
                string tid = ticket->getId();
                slot->setTicketId(tid);
                lot->addNewTicket(ticket);
                cout << "Parked vehicle. Ticket ID: " << tid << endl;
            }
            else
                cout << "Parking Lot Full" << endl;
        }
        else
            throw invalid_argument("Invalid Vehicle type.");
    }
    void unpark(stringstream &ss)
    {
        string s1;
        ss>>s1;
        lot->removeTicket(s1);
    }
};

ParkingLot *ParkingLot::instance = nullptr;

int main()
{
    freopen("./input.txt", "r", stdin);
    freopen("./output.txt", "w", stdout);

    CommandPanel panel;
    panel.take_input();
    return 0;
}
