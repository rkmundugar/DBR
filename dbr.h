
#ifndef DBR_ROUTING_H_
#define DBR_ROUTING_H_

#include "config.h"
#include "agent.h"
#include "ip.h"
#include "address.h"
//#include "scheduler.h"
#include "timer-handler.h"
#include "mobilenode.h"
#include "tools/random.h"
#include "packet.h"
#include "trace.h"
#include "classifier-port.h"
#include "cmu-trace.h"
#include "pthread.h"

#include "dbr_packet.h"
#include "dbr_pkt_table.h"
#include "dbr_rtable.h"
#include "dbr_waypointList.h"


class DBRAgent;

class DBRHelloTimer : public TimerHandler
{
	public:
		DBRHelloTimer(DBRAgent *a) : TimerHandler() {a_=a;}
	protected:
		inline virtual void expire(Event *e);
		DBRAgent *a_;
};


class DBRAgent : public Agent
{
	private:
		friend class DBRHelloTimer;
		//friend class DistQueryTimer;
		MobileNode *node_;             //the attached mobile node
		PortClassifier *port_dmux_;    //for the higher layer app de-multiplexing


		nsaddr_t my_id_;               //node id (address), which is NOT necessary
		double my_sx_;                  //node location info
		double my_sy_;
		double my_dx_;
		double my_dy_;
		double my_speed_;
		double prev_speed_;

		dbr_pktlist *pkt_list_;      //for packet path information
		dbr_rtable *nblist_; //neighbor list: routing table implementation
		dbr_waypoint *wplist_;	//for to store next intersection point

		double hello_period_;
		//double distquery_period_;
		int recv_counter_;
		RNG randSend_;

		//DistQueryTimer dist_query_timer_;
		DBRHelloTimer hello_timer_;

		void readmyinfo();
		void turnon();              //set to be alive
		void turnoff();             //set to be dead
		void startSink();
		void GetLocation(double*, double*); //called at initial phase
		virtual void getLoc();
		void getSpeed();
		void GetVehicleSpeed(double *);
		void getDestination();
		void hellomsg();
		void recvHello(Packet*);
		void recvData(Packet*);
		void recvEmergency(Packet*);
		void sinkRecv(Packet*);
		void forwardData(Packet*);
		void emerRecv(Packet *);
		void dataRecv(Packet*);
		void helloRecv(Packet *);
		void pktRecv(Packet*);


	protected:
		Trace *tracetarget;              //for routing agent special trace
		void trace(char *fmt,...);       //   Not necessary

	public:
		DBRAgent();
		int command(int, const char*const*);
		void hellotout();
		void helloout();
		void recv(Packet*, Handler*);         //inherited virtual function
};

#endif
