#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <sstream>
#include "Header.h"

using namespace std;

void datafile::create_the_file() {
	fstream create_the_file("booking_information.txt", ios::app);
	create_the_file.close();
}

void datafile::exit_function() {
	interface::setstars('*');
	cout << "Press \"0\" to exit: \n";
	interface::setstars('*');
	cin >> exit_checker;
	if (exit_checker == 0) {
		interface::main_interface_output();
		exit_checker = 0;
	}
}

std::string secr::hashfunc(std::string& input) {
	unsigned int hash = 5381;
	for (char c : input)
		hash = ((hash << 5) + hash) + static_cast<unsigned int>(c);
	std::stringstream ss;
	ss << std::hex << hash;
	return ss.str();
}

static void secr::check_func() {
	std::string correctHash = "17f6dc38";
	std::string input;
	interface::setstars('*');
	std::cout << "Enter a password: "; //password
	std::cin >> input;
	std::string inputHash = hashfunc(input);
	//std::cout << inputHash;
	if (inputHash == correctHash)
	{
		interface::setstars('*');
		std::cout << "Access Allowed!\n";
		datafile::write_function();
	}
	else
	{
		interface::setstars('*');
		std::cout << "Access Denied!\n";
		datafile::exit_function();
	}

}

pair<int, int> time_checker() {
	int time_1, time_2;
	cout << "Enter hours: ";
	cin >> time_1;
	if ((time_1 < 0) || (time_1 >= 24) || (!time_1)) {
		interface::setstars('*');
		throw "Incorrect value\n";
		interface::setstars('*');
	}
	cout << "Enter minutes: ";
	cin >> time_2;
	if ((time_2 < 0) || (time_2 >= 60) || (!time_2)) {
		interface::setstars('*');
		throw "Incorrect value\n";
		interface::setstars('*');
	}
	else {
		return pair<int, int>(time_1, time_2);
	}
}

pair<int, int> time_thrower() {
	try {
		auto hours_minutes = time_checker();
		return pair<int, int>(hours_minutes.first, hours_minutes.second);
	}
	catch (const char* error_message) {
		cerr << error_message;
		datafile::exit_function();
	}
}

class flight_time {
private:
	int hours;
	int minutes;
public:
	flight_time(int h, int m) : hours(h), minutes(m) {}
	flight_time(const flight_time& other) : hours(other.hours), minutes(other.minutes) {}
	flight_time(flight_time&& other) noexcept : hours(std::move(other.hours)),
		minutes(std::move(other.minutes)) {
		other.hours = 0; other.minutes = 0;
	}
	friend ostream& operator<<(ostream& os, const flight_time& time) {
		return os << setw(2) << setfill('0') << to_string(time.hours) << ":"
			<< setw(2) << setfill('0') << to_string(time.minutes);
	}
	friend istream& operator>>(istream& is, flight_time& time) {
		char colon;
		return is >> time.hours >> colon >> time.minutes;
	}
	flight_time operator-(const flight_time& other) const {
		int total_minutes1 = hours * 60 + minutes;
		int total_minutes2 = other.hours * 60 + other.minutes;
		int diff;
		if (total_minutes1 >= total_minutes2) {
			diff = total_minutes1 - total_minutes2;
		}
		else {
			diff = (24 * 60 - total_minutes2) + total_minutes1;
		}
		return flight_time(diff / 60, diff % 60);
	}
	bool operator<(const flight_time& other) const {
		return (hours * 60 + minutes) < (other.hours * 60 + other.minutes);
	}
	bool operator>=(const flight_time& other) const {
		return !(*this < other);
	}
	string dur_to_string() const {
		stringstream ss;
		ss << hours << "h " << minutes << "m";
		return ss.str();
	}
};

flight_time get_time(pair<int, int> t_duration) {
	return flight_time(t_duration.first, t_duration.second);
}

void datafile::write_function() {
	interface::setstars('*');
	string newid, newstation, newterminal, newtickets;
	cin.ignore();
	cout << "Write a flight ID:\n";
	getline(cin, newid);
	cout << "Write a starting station:\n";
	getline(cin, newstation);
	cout << "Write a terminal station:\n";
	getline(cin, newterminal);
	cout << "Write available tickets:\n";
	getline(cin, newtickets);
	cout << "Write the start time:\n";
	auto start_timer = time_thrower();
	cout << "Write the end time:\n";
	auto end_timer = time_thrower();
	flight_time total = (get_time(end_timer) - get_time(start_timer));
	string total_time = total.dur_to_string();
	datafile::write_at_the_file(newid,
		newstation,
		newterminal,
		start_timer.first, start_timer.second,
		end_timer.first, end_timer.second,
		newtickets, total_time
	);
};

void datafile::write_at_the_file(
	string flight_id,
	string starting_station,
	string terminal_station,
	int start_timer_first, int start_timer_second,
	int end_timer_first, int end_timer_second,
	string avaliable_tickets, string total_time
)
{
	ofstream file("booking_information.txt", ios::app);
	if (!file.is_open()) {
		cerr << "File Open Error!" << endl;
		return;
	}
	file << flight_id << "/"
		<< starting_station << "/"
		<< terminal_station << "/"
		<< start_timer_first << ":" << start_timer_second << "-"
		<< end_timer_first << ":" << end_timer_second << "/"
		<< avaliable_tickets << "/" << total_time << '\n';
	file.close();
	interface::setstars('*');
	exit_function();
};

void datafile::show_all_flights() {
	interface::setstars('*');
	cout << "This is all avliable flights:\n\n";
	ifstream show_all_flights("booking_information.txt");
	if (!show_all_flights.is_open()) {
		cerr << "File Open Error!" << std::endl;
		return;
	}
	vector<string> flights;
	string line;
	while (getline(show_all_flights, line)) {
		flights.push_back(line);
	}
	show_all_flights.close();
	for (auto it = flights.begin(); it != flights.end(); ++it) {
		stringstream ss(*it);
		vector<string> fields;
		string field;
		while (getline(ss, field, '/')) {
			fields.push_back(field);
		}

		if (fields.size() >= 6) {
			cout << "Flight ID is: " << fields[0] << " | "
				<< "Starting station is: " << fields[1] << " | "
				<< "Terminal station is: " << fields[2] << " | \n"
				<< "Arrival time is: " << fields[3] << " | "
				<< "Number of tickets: " << fields[4] << " | "
				<< "Flight duration: " << fields[5] << "\n\n";
		}
	}

	interface::setstars('*');
	exit_function();
}

void datafile::search_the_file() {
	bool found = false;
	int counter = 0;
	string line;
	interface::setstars('*');
	ifstream search_the_file("booking_information.txt");
	if (!search_the_file.is_open()) {
		cerr << "File Open Error!\n";
		return;
	}

	vector<string> all_lines;
	while (getline(search_the_file, line)) {
		all_lines.push_back(line);
	}
	search_the_file.close();

	string search_term;
	cout << "Enter search term (flight ID, time or terminal station):\n";
	interface::setstars('*');
	cin.ignore();
	getline(cin, search_term);
	transform(search_term.begin(), search_term.end(), search_term.begin(), ::tolower);
	for (auto line_it = all_lines.begin(); line_it != all_lines.end(); ++line_it) {
		vector<string> fields;
		stringstream ss(*line_it);
		string field;

		while (getline(ss, field, '/')) {
			fields.push_back(field);
		}
		if (fields.size() < 6) continue;
		bool match = false;
		for (auto field_it = fields.begin(); field_it != fields.end() && !match; ++field_it) {
			string lower_field = *field_it;
			transform(lower_field.begin(), lower_field.end(), lower_field.begin(), ::tolower);
			if (lower_field.find(search_term) != string::npos) {
				match = true;
			}
		}

		if (match) {
			if (!found) {
				cout << "Search results:\n\n";
				found = true;
			}
			counter++;
			cout << "Result " << counter << ":\n"
				<< "Flight ID: " << fields[0] << "\n"
				<< "From: " << fields[1] << " to " << fields[2] << "\n"
				<< "Time: " << fields[3] << "\n"
				<< "Tickets available: " << fields[4] << "\n"
				<< "Flight duration: " << fields[5] << "\n\n";
		}
	}

	if (!found) {
		cout << "No flights found matching your search.\n";
	}
	else {
		cout << "Total found: " << counter << " flights\n";
	}

	interface::setstars('*');
	exit_function();
}


void datafile::book_the_flight() {
	vector<string> all_lines;
	bool found = false;
	interface::setstars('*');
	ifstream infile("booking_information.txt");
	if (!infile.is_open()) {
		cerr << "File Open Error!\n";
		return;
	}
	string line;
	while (getline(infile, line)) {
		all_lines.push_back(line);
	}
	infile.close();
	string search_term;
	cout << "Enter the flight you want to book (flight ID):\n";
	interface::setstars('*');
	cin.ignore();
	getline(cin, search_term);
	transform(search_term.begin(), search_term.end(), search_term.begin(), ::tolower);
	for (auto it = all_lines.begin(); it != all_lines.end(); ++it) {
		vector<string> fields;
		stringstream ss(*it);
		string field;
		while (getline(ss, field, '/')) {
			fields.push_back(field);
		}
		if (fields.size() >= 6) {
			string lower_id = fields[0];
			transform(lower_id.begin(), lower_id.end(), lower_id.begin(), ::tolower);

			if (lower_id == search_term) {
				int tickets = stoi(fields[4]);
				if (tickets > 0) {
					tickets--;
					fields[4] = to_string(tickets);
					found = true;
					string updated_line;
					for (auto field_it = fields.begin(); field_it != fields.end(); ++field_it) {
						updated_line += *field_it;
						if (next(field_it) != fields.end()) {
							updated_line += "/";
						}
					}

					*it = updated_line;
					cout << "You have booked this flight:\n\n";
					cout << "Flight ID: " << fields[0] << "\n"
						<< "From: " << fields[1] << " to " << fields[2] << "\n"
						<< "Time: " << fields[3] << "\n"
						<< "Tickets available: " << fields[4] << "\n"
						<< "Flight duration: " << fields[5] << "\n\n";
				}
				else {
					cout << "Sorry, no tickets left for this flight.\n";
				}
				break;
			}
		}
	}

	if (found) {
		ofstream outfile("booking_information.txt", ios::trunc);
		for (auto it = all_lines.begin(); it != all_lines.end(); ++it) {
			outfile << *it << '\n';
		}
		outfile.close();
	}
	else {
		cout << "No flights found matching your input.\n";
	}

	interface::setstars('*');
	exit_function();
}

void datafile::refound_the_flight() {
	interface::setstars('*');

	vector<string> all_lines;
	ifstream infile("booking_information.txt");
	if (!infile.is_open()) {
		cerr << "File Open Error!\n";
		return;
	}
	string line;
	while (getline(infile, line)) {
		all_lines.push_back(line);
	}
	infile.close();

	string search_term;
	cout << "Enter the flight you want to refound (flight ID):\n";
	interface::setstars('*');
	cin.ignore();
	getline(cin, search_term);
	transform(search_term.begin(), search_term.end(), search_term.begin(), ::tolower);
	bool found = false;
	for (auto it = all_lines.begin(); it != all_lines.end(); ++it) {
		vector<string> fields;
		stringstream ss(*it);
		string field;

		while (getline(ss, field, '/')) {
			fields.push_back(field);
		}
		if (fields.size() >= 6) {
			string lower_id = fields[0];
			transform(lower_id.begin(), lower_id.end(), lower_id.begin(), ::tolower);
			if (lower_id == search_term) {
				int tickets = stoi(fields[4]);
				tickets++;
				fields[4] = to_string(tickets);
				found = true;

				string updated_line;
				for (auto field_it = fields.begin(); field_it != fields.end(); ++field_it) {
					updated_line += *field_it;
					if (next(field_it) != fields.end()) {
						updated_line += "/";
					}
				}

				*it = updated_line;
				cout << "You have refounded this flight:\n\n";
				cout << "Flight ID: " << fields[0] << "\n"
					<< "From: " << fields[1] << " to " << fields[2] << "\n"
					<< "Time: " << fields[3] << "\n"
					<< "Tickets available: " << fields[4] << "\n"
					<< "Flight duration: " << fields[5] << "\n\n";
				break;
			}
		}
	}

	if (found) {
		ofstream outfile("booking_information.txt", ios::trunc);
		for (auto it = all_lines.begin(); it != all_lines.end(); ++it) {
			outfile << *it << '\n';
		}
		outfile.close();
	}
	else {
		cout << "No flights found matching your input.\n";
	}

	exit_function();
};

void interface::setstars(char symb = '*') {
	cout << "\n" << string(120, symb) << "\n\n";
}

void interface::secondary_interface_output() {
	switch (interface_selection) {
	case 1:
		datafile::show_all_flights();
		break;
	case 2:
		datafile::search_the_file();
		break;
	case 3:
		datafile::book_the_flight();
		break;
	case 4:
		datafile::refound_the_flight();
		break;
	case 5:
		secr::check_func();
		break;
	case 9:
		exit(0);
		break;
	}

}

void interface::main_interface_output() {
	setstars();
	cout
		<< "Airline Booking System\n\n"
		<< "Choose what you want to do(write a number): \n\n"
		<< "1.See all flights\n"
		<< "2.Find a flight\n"
		<< "3.Book a ticket\n"
		<< "4.Refund a ticket\n"
		<< "5.Input a new flight(devolopers only)\n";
	interface::setstars('*');
	cout << "Press \" 9 \" to close the program\n";
	interface::setstars('*');
	cin >> interface_selection;
	interface::secondary_interface_output();
}