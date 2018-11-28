/*
 * In dbr_rtable.cc, the function implementation is defined.
 */


#include <stdio.h>
#include <iostream>
#include <fstream>
#include "dbr/dbr_rtable.h"
using namespace std;


dbr_rtable::dbr_rtable()
{
	head_=tail_=NULL;
	rtable_size_=0;
	my_dx_=0;
	my_dy_=0;
	my_id_=-1;
	my_speed_=0;
	my_sx_=0;
	my_sy_=0;

}

dbr_rtable::~dbr_rtable()
{
	struct dbr_route_table *temp = head_;
	while(temp)
	{
	    temp = temp->next_;
	    free(head_);
	    head_ = temp;
	}
}

bool dbr_rtable::dbr_rtable_empty(struct dbr_route_table *rt)
{
	if(rt==NULL)
		return true;
	else
		return false;
}

void dbr_rtable::dbr_rtable_insert(struct hdr_dbr_hello *pkt, double time_)
{
	//cout<<"\nrtable"<<pkt->id_<<"\t"<<pkt->sx_;

	struct dbr_route_table *temp=dbr_rtable_read(pkt->id_);
	double dis=dbr_inter_distance(pkt->speed_,time_);

	if(temp==NULL)		// the vehicle entry is not there in table
	{

		temp=(struct dbr_route_table*)malloc(sizeof(struct dbr_route_table));	//create new row for each vehicle
		temp->dx_=pkt->dx_;
		temp->dy_=pkt->dy_;
		temp->node_id_=pkt->id_;
		temp->speed_=pkt->speed_;
		temp->sx_=pkt->sx_;
		temp->sy_=pkt->sy_;
		temp->inter_distance_=dis;	//calculate the intervehicular distance
		temp->my_speed_=my_speed_;
		temp->next_=temp->prev_=NULL;

		if(tail_==NULL||head_==NULL)	// the table is empty
		{
			head_=tail_=temp;
			rtable_size_=1;
			//cout<<"head set";
		}
		else	// the table has entry then append the vehicle information
		{
			tail_->next_ = temp;
			temp->prev_ = tail_;
			tail_ = temp;
			rtable_size_++;
		}
		ofstream lp;
		lp.open("rtable_trace.tr",ofstream::app);
		lp<<"INSERTED in"<<my_id_<< "  id="<<temp->node_id_<<"\tsx="<<temp->sx_<<"\tsy="<<temp->sy_<<"\tdx="<<temp->dx_<<"\tdy="<<temp->dy_<<"\tspeed="<<temp->speed_<<"\t dis="<<temp->inter_distance_<<"\n";
		//lp.close();
		//cout<<"at id="<<my_id_<<" cout="<<rtable_size_<<"\n";
	}
	else	//the vehicle information already exits
	{
		ofstream lp;
		lp.open("rtable_trace.tr",ofstream::app);
		lp<<"UPDATED in"<<my_id_<< "  id="<<temp->node_id_<<"\tsx="<<temp->sx_<<"\tsy="<<temp->sy_<<"\tdx="<<temp->dx_<<"\tdy="<<temp->dy_<<"\tspeed="<<temp->speed_<<"\t dis="<<temp->inter_distance_<<"\n";
		/*lp.close();*/

		dbr_rtable_update(temp,time_);
	}
	//free(temp);
}

void dbr_rtable::dbr_myinfo(nsaddr_t id,double sx,double sy,double dx,double dy,double sp,double ts)
{
	my_id_=id;
	my_sx_=sx;
	my_sy_=sy;
	my_dx_=dx;
	my_dy_=dy;
	my_speed_=sp;
	ts_=ts;
	//std::cout<<"Rtable entered\n";
}

double dbr_rtable::dbr_inter_distance(double sp_, double time_)
{
	double dis_;
	//cout<<"\n speed="<<my_speed_<<"\t time="<<ts_;
	dis_=((sp_*time_)-(my_speed_*ts_));
	cout<<my_id_<<"inter dis"<<dis_<<"\n";
	dis_=(dis_ < 0) ? -dis_: dis_;
	return dis_;
}

struct dbr_route_table* dbr_rtable::dbr_rtable_read(nsaddr_t id)
{
	struct dbr_route_table *temp=head_;

	while(temp!=NULL)
	{
		if(temp->node_id_==id)
		{
			return temp;
		}
		temp=temp->next_;
	}
	//free(temp);
	return NULL;
}

void dbr_rtable::dbr_rtable_update(struct dbr_route_table *node_,double time_)
{
	struct dbr_route_table *temp=head_;
	//cout<<"in while\n";
	while(temp)
	{

		if(node_->node_id_==temp->node_id_)
		{
			temp->dx_=node_->dx_;
			temp->dy_=node_->dy_;
			temp->node_id_=node_->node_id_;
			temp->speed_=node_->speed_;
			temp->sx_=node_->sx_;
			temp->sy_=node_->sy_;
			temp->inter_distance_=dbr_inter_distance(node_->speed_,time_);	//calculate the intervehicular distance
			temp->my_speed_=my_speed_;
		}
		temp=temp->next_;
	}

	//cout<<"Row updated in rtable\n";
	//free(temp);
}

void dbr_rtable::dbr_rtable_remove(double node_id_)
{
	struct dbr_route_table *temp=head_;
	while(temp)
	{
		if(temp->node_id_==node_id_)
		{
			dbr_delete(temp);
		}
		temp=temp->next_;
	}
	//free(temp);
}

void dbr_rtable::dbr_rtable_remove(int length_)
{
	struct dbr_route_table *temp=head_;
	while(temp)
	{
		if(temp->inter_distance_>length_)
		{
			dbr_delete(temp);
		}
		temp=temp->next_;
	}
	//free(temp);
}

void dbr_rtable::dbr_delete(struct dbr_route_table *node)
{
	struct dbr_route_table *temp=node->prev_;
	if(temp==NULL)
	{
		head_ = node->next_;
		node->next_ = NULL;
	    if(head_ == NULL)
	    	tail_ = NULL;
	    else
			head_->prev_ = NULL;
	    free(node);
	}
	else
	{
	    temp->next_ = node->next_;
	    node->prev_ = NULL;
	    if(temp->next_ == NULL)
	    	tail_ = temp;
	    else
			(temp->next_)->prev_ = temp;
	    free(node);
	}
	rtable_size_--;
}

