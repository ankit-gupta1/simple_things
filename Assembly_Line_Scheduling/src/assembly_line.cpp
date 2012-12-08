/*
 * assembly_line.cpp
 *
 *  Created on: 04-Dec-2012
 *      Author: ankitgupta
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <new>
#include <iomanip>
#include "../include/assembly_line_params.h"

using namespace std;

/*
 * Station APIs.
 */

void station_t::set_station_number (U32 id)
{
	this->station_number = id;
}

U32 station_t::get_station_number ()
{
	return this->station_number;
}

void station_t::set_station_location (U32 id)
{
	this->station_location = id;
}

U32 station_t::get_station_location ()
{
	return this->station_location;
}

void station_t::set_service_time ()
{
	srand(time(NULL) + 100 * this->station_number);
	srand(time(NULL) + (100 * this->station_location) + (rand() % MAX_SERVICE_TIME) * 2);
	this->service_time = MIN_SERVICE_TIME + (rand() % MAX_SERVICE_TIME);
}

U32 station_t::get_service_time ()
{
	return this->service_time;
}

void station_t::set_transfer_time ()
{
	U32 i;
	U32 *h_transfer_time = NULL;

	this->transfer_time = new U32[NO_OF_ASSEMBLY_LINE];
	h_transfer_time = this->transfer_time;

	for (i = 0; i < NO_OF_ASSEMBLY_LINE; i++) {
		srand(time(NULL) + (10 * i) + i + rand() % MAX_TRANSFER_TIME * this->station_location);
		srand(time(NULL) + (100 * this->station_number) + (rand() % MAX_SERVICE_TIME) * 2);
		h_transfer_time[i] = MIN_TRANSFER_TIME + (rand() % MAX_TRANSFER_TIME);
	}

	h_transfer_time[this->station_location] = 0;
}

U32* station_t::get_transfer_time ()
{
	return this->transfer_time;
}

/*
 * Assembly Line APIs.
 */

void assembly_line_t::set_line_number (U32 id)
{
	this->line_number = id;
}

U32 assembly_line_t::get_line_number ()
{
	return this->line_number;
}

void assembly_line_t::set_no_of_stations ()
{
	this->no_of_stations = NO_OF_STATION;
}

U32 assembly_line_t::get_no_of_stations ()
{
	return this->no_of_stations;
}

void assembly_line_t::init_stations ()
{
	U32 i;
	station_t *h_stations = NULL;

	this->stations = new station_t[NO_OF_STATION];
	h_stations = this->stations;

	for (i = 0; i < NO_OF_STATION; i++) {
		h_stations[i].set_station_location(this->line_number);
		h_stations[i].set_station_number(i);
		h_stations[i].set_service_time();
		h_stations[i].set_transfer_time();
	}
}

station_t* assembly_line_t::get_stations ()
{
	return this->stations;
}

void assembly_line_t::set_entry_time()
{
	srand(time(NULL) + this->line_number * NO_OF_STATION);
	this->entry_time = 1 + (rand() % MAX_ENTRY_TIME);
}

U32 assembly_line_t::get_entry_time ()
{
	return this->entry_time;
}

void assembly_line_t::set_exit_time ()
{
	srand(time(NULL) + this->line_number * NO_OF_STATION);
	this->exit_time = 1 + (rand() % MAX_EXIT_TIME);
}

U32 assembly_line_t::get_exit_time ()
{
	return this->exit_time;
}

void factory_t::print_service_times ()
{
	U32 i, j;
	assembly_line_t *h_assembly_line = NULL;
	station_t *h_stations = NULL;
	h_assembly_line = this->assembly_lines;
	cout<<"Service times\n";

	for (i = 0; i < NO_OF_ASSEMBLY_LINE; i++) {
		h_stations = h_assembly_line[i].get_stations();

		for (j = 0; j < NO_OF_STATION; j++) {
			cout<<setw(4)<<h_stations[j].get_service_time();
		}
		cout<<endl;
	}
}

/*
 * Factory APIs
 */

void factory_t::set_no_of_assembly_lines ()
{
	this->no_of_assembly_lines = NO_OF_ASSEMBLY_LINE;
}

U32 factory_t::get_no_of_assembly_lines ()
{
	return this->no_of_assembly_lines;
}

void factory_t::init_assembly ()
{
	U32 i;
	assembly_line_t *h_assembly_lines = NULL;

	this->set_no_of_assembly_lines();
	this->assembly_lines = new assembly_line_t[NO_OF_ASSEMBLY_LINE];
	this->optimal_route = new U32[NO_OF_STATION];
	h_assembly_lines = this->assembly_lines;

	for (i = 0; i < NO_OF_ASSEMBLY_LINE; i++) {
		h_assembly_lines[i].set_entry_time();
		h_assembly_lines[i].set_exit_time();
		h_assembly_lines[i].set_line_number(i);
		h_assembly_lines[i].set_no_of_stations();
		h_assembly_lines[i].init_stations();
	}
}

assembly_line_t *factory_t::get_assembly_lines ()
{
	return this->assembly_lines;
}

/*
 * Core Algorithm.
 * Uses dynamic programming approach.
 */

void factory_t::find_optimal_route ()
{
	U32 i, j, k;
	int l;
	U32 min_time[NO_OF_ASSEMBLY_LINE];
	U32 line_number[NO_OF_ASSEMBLY_LINE];
	U32 time_taken[NO_OF_ASSEMBLY_LINE][NO_OF_STATION];
	U32 line_taken[NO_OF_ASSEMBLY_LINE][NO_OF_STATION];
	U32 exit_line;
	assembly_line_t *h_assembly_line = NULL;
	station_t *h_stations = NULL;
	U32 *h_optimal_route = NULL;
	U32 *h_transfer_time = NULL;

	h_assembly_line = this->assembly_lines;
	h_optimal_route = this->optimal_route;

	for (i = 0; i < NO_OF_ASSEMBLY_LINE; i++) {
		h_stations = h_assembly_line[i].get_stations();
		h_transfer_time = h_stations[0].get_transfer_time();

		time_taken[i][0] = h_assembly_line[i].get_entry_time() +
				h_transfer_time[i] + h_stations[0].get_service_time();
		line_taken[i][0] = i;
	}

	for (i = 1; i < NO_OF_STATION; i++) {
		for (j = 0; j < NO_OF_ASSEMBLY_LINE; j++) {
			min_time[j] = ~(1 << 31);

			for (k = 0; k < NO_OF_ASSEMBLY_LINE; k++) {
				h_stations = h_assembly_line[k].get_stations();
				h_transfer_time = h_stations[i - 1].get_transfer_time();

				if (min_time[j] > (time_taken[k][i - 1] +
					h_stations[i].get_service_time() +
					h_transfer_time[j])) {
					min_time[j] = time_taken[k][i - 1] +
							h_stations[i].get_service_time() +
							h_transfer_time[j];

					line_number[j] = k;
				}
			}

			time_taken[j][i] = min_time[j];
			line_taken[j][i] = line_number[j];
		}
	}

#if DEBUG
	cout<<"\nExit times of all lines\n";
	for (k = 0; k < NO_OF_ASSEMBLY_LINE; k++) {
		cout<<"line "<<setw(2)<<k<<" exit time "<<setw(4)<<(time_taken[k][NO_OF_STATION - 1] +
				h_assembly_line[k].get_exit_time())<<endl;
	}
#endif


#if DEBUG
	cout<<"\nPassing times table\n";
	for (k = 0; k < NO_OF_ASSEMBLY_LINE; k++) {
		for (i = 0; i < NO_OF_STATION; i++) {
			cout<<setw(4)<<time_taken[k][i];
		}
		cout<<endl;
	}
#endif

#if DEBUG
	cout<<"\nPassing assembly lines\n";
	for (k = 0; k < NO_OF_ASSEMBLY_LINE; k++) {
		for (i = 0; i < NO_OF_STATION; i++) {
			cout<<setw(2)<<line_taken[k][i];
		}
		cout<<endl;
	}
#endif

	this->optimal_time = ~(1 << 31);
	for (k = 0; k < NO_OF_ASSEMBLY_LINE; k++) {
		if (this->optimal_time > (time_taken[k][NO_OF_STATION - 1] +
				h_assembly_line[k].get_exit_time())) {
			exit_line = k;
			this->optimal_time = time_taken[k][NO_OF_STATION - 1] +
					h_assembly_line[k].get_exit_time();
		}
	}

	for (l = (NO_OF_STATION - 1); l >= 0; l--)
		h_optimal_route[l] = line_taken[exit_line][l];
}

void factory_t::display_optimal_route ()
{
	int i;

	cout<<"\nOptimal route is :\n";
	for (i = NO_OF_STATION - 1; i >= 0; i--) {
		cout<<"line "<<setw(2)<<(this->optimal_route[i] + 1);
		cout<<" station  "<<setw(2)<<(i + 1)<<endl;
	}

	cout<<"\nTotal time taken is : "<<this->optimal_time<<endl;
}
