#include "nuSQuIDS/nuSQuIDS.h"

class fluxes{
  nusquids::marray<double,2> pion;
  nusquids::marray<double,2> kaon;

  double minmass,maxmass,minE,maxE,minflux,maxflux;
  int size,arraysize;
  int sizeZ,sizeE;
 
  std::string fluxpath="/lustre/juliag/nuSQuIDS-master_07_18/examples/Bodies/data_nu_prod";
public:
  fluxes(){};
  fluxes(std::string name){
    SetFlux(name);
  }
  void SetFlux(std::string name){
    pion=nusquids::quickread(fluxpath+"/AtProduction_neutrinos_"+name+".dat");
    kaon=nusquids::quickread(fluxpath+"/AtProduction_neutrinos_"+name+".dat");
 
     
    size=pion.size()/pion.extent(1);
    minmass=pion[0][0];    
    maxmass=pion[size-1][0];
    minE=pion[0][1];
    maxE=pion[size-1][1];
    for(int i=0;i<size;i++){
      if(pion[0][0]!=pion[i][0]){
        sizeE=i;
        sizeZ=size/sizeE;
        break;
      }
    }
  }

  
 
  double flux_pion(double z,double E, unsigned int type){
     int iz=(int)((sizeZ-1)*((z-minmass)/(maxmass-minmass)));
      int iE=(int)((sizeE-1)*((E-minE)/(maxE-minE)));
      //          std::cout << "*** "  <<((sizeZ-1)*((z-minmass)/(maxmass-minmass)))<<" "   <<pion[iz*sizeE+iE][2+type]<<" " <<iz*sizeE+iE<<" "<<2+type<< std::endl;                                              
       return pion[iz*sizeE+iE][1+type];
  }

  double flux_kaon(double z,double E, unsigned int type){
    int iz=(int)((sizeZ-1)*((z-minmass)/(maxmass-minmass)));
    int iE=(int)((sizeE-1)*((E-minE)/(maxE-minE)));                                                                                     
    return kaon[iz*sizeE+iE][1+type];
    }
};

