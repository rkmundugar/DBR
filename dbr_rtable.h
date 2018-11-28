/* This file contains of routing table  information.
 * The table stores the neighboring node information like its speed, coordinates.
 * The table  is updated when ever the vehicle speed changes.
 * The location information like coordinates values are calculated
 * based on the distance traveled and digital map locations.
 *
 * The table format is as follows
 * ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |  Node_ID 	|	source x-coordinates	|	source y-coordinates	| destination x-coordinates 	|	destination y-coordinates	| speed	| 	Intervehicular distance	|
 * ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 */


#ifndef __DBR_RTABLE_H__
#define __DBR_RTABLE_H__

#include "dbr_packet.h"
#include "mobilenode.h"

struct dbr_route_table
{
				nsaddr_t node_id_;	//Vehicle identification number
				double sx_;
				double sy_;
				double dx_;
				double dy_;
				double speed_;
				double my_speed_;
				double inter_distance_;	//intervehicular distance
				struct dbr_route_table *next_;
				struct dbr_route_table *prev_;
};

class dbr_rtable
{
	private:
		dbr_route_table *head_;
		dbr_route_table *tail_;
		int rtable_size_;	// no.of table entries
		double my_sx_;
		double my_sy_;
		double my_dx_;
		double my_dy_;
		double my_speed_;
		double my_id_;
		double ts_;



		void dbr_rtable_remove(int);	// remove the vehicle information form table based on intervehicular distance
		void dbr_rtable_remove(double);	// remove the vehicle information based on ID
		bool dbr_rtable_empty(struct dbr_route_table*);	// checking whether the routing table is empty or not
		struct dbr_route_table* dbr_rtable_read(nsaddr_t);	// read the table information for routing
		void dbr_rtable_update(struct dbr_route_table*,double);	// the table is updated using digital map
		double dbr_inter_distance(double,double);	// function to calculate intervehicular distance




	public:
			dbr_rtable();		// Constructor
			~dbr_rtable();		//destructor
			void dbr_rtable_insert(hdr_dbr_hello*,double);	// insert vehicle information into the routing table
			void dbr_delete(struct dbr_route_table*);	// remove the table entry
			void dbr_myinfo(nsaddr_t,double,double,double,double,double,double);	// get the information
};

#endif
