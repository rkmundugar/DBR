#include "dbr.h"
#include <iostream>
#include <fstream>
using namespace std;

int hdr_dbr::offset_;

static class DBRHeaderClass : public PacketHeaderClass
{
	public:
		DBRHeaderClass() : PacketHeaderClass("PacketHeader/DBR", sizeof(hdr_all_dbr))
		{
			bind_offset(&hdr_dbr::offset_);
		}
}class_dbrhdr;

static class DBRAgentClass : public TclClass
{
	public:
		DBRAgentClass() : TclClass("Agent/DBR"){}
		TclObject *create(int, const char*const*)
		{
			return (new DBRAgent());
		}
}class_dbr;

void DBRHelloTimer::expire(Event *e)
{
	//cout<<"Hello time out\n";
	a_->hellotout();
}


void DBRAgent::hellotout()
{
	//cout<<"Hello sent in hellotout\n";
	getSpeed();
	if(prev_speed_<my_speed_)
	{
		hellomsg();
		//cout<<my_id_<<"hellotime out\n";
	}
	hello_timer_.resched(hello_period_);
}

void DBRAgent::helloout()
{
	//cout<<"Hello sent in helloout\n";
	getSpeed();
	getLoc();
	getDestination();
	hellomsg();
	hello_timer_.resched(hello_period_);
}


void DBRAgent::getSpeed()
{
	GetVehicleSpeed(&my_speed_);
	//cout<<my_speed_<<"speed\n";

}

void DBRAgent::GetVehicleSpeed(double *speed)
{
	double sp;
	sp=(int)node_->speed();
	*speed=sp;
}

void DBRAgent::getLoc()
{
	GetLocation(&my_sx_, &my_sy_);
	//cout<<my_id_<<"hello\n";
}


void DBRAgent::GetLocation(double *sx, double *sy)
{
	double pos_x_, pos_y_, pos_z_;
	node_->getLoc(&pos_x_, &pos_y_, &pos_z_);
	*sx=pos_x_;
	*sy=pos_y_;
}

void DBRAgent::getDestination()
{
	//dbr_wplist *temp=NULL;
	double x,y;
	wplist_->wp_list(my_sx_,my_sy_,&x,&y);
	/*if(temp!=NULL)
	{
		my_dx_=temp->wx_;
		my_dy_=temp->wy_;
	}
	else
	{
		my_dx_=my_sx_;
		my_dy_=my_sy_;
	}*/
	my_dx_=x;
	my_dy_=y;
	//cout<<my_id_<<"destination received\n";
}

DBRAgent::DBRAgent() : Agent(PT_DBR),hello_timer_(this),my_id_(-1), my_sx_(0.0),
						my_sy_(0.0),my_dx_(0.0),my_dy_(0.0),my_speed_(0.0),recv_counter_(0),
						prev_speed_(0.0)
{

	bind("hello_period_", &hello_period_);
	//std::cout<<"TImer assigned\n"<<hello_period_;
	pkt_list_ = new dbr_pktlist();
	nblist_ = new dbr_rtable();
	wplist_=new dbr_waypoint();
	for(int i=0; i<5; i++)
		randSend_.reset_next_substream();
}

void DBRAgent::turnon()
{
	//std::cout<<"node"<<my_id_<<"started\n";
	getLoc();
	getSpeed();
	//prev_speed_=my_speed_;
	//double_t now=Scheduler::instance().clock();
	getDestination();
	nblist_->dbr_myinfo(my_id_,my_sx_,my_sy_,my_dx_,my_dy_,my_speed_,DBR_CURRENT);
	//helloout();
	hello_timer_.resched(randSend_.uniform(0.0,1.0));
	//std::cout<<my_id_<<"\t"<<my_sx_<<"\t"<<my_sy_<<"\t"<<my_dx_<<"\t"<<my_dy_<<"\t"<<my_speed_<<"\t"<<now<< "\n";
}

void DBRAgent::turnoff()
{
   hello_timer_.resched(INFINITY);
}

void DBRAgent::readmyinfo()
{
		getLoc();
		getDestination();
		getSpeed();
		double_t now=Scheduler::instance().clock();
		//dbr_myinfo(nsaddr_t id,double sx,double sy,double dx,double dy,u_int8_t sp,double_t ts)
		nblist_->dbr_myinfo(my_id_,my_sx_,my_sy_,my_dx_,my_dy_,my_speed_,now);

}
void DBRAgent::hellomsg()
{
	if(my_id_ < 0)
		return;
	getLoc();
	getDestination();
	cout<<"\npre speed"<<prev_speed_<<" current"<<my_speed_<<"  at"<<DBR_CURRENT;
	prev_speed_=my_speed_;

	Packet *p = allocpkt();
	struct hdr_cmn *cmh = HDR_CMN(p);
	struct hdr_ip *iph = HDR_IP(p);
	struct hdr_dbr_hello *dbrh = HDR_DBR_HELLO(p);
	cmh->next_hop_ = IP_BROADCAST;
	cmh->direction_=hdr_cmn::DOWN;
	cmh->last_hop_ = my_id_;
	cmh->addr_type_ = NS_AF_INET;
	cmh->ptype() = PT_DBR;
	cmh->size() = IP_HDR_LEN + dbrh->size();

	iph->daddr() = IP_BROADCAST;
	iph->saddr() = my_id_;
	iph->sport() = RT_PORT;
	iph->dport() = RT_PORT;
	iph->ttl_ = 1;
	//cout<<my_id_<<"\n";
	dbrh->id_=my_id_;
	dbrh->flags_ = DBR_HELLO;
	dbrh->sx_ = my_sx_;
	dbrh->sy_ = my_sy_;
	dbrh->speed_=my_speed_;
	dbrh->dx_=my_dx_;
	dbrh->dy_=my_dy_;
	//cout<<"Hello sent\n";
	send(p, 0);
	cout<<"\n"<< my_id_<<"Packet sent\n";
	}

void DBRAgent::helloRecv(Packet *p)
{
	//cout<<"hello\n";
	FILE *fp = fopen(HELLO_TRACE_FILE, "a+");
	struct hdr_cmn *cmh = HDR_CMN(p);
	struct hdr_ip *iph = HDR_IP(p);
	struct hdr_dbr_hello *dbrh=HDR_DBR_HELLO(p);
	cout<<my_id_<<"Packet received by "<<dbrh->id_<<"\n";
	//struct hdr_dbr_hello *hello=HDR_DBR_HELLO(p);
	//double_t now=Scheduler::instance().clock();
	fprintf(fp, "%f\t%d\t%d\t%f\t%f\t%f\t%f\t%d\n", DBR_CURRENT,iph->saddr(), my_id_	,my_sx_,my_sy_,my_dx_,my_dy_,my_speed_);
	fclose(fp);
	Packet::free(p);

}

void DBRAgent::recvHello(Packet *p)
{
	struct hdr_cmn *cmh = HDR_CMN(p);
	struct hdr_dbr_hello *dbrh = HDR_DBR_HELLO(p);
	double now=Scheduler::instance().clock();
	helloRecv(p);
	//Packet::free(p);
	//trace("%d recv Hello from %d", my_id_, cmh->last_hop_);
}

void DBRAgent::emerRecv(Packet *p)
{
	FILE *fp = fopen(EMERGENCY_TRACE_FILE, "a+");
	struct hdr_cmn *cmh = HDR_CMN(p);
	struct hdr_ip *iph = HDR_IP(p);
	fprintf(fp, "%2.f\t%d\t%d\n", DBR_CURRENT,iph->saddr(), cmh->num_forwards());
	fclose(fp);
	//Packet::free(p);

}

void DBRAgent::recvEmergency(Packet *p)
{
	struct hdr_cmn *cmh=HDR_CMN(p);
	struct hdr_dbr_emergency *dbre=HDR_DBR_EMERGENCY(p);
	send(p,0);
	if(my_id_==100)
	{
		emerRecv(p);
		//sPacket::free(p);
	}
}

void DBRAgent::dataRecv(Packet *p)
{
	FILE *fp = fopen(DATA_TRACE_FILE, "a+");
	struct hdr_cmn *cmh = HDR_CMN(p);
	struct hdr_ip *iph = HDR_IP(p);
	fprintf(fp, "%2.f\t%d\t%d\n", DBR_CURRENT,iph->saddr(), cmh->num_forwards());
	fclose(fp);
	Packet::free(p);
}

void DBRAgent::recvData(Packet *p)
{
	struct hdr_cmn *cmh=HDR_CMN(p);
	struct hdr_ip *iph=HDR_IP(p);
	struct hdr_dbr_data *dbrd=HDR_DBR_DATA(p);
	if(dbrd->dest_id_==my_id_)
	{
		dataRecv(p);
		//Packet::free(p);
		return;
	}
	else
	{
		iph->ttl_--;
		if(iph->ttl_ == 0)
		{
			drop(p, DROP_RTR_TTL);
			return;
		}
		forwardData(p);
		//Packet::free(p);
	}

}


void DBRAgent::pktRecv(Packet *p)
{
	FILE *fp = fopen(PACKET_TRACE_FILE, "a+");
	struct hdr_cmn *cmh = HDR_CMN(p);
	struct hdr_ip *iph = HDR_IP(p); //  struct hdr_dbr_data *gdh = HDR_DBR_DATA(p);

	fprintf(fp, "%2.f\t%d\t%d\n", DBR_CURRENT,iph->saddr(), cmh->num_forwards());
	fclose(fp);
	Packet::free(p);
}

void DBRAgent::forwardData(Packet *p)
{
	struct hdr_cmn *cmh = HDR_CMN(p);
	struct hdr_ip *iph = HDR_IP(p);

	if(cmh->direction() == hdr_cmn::UP && ((nsaddr_t)iph->daddr() == IP_BROADCAST ||iph->daddr() == my_id_))
	{
		pktRecv(p);
		printf("receive\n");
		port_dmux_->recv(p, 0);
		return;
	}
	else
	{
		struct hdr_dbr_data *dbrd=HDR_DBR_DATA(p);

		//double dx = dbrd->dx_;
		//double dy = dbrd->dy_;

		nsaddr_t nexthop=-1;
		if(dbrd->next_id==my_id_)
		{
			//search for next hop in packet table
			pkt_list_->dbr_pktnexthop(dbrd->next_id, &nexthop);
		}
		if(nexthop==-1)
		{
			//bool dbr_pktlist::dbr_pktentry(nsaddr_t id,nsaddr_t prev,int seq,int hops)
			pkt_list_->dbr_pktentry(dbrd->id_,dbrd->next_id,dbrd->seqno_,10);
			drop(p,"NOROUTE");
		}
		else
		{
			dbrd->next_id=nexthop;
		}
		cmh->direction() = hdr_cmn::DOWN;
		cmh->addr_type() = NS_AF_INET;
		cmh->ptype()=PT_DBR;
		cmh->last_hop_ = my_id_;
		cmh->next_hop_ = nexthop;
		send(p, 0);
	}
}

void DBRAgent::recv(Packet *p, Handler *h)
{
	//cout<<"received packet\n";
	struct hdr_cmn *cmh = HDR_CMN(p);
	struct hdr_ip *iph = HDR_IP(p);
	double_t now=Scheduler::instance().clock();
	if(iph->saddr() == my_id_)
	{//a packet generated by myself
		cout<<"checking for forward 0  "<<my_id_<<"\t"<<iph->daddr()<<"\t"<<now<<"\t"<<cmh->ptype()<<"\t"<<cmh->num_forwards_<<"\n";
		if(cmh->num_forwards() == 0)
		{
			drop(p, "NoNeed");
		}
		else if(cmh->num_forwards() > 0)
		{ //routing loop
			if(cmh->ptype() != PT_DBR)
				drop(p, DROP_RTR_ROUTE_LOOP);
			else
				Packet::free(p);
			return;
		}
	}

	if(cmh->ptype() == PT_DBR)
	{
		struct hdr_dbr *dbrp = HDR_DBR(p);
		struct hdr_dbr_hello *dbrh=HDR_DBR_HELLO(p);
		switch(dbrp->flags_)
		{
			case DBR_HELLO:
							//cout<<"Hello received by"<<dbrp->id_<<"\t"<<"\n";
							//cout<<"id="<<dbrh->id_<<"\t"<<dbrh->sx_<<"\n";
							nblist_->dbr_rtable_insert(dbrh,now);
							recvHello(p);
							break;
			case DBR_DATA:
							iph->ttl_--;
							if(iph->ttl_ == 0)
							{
								drop(p, DROP_RTR_TTL);
								return;
							}
							cout<<"Data received by"<<dbrp->id_<<"\n";
							recvData(p);
							break;
			case DBR_EMERGENCY:
							iph->ttl_--;
							if(iph->ttl_ == 0)
							{
								drop(p, DROP_RTR_TTL);
								return;
							}
							cout<<"Hello received by"<<dbrp->id_<<"\n";
							recvEmergency(p);
							break;
			default:
				printf("Error with packet type.\n");
				exit(1);
		}
	}
	else
	{
		iph->ttl_--;
		if(iph->ttl_ == 0)
		{
			drop(p, DROP_RTR_TTL);
			return;
		}
		forwardData(p);
	}
}

void DBRAgent::trace(char *fmt, ...)
{
	va_list ap;
	if(!tracetarget)
		return;
	va_start(ap, fmt);
	vsprintf(tracetarget->pt_->buffer(), fmt, ap);
	tracetarget->pt_->dump();
	va_end(ap);
}

int DBRAgent::command(int argc, const char*const* argv)
{
	//std::cout<<"node entered\n";
	if(argc==2)
	{
		//std::cout<<argv[1];
		if(strcasecmp(argv[1], "getloc")==0)
		{
			getLoc();
			getSpeed();
			getDestination();
			return TCL_OK;
		}

		if(strcasecmp(argv[1], "turnon")==0)
		{
			turnon();
			hello_timer_.resched(hello_period_);
			return TCL_OK;
		}

		if(strcasecmp(argv[1], "turnoff")==0)
		{
			turnoff();
			return TCL_OK;
		}

	}


	if(argc==3)
	{

		if(strcasecmp(argv[1], "addr")==0)
		{
			my_id_ = Address::instance().str2addr(argv[2]);
			return TCL_OK;
		}

		TclObject *obj;
		if ((obj = TclObject::lookup (argv[2])) == 0)
		{
			fprintf (stderr, "%s: %s lookup of %s failed\n", __FILE__, argv[1],argv[2]);
			return (TCL_ERROR);
		}

		if (strcasecmp (argv[1], "node") == 0)
		{
			node_ = (MobileNode*) obj;
			return (TCL_OK);
		}
		else if (strcasecmp (argv[1], "port-dmux") == 0)
		{
			port_dmux_ = (PortClassifier*) obj; //(NsObject *) obj;
			return (TCL_OK);
		}
		else if(strcasecmp(argv[1], "tracetarget")==0)
		{
			tracetarget = (Trace *)obj;
			return TCL_OK;
		}
	}// if argc == 3

	return (Agent::command(argc, argv));
}
