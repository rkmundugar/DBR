/*
 * /*std::ifstream wp;
	wp.open("intersection_list",std::ifstream::in);
	char idbuf[255]="",buf[255]="";
	int i=0,j=0,tab;
	unsigned short int wid,wl;
	double wpx,wpy;

	if(wp==NULL)
	{
		std::cout<<"Error in file"<<std::endl;
		exit(0);
	}
	//cout<<"File ok"<<endl;
   while(!wp.eof())
   {
	   i=0;j=0;tab=0;
	   wp.getline(buf,255);
	   //puts(buf);
	   while(buf[i]!= '\0')
	   {
	  	//printf("am in 1st loop\n");
		   if(buf[i] == '\t') //Point to next field.
	  		  i++;
		   while(buf[i]!= '\t' && buf[i]!= '\0')
		   {
			   //printf("am in 2nd loop\n");
			   idbuf[j] = buf[i];
			   idbuf[j+1] = '\0';
			   //std::cout<<"\n"<<buf[i];
			   j++;
			   i++;
		   }
		   //puts(idbuf);
		   if(buf[i] == '\t'&& tab == 0)
		   {
			   wid = atox(idbuf,j);
	  	  	  	//printf("W: Id =  %X\n",wid);
           }
		   if(buf[i] == '\t'&& tab == 1)
		   {
			   wpx  = atof(idbuf);
			   //std::cout<<"wpx = "<<wpx;
		   }
		   if(buf[i] == '\t'&& tab == 2)
		   {
			   wpy  = atof(idbuf);
			   //std::cout<<"wpy = "<<wpy;
		   }
		   if(buf[i] == '\0')
		   {
			   wl = atox(idbuf,j);
			   //std::cout<<"wl = "<<wl;
		   }
		   j=0;// intialize idbuf
		   tab++;
		   //std::cout<<std::endl;

	   if(wpt->wid_ == wid)
	   {
       	   wpt->wx_ = wpx;
	  	   wpt->wy_ = wpy;
	  	   wpt->wl_ = wl;
	  	   wp.close();
	  	   //cout<<"Success"<<endl;
		   return true;
	   }*/
	   //std::cout<<"mysx"<<sx<<" mysy"<<sy<<" wx"<<wpx<<" wy"<<wpy<<std::endl;
	   /*if((wpx==sx+30||wpx==sx-30)&&(wpy>sy-30||wpy>sy+30))
	   {
		   std::cout<<"searching for destination\n";
		   wpt->wx_ = wpx;
		   wpt->wy_ = wpy;
		   wpt->wl_ = wl;
		   wp.close();
		   //return wpt;
	   }
	   else if((wpy==sy-30||wpy==sy+30)&&(wpx>sx-30||wpx>sx+30))
	   {
		   wpt->wx_ = wpx;
		   wpt->wy_ = wpy;
		   wpt->wl_ = wl;
		   wp.close();
		   //return wpt;
	   }
	   }
   }
	wp.close();
 */

#include <iostream>
#include <istream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "dbr_waypointList.h"

using namespace std;

dbr_waypoint::dbr_waypoint()
{
	head_=NULL;
}

struct dbr_wplist* dbr_waypoint::wp_list(double sx,double sy)
{
	struct dbr_wplist *wpt;
	double x,y;
	x=ceil(sx);
	y=ceil(sy);
	//cout<<"searching destination\n";
	//wpt->wid_=1;
	//wpt->wl_=1;
	//wpt->wx_=0.0;
	//wpt->wy_=0.0;
	//cout<<"mysx"<<x<<" mysy"<<y<<"\n";// wx"<<wpt->wx_<<" wy"<<wpt->wy_<<std::endl;


	if(x==0.0||x==500.0||x==1000.0)//||sx-30==0.0||sx-30==500.0||sx-30==1000.0 )
	{

		if((y>500.0 && y<1000.0))//||(sy-30>500.0 && sy-30<1000.0))
		{
			wpt->wx_=sx;
			wpt->wy_=(double)1000.0;
			//std::cout<<"1mysx"<<sx<<" mysy"<<sy<<" wx"<<wpt->wx_<<" wy"<<wpt->wy_<<std::endl;
			return wpt;
		}
		else if((y>0.0&&y<500.0))//||(sy-30>0.0&&sy-30<500.0))
		{
			wpt->wx_=sx;
			wpt->wy_=(double)500.0;
			//std::cout<<"2mysx"<<sx<<" mysy"<<sy<<" wx"<<wpt->wx_<<" wy"<<wpt->wy_<<std::endl;
			return wpt;
	    }
		else
		{
			wpt->wx_=sx;
			wpt->wy_=sy;
			//std::cout<<"3mysx"<<sx<<" mysy"<<sy<<" wx"<<wpt->wx_<<" wy"<<wpt->wy_<<std::endl;
			return wpt;
		}
	}
	else if(y==0.0||y==500.0||y==1000.0)//||sy-30==0.0||sy-30==500.0||sy-30==1000.0)
	{
		if((x>500.0&&x<1000.0))//||(sx-30>500.0&&sx-30<1000.0))
		{
			wpt->wy_=sy;
			wpt->wx_=(double)1000.0;
			//std::cout<<"4mysx"<<sx<<" mysy"<<sy<<" wx"<<wpt->wx_<<" wy"<<wpt->wy_<<std::endl;
			return wpt;
		}
		else if((x>0.0&&x<500.0))//||(sx-30>0.0&&sx-30<500.0))
		{
			wpt->wy_=sy;
			wpt->wx_=(double)500.0;
			//std::cout<<"5mysx"<<sx<<" mysy"<<sy<<" wx"<<wpt->wx_<<" wy"<<wpt->wy_<<std::endl;
			return wpt;
		}
		else
		{
			wpt->wy_=sy;
			wpt->wx_=sx;
			//std::cout<<"6mysx"<<sx<<" mysy"<<sy<<" wx"<<wpt->wx_<<" wy"<<wpt->wy_<<std::endl;
			return wpt;
		}
	}
	else
	{
		//std::cout<<"Error: Waypoint not present\n";
		return NULL;
	}
}

//---------------------------------
void dbr_waypoint::wp_list(double sx,double sy,double *dx,double *dy)
{
	//struct dbr_wplist *wpt;
	//double x,y;
	if(sx>500.0||sx>1000.0||sx<1.0)
	{
		sx=floor(sx);
	}
	else
	{
		sx=ceil(sx);
	}
	if(sy>500.0||sy>1000.0||sy<1.0)
	{
		sy=floor(sy);
	}
	else
	{
		sy=ceil(sy);
	}
	//cout<<"searching destination\n";

	if(sx<=10.0||sx==500.0||sx==1000.0)//||sx-30==0.0||sx-30==500.0||sx-30==1000.0 )
	{

		if((sy>500.0 && sy<1000.0)||(sy-30>500.0 && sy-30<1000.0))
		{
			*dx=sx;
			*dy=(double)1000.0;
			//std::cout<<"1mysx"<<sx<<" mysy"<<sy<<" wx"<<*dx<<" wy"<<*dy<<std::endl;
			//return wpt;
		}
		else if((sy>0.0&&sy<500.0)||(sy-30>0.0&&sy-30<500.0))
		{
			*dx=sx;
			*dy=(double)500.0;
			//std::cout<<"2mysx"<<sx<<" mysy"<<sy<<" wx"<<*dx<<" wy"<<*dy<<std::endl;
			//return wpt;
	    }
		else
		{
			*dx=sx;
			*dy=sy;
			//std::cout<<"3mysx"<<sx<<" mysy"<<sy<<" wx"<<*dx<<" wy"<<*dy<<std::endl;
			//return wpt;
		}
	}
	else if(sy<=10.0||sy==500.0||sy==1000.0)//||sy-30==0.0||sy-30==500.0||sy-30==1000.0)
	{
		if((sx>500.0&&sx<1000.0)||(sx-30>500.0&&sx-30<1000.0))
		{
			*dy=sy;
			*dx=(double)1000.0;
			//std::cout<<"4mysx"<<sx<<" mysy"<<sy<<" wx"<<*dx<<" wy"<<*dy<<std::endl;
			//return wpt;
		}
		else if((sx>0.0&&sx<500.0)||(sx-30>0.0&&sx-30<500.0))
		{
			*dy=sy;
			*dx=(double)500.0;
			//std::cout<<"5mysx"<<sx<<" mysy"<<sy<<" wx"<<*dx<<" wy"<<*dy<<std::endl;
			//return wpt;
		}
		else
		{
			*dy=sy;
			*dx=sx;
			//std::cout<<"6mysx"<<sx<<" mysy"<<sy<<" wx"<<*dx<<" wy"<<*dy<<std::endl;
			//return wpt;
		}
	}
	else
	{
		std::cout<<"Error: Waypoint not present\n";
		//return NULL;
		*dx=-1;
		*dy=-1;
	}
}
