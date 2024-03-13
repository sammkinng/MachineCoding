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
    string getId() const { return id + "_" + to_string(floor_no) + to_string(slot_no); }

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
        int count = 3;
        while (number_of_slots > 2)
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
    vector<Ticket *> tickets;

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
        tickets.push_back(tk);
    }

    vector<Ticket *> getTickets() const { return tickets; }
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

    void create_parking_lot(string s, int j)
    {
        int count = 0;
        while (s[j] != ' ')
            j++;
        j++;
        int m = 0;
        while (s[j] != ' ')
        {
            m *= 10;
            m += s[j] - '0';
            j++;
        }
        j++;
        int n = 0;
        while (s[j] != ' ')
        {
            n *= 10;
            n += s[j] - '0';
            j++;
        }
        lot = ParkingLot::getInstance(m, n, "PR1234");
    }

    void free_slots(int type, int j, string s)
    {
        string x = s.substr(j + 1);
        if (map_vehicle_type.find(x) != map_vehicle_type.end())
        {
            for (auto &floor : lot->getFloorList())
            {
                if (type == 0)
                    cout << "No. of free slots for " << x << " on Floor " << floor->getId() << ": " << floor->countFreeSlots(map_vehicle_type[x]);
                else if (type == 1)
                {
                    cout << "Free slots for " << x << " on Floor " << floor->getId() << ": ";
                    for (auto &slot : floor->getFreeSlots(map_vehicle_type[x]))
                        cout << slot->getId() << " ";
                    cout << endl;
                }
                else
                {
                    cout << "Occupied slots for " << x << " on Floor " << floor->getId() << ": ";
                    for (auto &slot : floor->getOccupiedSlots(map_vehicle_type[x]))
                        cout << slot->getId() << " ";
                    cout << endl;
                }
            }
        }
        else
            throw invalid_argument("Invalid Vehicle type.");
    }
    void display(string s, int j)
    {
        int i = j;
        while (s[j] != ' ')
            j++;
        if (s.substr(i, j) == "free_count")
            free_slots(0, j, s);
        else if (s.substr(i + 1, j) == "free_slots")
            free_slots(1, j, s);
        else if (s.substr(i + 1, j) == "occupied_slots")
            free_slots(2, j, s);
        else
            throw invalid_argument("Display command has inavlid argument.");
    }

    void take_input()
    {
        string s;
        getline(cin, s);
        while (1)
        {

            if (s == "exit")
                break;
            else
            {
                int i = 0;
                while (s[i] != ' ')
                    i++;
                if (s.substr(0, i) == "create_parking_lot")
                    create_parking_lot(s, i + 1);
                else if (s.substr(0, i) == "display")
                    display(s, i + 1);
                else if (s.substr(0, i) == "park_vehicle")
                    park(s, i);
                else if (s.substr(0, i) == "unpark_vehicle")
                    unpark(s, i);
                else
                    throw invalid_argument("Invalid input");
            }
        }
    }

    void park(string s, int j)
    {
        int k = j + 1;
        while (s[k] != ' ')
            k++;

        string x = s.substr(j + 1, k);
        int a = ++k;
        while (s[k] != ' ')
            k++;
        string y = s.substr(a, k);

        string z = s.substr(k + 1);

        Vehicle vehicle(map_vehicle_type[x], y, z);

        if (map_vehicle_type.find(x) != map_vehicle_type.end())
        {
            ParkingSlot *slot = NULL;
            for (auto &floor : lot->getFloorList())
            {
                slot = floor->getFirstFreeSlot(map_vehicle_type[x]);
                if (slot != NULL)
                    break;
            }
            if (slot != NULL)
            {
                slot->setOccupied(true);

                Ticket ticket(lot->getId(), slot->getFloorId(), slot->getId(), vehicle);
                string tid = ticket.getId();
                slot->setTicketId(tid);
                lot->addNewTicket(&ticket);
                cout << "Parked vehicle. Ticket ID: " << tid << endl;
            }
            else
                cout << "Parking Lot Full" << endl;
        }
        else
            throw invalid_argument("Invalid Vehicle type.");
    }
    void unpark(string s, int j)
    {
        int k = j + 1;
        while (s[k] != ' ')
            k++;

        string x = s.substr(j + 1, k);
        Ticket *tk = NULL;

        for (auto it = lot->getTickets().begin(); it != lot->getTickets().end(); ++it)
            if ((*it)->getId() == x)
            {
                tk = *it;
                lot->getTickets().erase(it);
                break;
            }

        if (tk == NULL)
            cout << "Invalid Ticket" << endl;
        else
        {
            int f = tk->floorNo();
            int s = tk->slotNo();
            lot->getFloorList()[f]->getSlots()[s]->setOccupied(false);
            cout<<"Unparked vehicle with Registration Number: "<<tk->getVehicle().getRegistrationNumber()<<" and Color: "<<tk->getVehicle().getColor()<<endl;
        }
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
