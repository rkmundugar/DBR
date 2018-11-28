/*
 * The defines the packet format used in DBR routing protocol
 *
 * The packet is used to send both location information and data,
 * the flags are used to differentiate between hello message, data message and
 * emergency messages.
 *
 * The packet structure is as follows
 *
 *---------------------------------------------------------------------------
 *|	Flags(8 bits)	|	Speed(8 bits)	|			Vehicle ID			   	|
 *---------------------------------------------------------------------------
 *|			Destination ID				|			Next hop				|
 *---------------------------------------------------------------------------
 *|							Source X-coordinate(32 bits)					|
 *---------------------------------------------------------------------------
 *|							Source Y-coordinate(32 bits)					|
 *---------------------------------------------------------------------------
 *|						Destination X-coordinates(32 bits)					|
 *---------------------------------------------------------------------------
 *| 					Destination X-coordinates(32 bits)					|
 *---------------------------------------------------------------------------
 *|									Data									|
 *---------------------------------------------------------------------------
 *
 *
 *
 *
 */


#ifndef __DBR_PACKET_H__
#define __DBR_PACKET_H__


#include <packet.h>
#include <math.h>

#define PACKET_TRACE_FILE "packet_trace.tr"
#define EMERGENCY_TRACE_FILE "emergency_trace.tr"
#define DATA_TRACE_FILE	"data_trace.tr"
#define HELLO_TRACE_FILE "hello_trace.tr"

#define DBR_CURRENT Scheduler::instance().clock()

#define DBR_HELLO 0x01
#define DBR_DATA 0x02
#define DBR_EMERGENCY 0x04

#define HDR_DBR(p)   ((struct hdr_dbr*)hdr_dbr::access(p))
#define HDR_DBR_HELLO(p) ((struct hdr_dbr_hello*)hdr_dbr::access(p))
#define HDR_DBR_DATA(p)	((struct hdr_dbr_data*)hdr_dbr::access(p))
#define HDR_DBR_EMERGENCY(p)	((struct hdr_dbr_emergency*)hdr_dbr::access(p))

struct hdr_dbr
{
	u_int8_t flags_;	//Flags for identifying the message format
	double speed_;	//Speed of the vehicle
	nsaddr_t id_;	//Unique vehicle identification for data transfer
	double sx_;	//Source x-coordinates
	double sy_;	//Source y-coordinates
	double dx_;	//destination x-coordinates
	double dy_;	//destination y-coordinates
	static int offset_;
	inline int size()
	{
		int pkt_size_=2*sizeof(u_int8_t)+sizeof(nsaddr_t)+4*sizeof(double);
		return pkt_size_;
	}
	inline static int& offset()
	{
		return offset_;
	}
	inline static struct hdr_dbr* access(const Packet *p)
	{
	    return (struct hdr_dbr*) p->access(offset_);
	}
};

struct hdr_dbr_hello
{
		u_int8_t flags_;	//Flags for identifying the message format
		double speed_;	//Speed of the vehicle
		nsaddr_t id_;	//Unique vehicle identification for data transfer
		double sx_;	//Source x-coordinates
		double sy_;	//Source y-coordinates
		double dx_;	//destination x-coordinates
		double dy_;	//destination y-coordinates

		inline int size()
		{
			int pkt_size_=2*sizeof(u_int8_t)+sizeof(nsaddr_t)+4*sizeof(double);
			return pkt_size_;
		}
};

struct hdr_dbr_data
{
			u_int8_t flags_;	//Flags for identifying the message format
			double speed_;	//Speed of the vehicle
			int seqno_;	// Sequence number for each data packet
			nsaddr_t id_;	//Unique vehicle identification for data transfer
			nsaddr_t dest_id_;	//Destination vehicle identification
			nsaddr_t next_id;	//Next hop vehicle identification
			double sx_;	//Source x-coordinates
			double sy_;	//Source y-coordinates
			double dx_;	//destination x-coordinates
			double dy_;	//destination y-coordinates
			char* data_;	//data

			inline int size()
			{
				int pkt_size_=2*sizeof(u_int8_t)+3*sizeof(nsaddr_t)+4*sizeof(double)+sizeof(char);
				return pkt_size_;
			}
};

struct hdr_dbr_emergency
{
	u_int8_t flags_;	//Flags for identifying the message format
	nsaddr_t id_;	//Unique vehicle identification for data transfer
	double sx_;	//Source x-coordinates
	double sy_;	//Source y-coordinates
	double dx_;	//destination x-coordinates
	double dy_;	//destination y-coordinates

	inline int size()
	{
		int pkt_size_=sizeof(u_int8_t)+sizeof(nsaddr_t)+4*sizeof(double);
		return pkt_size_;
	}
};

union hdr_all_dbr {
  hdr_dbr       dbr;
  hdr_dbr_hello dbrh;
  hdr_dbr_data  dbrd;
  hdr_dbr_emergency dbre;
};
#endif
