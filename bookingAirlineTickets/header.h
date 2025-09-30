#pragma once
#ifndef HEADER_H
#define HEADER_H

namespace datafile {
	void create_the_file();
	void write_at_the_file(std::string, std::string, std::string, int, int, int, int, std::string, std::string);
	void search_the_file();
	void show_all_flights();
	void book_the_flight();
	void exit_function();
	void refound_the_flight();
}
namespace interface {
	void write_function();
	void main_interface_output();
	void secondary_interface_output();
	void setstars(char);
}

namespace secr {
	void check_func();
	std::string hashfunc(std::string&);
}

class flight_time {
private:
	int hours;
	int minutes;
public:
	flight_time(int h = 0, int m = 0);
	flight_time(const flight_time& other);
	flight_time(flight_time&& other) noexcept;

	friend std::ostream& operator<<(std::ostream& os, const flight_time& time);
	friend std::istream& operator>>(std::istream& is, flight_time& time);

	flight_time operator-(const flight_time& other) const;
	bool operator<(const flight_time& other) const;
	bool operator>=(const flight_time& other) const;
	std::string dur_to_string() const;
};

static int interface_selection, exit_checker;

static std::string flight_id, starting_station, terminal_station, ar_time, avaliable_tickets;

#endif
#pragma once
