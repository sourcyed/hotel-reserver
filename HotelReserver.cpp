#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

// RoomType enum declares the types of rooms which are single and double
enum RoomType
{
	Single,
	Double
};


// Room struct stores important information for each room
struct Room
{
	int number;
	RoomType roomType;
	bool booked;
};

struct Reservation
{
	int number;
	string reserverName;
	int reservedNights;
	float discountPercentage;
	Room room;
};

const int MIN_RESERVATION_NUMBER = 10000;
const int MAX_RESERVATION_NUMBER = 99999;

const int ROOM_TYPE_COUNT = 2;
const float ROOM_TYPE_PRICES[ROOM_TYPE_COUNT] = { 100, 150 };

const int MIN_ROOM_COUNT = 20;
const int MAX_ROOM_COUNT = 40;

const int DISCOUNT_OPTION_COUNT = 3;
const float DISCOUNT_PERCENTAGES[DISCOUNT_OPTION_COUNT] = { 0.0, 0.1, 0.2 };

vector<Room*> rooms;
unordered_map<int, Reservation*> reservationNumbers;
unordered_map<string, vector<Reservation*> > reservers;
vector<Room*> availableSingleRooms;
vector<Room*> availableDoubleRooms;

float getDiscount()
{
	int option = (rand() % (DISCOUNT_OPTION_COUNT));
	return DISCOUNT_PERCENTAGES[option];
}

Room* drawRoom(const vector<Room*> &rooms)
{
	int option = (rand() % (rooms).size());
	Room* room = rooms[option];
	return room;
}

Reservation* bookRoom(Room* room, string reserverName, int reservedNights, float discountPercentage)
{
	int reservationNumber = MIN_RESERVATION_NUMBER + (rand() % (MAX_RESERVATION_NUMBER - MIN_RESERVATION_NUMBER + 1));
	Reservation* reservation = new Reservation(); 
	reservation->number = reservationNumber;
	reservation->reserverName = reserverName;
	reservation->reservedNights = reservedNights;
	reservation->discountPercentage = discountPercentage;
	reservation->room = *room;
	room->booked = true;
	reservationNumbers.insert({ reservationNumber, reservation });
	reservers[reserverName].push_back(reservation);

	if (room->roomType == Single)
	{
		availableSingleRooms.erase(remove(availableSingleRooms.begin(), availableSingleRooms.end(), room), availableSingleRooms.end());
	}
	else if (room->roomType == Double)
	{
		availableDoubleRooms.erase(remove(availableDoubleRooms.begin(), availableDoubleRooms.end(), room), availableDoubleRooms.end());
	}
	return reservation;
}

Room* getRoom(int roomNumber)
{
	return rooms[roomNumber - 1];
}

Reservation* checkReservation(int reservationNumber)
{
	if (reservationNumbers.find(reservationNumber) != reservationNumbers.end())
		return reservationNumbers[reservationNumber];
	else
		return nullptr;
}

vector<Reservation*> checkReservations(string reserver)
{
	if (reservers.find(reserver) != reservers.end())
		return reservers[reserver];
	else
		return vector<Reservation*>();
}

float getInvoice(RoomType roomType, int nightsToStay, float discountPercentage)
{
	float price = (ROOM_TYPE_PRICES[roomType] * nightsToStay) * (1.0 - discountPercentage);
	return price;
}

void showReservationDetails(Reservation &reservation)
{
	cout << "\nReservation details for reservation number " << reservation.number << endl;
	cout << "Room " << reservation.room.number << " reserved to " << reservation.reserverName << " for " << reservation.reservedNights << " 1 night(s) for " << getInvoice(reservation.room.roomType, reservation.reservedNights, reservation.discountPercentage) << " euros." << endl;
}

void showReservations(string name, vector<Reservation*> reservations)
{
	cout << "\nReservation numbers for reservations made by " << name << ": \n";
	for (Reservation* reservation : reservations)
	{
		cout << reservation->number << endl;
	}
}

// Detailed menu for reserving rooms
void reserveOptions()
{
	cout << endl;
	string option;
	RoomType roomType;
	int reservedNights = 0;
	float discountPercentage;
	if ((availableSingleRooms.size() + availableDoubleRooms.size()) == 0)
	{
		cout << "All rooms are fully booked.";
		return;
	}
	cout << "What kind of room would you like to book?" << endl;
	cout << "1) Single" << endl;
	cout << "2) Double" << endl;
	cout << "3) Exit" << endl;
	cout << "Please choose between the options (1, 2, 3): ";
	
	cin >> option;
	if (option == "1")
	{
		roomType = Single;
		if (availableSingleRooms.size() == 0)
		{
			cout << "All single rooms are fully booked.";
			return;
		}
	}
	else if (option == "2")
	{
		roomType = Double;
		if (availableDoubleRooms.size() == 0)
		{
			cout << "All double rooms are fully booked.";
			return;
		}
	}
	else if (option == "3")
	{
		return;
	}
	else
	{
		cout << "Please enter a valid input." << endl;
		return;
	}

	cout << "\nPlease choose many nights will you stay? (1-30): ";
	cin >> option;
	try {
		reservedNights = stoi(option);
	}
	catch (std::exception& e) {
		cout << "Please enter a valid number." << endl;
		return;
	}
	if (reservedNights < 1 || reservedNights > 30)
	{
		cout << "Please enter a valid number." << endl;
		return;
	}
	cout << "\nDo you want to draw a random room, or do you want to choose your room from the list?" << endl;
	cout << "1) Draw a random room." << endl;
	cout << "2) Choose a room from the list." << endl;
	cout << "3) Exit program." << endl;
	cout << "Please choose between the options (1, 2, 3): ";

	Room* room = nullptr;
	int roomNumber;
	vector<Room*> chosenRooms = roomType == Single ? availableSingleRooms : availableDoubleRooms;
	cin >> option;
	if (option == "1")
	{
		room = drawRoom(chosenRooms);
	}
	else if (option == "2")
	{
		cout << "Available rooms are: ";
		for (Room* room : chosenRooms)
		{
			cout << room->number << " ";
		}
		cout << endl;
		cout << "Please choose one of the rooms: ";
		cin >> option;
		try {
			roomNumber = stoi(option);
		}
		catch (std::exception& e) {
			cout << "Please enter a valid room number." << endl;
			return;
		}
		if (find(chosenRooms.begin(), chosenRooms.end(), rooms[roomNumber - 1]) == chosenRooms.end())
		{
			cout << "Please enter a valid room number." << endl;
			return;
		}
		room = rooms[roomNumber - 1];
	}
	else if (option == "3")
	{
		return;
	}
	else
	{
		cout << "Please enter a valid input." << endl;
		return;
	}

	discountPercentage = getDiscount();
	cout << "\nRoom " << room->number << " is selected." << endl;
	if (discountPercentage > 0)
	{
		cout << "Congratulations! You get a " << (discountPercentage * 100) << "% discount." << endl;
	}
	cout << "The price of the room " << room->number << " for " << reservedNights << " nights is " << getInvoice(roomType, reservedNights, discountPercentage) << " euros." << endl;
	cout << "Do you want to make the reservation?" << endl;
	cout << "1) Yes" << endl;
	cout << "2) No" << endl;
	cout << "Please choose between the options (1, 2): ";
	
	cin >> option;

	if (option == "1")
	{
		string reserver;
		cout << "Please enter your name: ";
		cin.ignore();
		getline(cin, reserver);
		Reservation* reservation = bookRoom(room, reserver, reservedNights, discountPercentage);
		cout << "Reservation has been creates succesfully. Reservation number is " << reservation->number << endl;
	}
	else if (option == "2")
	{
		return;
	}
	else
	{
		cout << "Please enter a valid input." << endl;
	}
}

// Detailed menu for checking reservations
void checkOptions()
{
	cout << endl;
	string option;
	cout << "Do you want to check a reservation by reservation number or reserver name?" << endl;
	cout << "1) Reservation number" << endl;
	cout << "2) Reserver name" << endl;
	cout << "3) Exit program." << endl;
	cout << "Please choose between the options (1, 2, 3): ";

	cin >> option;
	if (option == "1")
	{
		cout << "Enter reservation number: ";
		cin >> option;
		int reservationNumber = -1;
		try {
			reservationNumber = stoi(option);
		}
		catch (std::exception& e) {
			cout << "Please enter a valid room number." << endl;
			return;
		}
		Reservation* reservation = checkReservation(reservationNumber);

		if (reservation == nullptr)
			cout << "There is no reservation for the given reservation number." << endl;
		else
		{
			showReservationDetails(*reservation);
		}
	}

	else if (option == "2")
	{
		cout << "Enter reserver name: ";
		cin.ignore();
		getline(cin, option);
		vector<Reservation*> reservations = checkReservations(option);

		if (reservations.size() > 0)
		{
			showReservations(option, reservations);
		}
		else
		{
			cout << "No reservations found." << endl;
		}
	}

	else if (option == "3")
	{
		return;
	}

	else
	{
		cout << "Please enter a valid input." << endl;
	}
}

// Menu for user to choose options
void menu()
{
	while (true)
	{
		cout << endl;
		string option;
		cout << "What do you want to do?" << endl;
		cout << "1) Reserve a room." << endl;
		cout << "2) Check reservation." << endl;
		cout << "3) Exit program." << endl;
		cout << "Please choose between the options (1, 2, 3): ";

		cin >> option;
		if (option == "1")
		{
			reserveOptions();
		}
		else if (option == "2")
		{
			checkOptions();
		}
		else if (option == "3")
		{
			break;
		}
		else
		{
			cout << "Please enter a valid input." << endl;
		}
	}
		
}

// Initialize variables and open the menu
int main()
{
	srand((unsigned int)time(NULL));
	int roomCount = MIN_ROOM_COUNT + (rand() % (MAX_ROOM_COUNT - MIN_ROOM_COUNT + 1));
	for (int i = 0; i < roomCount; i++)
	{
		Room* room = new Room();
		room->number = (i + 1);
		room->roomType = Single;
		room->booked = false;
		rooms.push_back(room);
		availableSingleRooms.push_back(room);
	}
	for (int i = roomCount; i < roomCount * 2; i++)
	{
		Room* room = new Room();
		room->number = (i + 1);
		room->roomType = Double;
		room->booked = false;
		rooms.push_back(room);
		availableDoubleRooms.push_back(room);
	}
	cout << "Welcome to Hotel Services." << endl;
	cout << "Here you can make reservations, check your reservation, or cancel your reservation." << endl << endl;
	menu();
}
