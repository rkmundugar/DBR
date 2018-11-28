#include "dbr_pkt_table.h"
#include <iostream>

dbr_pktlist::dbr_pktlist()
{
	head_=NULL;
	table_entry_=0;
}

bool dbr_pktlist::dbr_pktentry(nsaddr_t id,nsaddr_t prev,int seq,int hops)
{
	struct dbr_pkttable *temp=dbr_pktfind(id,seq);
	if(temp==NULL)
	{
		temp=(struct dbr_pkttable*)malloc(sizeof(struct dbr_pkttable));
		temp->node_id_=id;
		temp->lasthop_=prev;
		temp->seqno_=seq;
		temp->hops_=hops;
		temp->next_=NULL;
		if(head_==NULL)
		{
			head_=temp;
			table_entry_=1;
			//free(temp);
			return true;
		}
		else
		{
			temp->next_=head_;
			head_=temp;
			table_entry_++;
			//free(temp);
			return true;
		}
	}
	else
	{
		//free(temp);
		return false;
	}
}

struct dbr_pkttable* dbr_pktlist::dbr_pktfind(nsaddr_t id,int seq)
{
	struct dbr_pkttable *temp=head_;
	while(temp)
	{
		if(temp->node_id_==id&& temp->seqno_==seq)
		{
			return temp;
		}
		temp=temp->next_;
	}
	return NULL;
}

bool dbr_pktlist::dbr_pktremove(nsaddr_t id,int seq)
{
	struct dbr_pkttable *temp;
	struct dbr_pkttable *prev,*next;

	next=head_;
	prev=NULL;
	while(next)
	{
		temp=next;
		if(temp->node_id_==id&&temp->seqno_==seq)
		{
			next=temp->next_;
			if(prev)
			{
				prev->next_=next;
			}
			else
			{
				head_=next;
			}
			free(temp);
			return true;
		}
		prev=next;
		next=next->next_;
	}
	free(temp);
	free(prev);
	free(next);
	return false;
}

void dbr_pktlist::dbr_pktsearch(nsaddr_t id,nsaddr_t *prev)
{
	struct dbr_pkttable *temp=head_;
	while(temp)
	{
		if(temp->node_id_==id)
		{
			*prev=temp->lasthop_;
		}
		temp=temp->next_;
	}
	free(temp);
}

void dbr_pktlist::dbr_pktnexthop(nsaddr_t prev,nsaddr_t *next)
{
	struct dbr_pkttable *temp=head_;
	while(temp)
	{
		if(temp->lasthop_==prev)
		{
			*next=temp->node_id_;
		}
		temp=temp->next_;
	}
	free(temp);
}
