#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <sstream>
#include "Header.h"

using namespace std;

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

flight_time::flight_time(int h, int m) : hours(h), minutes(m) {}

flight_time::flight_time(const flight_time& other)
	: hours(other.hours), minutes(other.minutes) {
}

flight_time::flight_time(flight_time&& other) noexcept
	: hours(std::move(other.hours)), minutes(std::move(other.minutes)) {
	other.hours = 0;
	other.minutes = 0;
}

std::ostream& operator<<(std::ostream& os, const flight_time& time) {
	return os << std::setw(2) << std::setfill('0') << time.hours << ":"
		<< std::setw(2) << std::setfill('0') << time.minutes;
}

std::istream& operator>>(std::istream& is, flight_time& time) {
	char colon;
	return is >> time.hours >> colon >> time.minutes;
}

flight_time flight_time::operator-(const flight_time& other) const {
	int total_minutes1 = hours * 60 + minutes;
	int total_minutes2 = other.hours * 60 + other.minutes;
	int diff = (total_minutes1 >= total_minutes2)
		? total_minutes1 - total_minutes2
		: (24 * 60 - total_minutes2) + total_minutes1;
	return flight_time(diff / 60, diff % 60);
}

bool flight_time::operator<(const flight_time& other) const {
	return (hours * 60 + minutes) < (other.hours * 60 + other.minutes);
}

bool flight_time::operator>=(const flight_time& other) const {
	return !(*this < other);
}

std::string flight_time::dur_to_string() const {
	std::stringstream ss;
	ss << hours << "h " << minutes << "m";
	return ss.str();
}

flight_time get_time(pair<int, int> t_duration) {
	return flight_time(t_duration.first, t_duration.second);
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

void interface::write_function() {
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


