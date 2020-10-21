//Code is written using C++

#include<bits/stdc++.h>
using namespace std;

// Car class used to represent every car that is present in the lot
//ParkingLot class used to represent the given parking lot

class Car{
    string vehicle_reg_number;
    int age;
    public:
        Car(int d_age, string reg_num){
            vehicle_reg_number=reg_num;
            age=d_age;
        }
        string get_reg_num(){
            return vehicle_reg_number;
        }
        int get_age(){
            return age;
        }
};

class ParkingLot{
    int slots;  //number of slots available in parking lot
    set<int> available_slots;   //list of available slots
    //Hashmaps have been used extensively to perform each operation in constant time
    unordered_map<int,list<string > >age_query; // given age find vrn of car
    unordered_map<int,string> car_parked;   //mapping slot with VRN
    unordered_map<string,pair<int,list<string>::iterator > > alloted_slot;  //mapping vrn with slot and age query
    unordered_map<string,int> Cars; //given vrn find slot of the car

    public:
    ParkingLot(int n){
        slots=n;
        for(int i=1;i<=slots;i++){
            available_slots.insert(i);  //populating available slots
        }
    }

    //function to park the car
    string park_a_car(string reg_num,int age){
        //Parking lot full
        if(available_slots.size()<=0)
            return "Parking lot full";
        // Adding car to the parking lot
        int slot=*(available_slots.begin());
        available_slots.erase(slot);
        Cars[reg_num]=age;
        age_query[age].push_back(reg_num);
        car_parked[slot]=reg_num;
        alloted_slot[reg_num]=make_pair(slot,--(age_query[age].end()));
        return "Car with vehicle registration number '" + reg_num + "' has been parked at slot number " + to_string(slot);
    }
    
    //function for removing a car from the lot
    string leaving_the_lot(int slot){
        //Already empty slot
        if(available_slots.find(slot)!=available_slots.end()){
            return "Slot already empty";
        }
        //Removing car from the lot
        available_slots.insert(slot);
        string car = car_parked[slot];
        auto p = alloted_slot[car].second;
        alloted_slot.erase(car);
        age_query[Cars[car]].erase(p);
        car_parked.erase(slot);
        string vrn = car;
        string age = to_string(Cars[car]);
        Cars.erase(car);

        return "Slot number " + to_string(slot) + " vacated, the car with vehicle registration number '" + vrn + "' left the space, the driver of the car was of age " + age;

    }
    
    //All vrn of cars with drivers of given age
    vector<string> get_vehicles_for_age(int age){
        vector<string> ans;
        if(age_query.find(age)==age_query.end())return ans;
        for(auto it=age_query[age].begin();it!=age_query[age].end();it++){
            ans.push_back((*it));
        }
        return ans;
    }
    
    //All slots of cars with drivers of given age
    vector<int> get_slots_for_age(int age){
        vector<int> ans;
        if(age_query.find(age)==age_query.end())return ans;
        for(auto it=age_query[age].begin();it!=age_query[age].end();it++){
            ans.push_back(alloted_slot[(*it)].first);
        }
        return ans;
    }

    //Slot for given vrn
    int get_slot_for_vrn(string vrn){
        int ans;
        if(alloted_slot.find(vrn)==alloted_slot.end())return ans;
        ans = alloted_slot[vrn].first;
        return ans;
    }
};

int main(){
    string filename;
    cout<<"Enter the full file path here : ";
    cin>>filename;
    
    fstream read_file;
    read_file.open(filename, ios::in);
    string command;
    ParkingLot *parking_lot;
    while(getline(read_file, command)){ //command has each input string
        string each_command;            //each_command for every term
        stringstream iss(command);
        while(iss>>each_command){
            if(each_command == "Create_parking_lot"){
                    iss>>each_command;
                    ParkingLot *p = new ParkingLot(stoi(each_command));
                    parking_lot=(p);
                    cout<<"Created parking of "+each_command+" slots"<<endl;
                }
            
            else if(each_command == "Park"){
                    iss>>each_command;
                    string vrn = each_command;
                    iss>>each_command;
                    iss>>each_command;
                    int driver_age = stoi(each_command);
                    cout<<parking_lot->park_a_car(vrn, driver_age)<<endl;
                }
            
            else if(each_command == "Slot_numbers_for_driver_of_age"){
                    iss>>each_command;
                    vector<int> slots = parking_lot->get_slots_for_age(stoi(each_command));
                    if(slots.size()==0){ //No such car exists
                        cout<<"No such car in the lot"<<endl;
                        continue;
                    }
                    for(int i = 0;i<slots.size();i++){
                        cout<<slots[i];
                        if(i!=slots.size()-1)
                            cout<<",";
                    }
                    cout<<endl;
                }

            else if(each_command == "Slot_number_for_car_with_number"){
                    iss>>each_command;
                    int space = parking_lot->get_slot_for_vrn(each_command);
                    if(space==NULL) cout<<"No such car in the lot"<<endl;
                    cout<<space<<endl;
            }

            else if(each_command == "Leave"){
                    iss>>each_command;
                    int space_to_empty = stoi(each_command);
                    cout<<parking_lot->leaving_the_lot(space_to_empty)<<endl;
                }

            else if(each_command == "Vehicle_registration_number_for_driver_of_age"){
                    iss>>each_command;
                    vector<string> vrns = parking_lot->get_vehicles_for_age(stoi(each_command));
                    if(vrns.size()==0){ //No such car exists
                        cout<<"No such car in the lot"<<endl;
                        continue;
                    }
                    for(int i = 0;i<vrns.size();i++){
                        cout<<vrns[i];
                        if(i!=vrns.size()-1)
                            cout<<",";
                    }
                    cout<<endl;
                }
            else
                cout<<"Case not mentioned"<<endl;
            }
       }
    read_file.close();
    return 0;
}
