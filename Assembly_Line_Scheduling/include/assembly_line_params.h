/*
 * assembly_line_params.h
 *
 *  Created on: 04-Dec-2012
 *      Author: ankitgupta
 */

#ifndef ASSEMBLY_LINE_PARAMS_H_
#define ASSEMBLY_LINE_PARAMS_H_

#define NO_OF_STATION 25
#define NO_OF_ASSEMBLY_LINE 15
#define MAX_SERVICE_TIME 75
#define MAX_TRANSFER_TIME 20
#define MIN_SERVICE_TIME 25
#define MIN_TRANSFER_TIME 10
#define MAX_ENTRY_TIME 6
#define MAX_EXIT_TIME 7

#define DEBUG 0

typedef unsigned int U32;
typedef int I32;

class station_t {
    private :
        U32 station_number;
        U32 station_location;
        U32 service_time;
        U32 *transfer_time;
    public :
        void set_station_number (U32 id);
        U32 get_station_number ();
        void set_station_location (U32 id);
        U32 get_station_location ();
        void set_service_time ();
        U32 get_service_time ();
        void set_transfer_time ();
        U32 *get_transfer_time ();
};

class assembly_line_t {
    private :
        U32 line_number;
        U32 no_of_stations;
        station_t *stations;
        U32 entry_time;
        U32 exit_time;
    public :
        void set_line_number (U32 id);
        U32 get_line_number ();
        void set_no_of_stations ();
        U32 get_no_of_stations ();
        void init_stations ();
        station_t *get_stations ();
        void set_entry_time ();
        void set_exit_time ();
        U32 get_entry_time ();
        U32 get_exit_time ();
};

class factory_t {
    private :
        U32 no_of_assembly_lines;
        assembly_line_t *assembly_lines;
        U32 optimal_time;
        U32 *optimal_route;
    public :
        void set_no_of_assembly_lines ();
        U32 get_no_of_assembly_lines ();
        void init_assembly ();
        assembly_line_t *get_assembly_lines ();
        void find_optimal_route ();
        void display_optimal_route ();
        void print_service_times ();
};

#endif /* ASSEMBLY_LINE_PARAMS_H_ */
