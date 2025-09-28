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
	void write_function();
	void refound_the_flight();
}
namespace interface {
	void main_interface_output();
	void secondary_interface_output();
	void setstars(char);
}

namespace secr {
	static void check_func();
	std::string hashfunc(std::string&);
}

static int interface_selection, exit_checker;

static std::string flight_id, starting_station, terminal_station, ar_time, avaliable_tickets;

#endif#pragma once
