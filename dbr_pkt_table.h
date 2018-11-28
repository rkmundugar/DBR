#ifndef DBR_PKT_TABLE_H_
#define DBR_PKT_TABLE_H_

#include "dbr_packet.h"

struct dbr_pkttable
{
	nsaddr_t node_id_;
	nsaddr_t lasthop_;
	int seqno_;
	int hops_;
	struct dbr_pkttable *next_;
};

class dbr_pktlist
{
		struct dbr_pkttable *head_;
		int table_entry_;
	public:
		dbr_pktlist();
		bool dbr_pktentry(nsaddr_t, nsaddr_t, int, int);
		bool dbr_pktremove(nsaddr_t,int);
		void dbr_pktsearch(nsaddr_t,nsaddr_t*);
		struct dbr_pkttable* dbr_pktfind(nsaddr_t,int);
		void dbr_pktnexthop(nsaddr_t,nsaddr_t *);
};

#endif
