#include <cmath>
#include <math.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <math.h> 
//based on 1208.1043
//The mean anomaly for the orbit of the Earth around the Sun, measured in days, starting from 1 Jan. 2013, 0:00 UTC 
double M (double dd){
  double mm=(356.666444 + 0.98560028*(dd + 1))*M_PI/180.0;
  return mm;
    };

//Correction to the true anomaly, given by Kepler' s "equation of the center":
double  Ce(double dd){
  double e=0.0167032;  
double ce=(2*e - 3/12*pow(e,3))*sin(M(dd))+ 5/4*pow(e,2)*sin(2*M(dd)) + 13.0/12.0*pow(e,3)*sin(3*M(dd));
 return ce;
};

//Ecliptic longitude of the Sun (measured with respect the northward equinox) :                                      
double Phi(double dd){
double phi= M(dd) + Ce(dd) - (360 - 283.161)/180.0*M_PI;         
 return phi;
};

//Polar angle of the Sun in the equatorial reference frame (i.e., the complement of the solar declination) :         
double Theta(double dd){
  //Obliquity of the Earth' s axis of rotation, with respect to the ecliptic :     
double  epsilon = (M_PI*23.43759)/180.0;
double theta= acos(sin(epsilon)*sin(Phi(dd)));        
 return theta;
};

//Time of northward equinox :  
double teq(double dd){
  double dif0=0.001;
  double t0=-10;
  double dif;  
for( double t=77; t<=80; t=t+ 0.01) {                                                                      
  dif=Theta(t)-M_PI/2;
  if(fabs(dif)<fabs(dif0)){
    dif0=dif;
    t0=t;
  };
 };
 return t0;
};

//Longitude at which nothward equinox occurs at sunset, including correction to the hour of sunset given by the equation of time for that day (20 March 2013, UTC):                                                                 
double l0(double dd){
  double intpart;  
  double fractpartteq= modf (teq(dd) , &intpart);
  double l00 = (3.0/4.0 + (7./60. + 25./3600.)/24. - fractpartteq)*360.;
 return l00;
 };

//Altitude of the Sun above the horizon, for the entire year :    
double a(double L, double dd, double l) {
  //Angular velocity (in radians per day) of the rotation of the Earth about its axis :   
  double Omega = (2*M_PI)/23.9344696*24;  
  //Obliquity of the Earth' s axis of rotation, with respect to the ecliptic :    
  double  epsilon = (M_PI*23.43759)/180.0;   
  double t0=  teq(dd) - (l - l0(dd))/Omega*M_PI/180.0;
  double aa=asin(-cos(M_PI*L/180)*sin(Omega*(dd - t0))*cos(Phi(dd))+cos((M_PI*L)/180.0)*cos(Omega*(dd - t0))*cos(epsilon)* sin(Phi(dd)) + sin((M_PI*L)/180)*sin(epsilon)*sin(Phi(dd)));
  return aa;
};


//calculate elapsed days since 1.1.2013
double elapsedtime(double day, int month, int year){
  double daysmonth=0;  
  double daysyear=0;

  int difyear=year-2013;
  int difmonth=month-1;
  double difdays=day-1;
  int numdaysyear=0;
  int numdaysmonth=0;


if(difyear<0)
    std::cout<<"ente year>2013"<<std::endl;
  else{
    for(int i=1;i<= difyear;i++){
      if(i%4==0)
	daysyear=366;
      else
	daysyear=365;
      numdaysyear+=daysyear;
    };
  };


 for(int i=1;i<= difmonth;i++){
    if(month-i==1||month-i==3||month-i==5||month-i==7||month-i==8||month-i==10||month-i==12)
       daysmonth=31;
    if(month-i==4||month-i==6||month-i==9||month-i==11)
      daysmonth=30;
    if(month-i==2){
      if(year==2016||year==2020||year==2024||year==2028)
	daysmonth=29;
      else
	daysmonth=28;
    };
      numdaysmonth+=daysmonth;
 };

  double tt=difdays+numdaysmonth+numdaysyear;

  return tt;
};


