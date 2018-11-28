#ifndef DBR_WAYPOINT_H_
#define DBR_WAYPOINT_H_

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "atox.h"


struct dbr_wplist
{
	int wid_;
	double wx_;
	double wy_;
	int wl_;
};

class dbr_waypoint
{
	public:
		struct dbr_wplist *head_;
		dbr_waypoint();
		struct dbr_wplist* wp_list(double,double);
		void wp_list(double sx,double sy,double *dx,double *dy);
};

#endif
