/*
 * main.cpp
 *
 *  Created on: 08-Dec-2012
 *      Author: ankitgupta
 */

#include <iostream>
#include "../include/assembly_line_params.h"

using namespace std;

/*
 * Assembly Line Scheduling problem.
 * Solved using approach given by CLR
 * Book. Though in CLR, they have solved
 * only for 2 assembly lines and multiple
 * stations, the problem can be solved
 * for N assembly stations. This solution
 * highlights later corollary.
 */

int main ()
{
	factory_t factory;

	factory.init_assembly();
	factory.find_optimal_route();
	factory.display_optimal_route();

#if DEBUG
	factory.print_service_times();
#endif

	return 0;
}



