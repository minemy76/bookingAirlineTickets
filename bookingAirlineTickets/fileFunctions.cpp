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

void datafile::write_at_the_file(
	string flight_id,
	string starting_station,
	string terminal_station,
	int start_timer_first, int start_timer_second,
	int end_timer_first, int end_timer_second,
	string avaliable_tickets, string total_time)
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


