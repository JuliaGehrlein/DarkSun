#include "nuSQuIDS/nuSQuIDS.h"


class fluxes{
  nusquids::marray<double,2> pion;
  nusquids::marray<double,2> kaon;

  double minzenith,maxzenith,minE,maxE;
  int size;
  int sizeZ,sizeE;
  std::string fluxpath="/lustre/juliag/nuSQuIDs-master_older/fluxes/initial";
public:
  fluxes(){};
  fluxes(std::string name){
    SetFlux(name);
  }
  void SetFlux(std::string name){
    pion=nusquids::quickread(fluxpath+"/initial_pion_atmopheric_"+name+".dat");
    kaon=nusquids::quickread(fluxpath+"/initial_kaon_atmopheric_"+name+".dat");
    size=pion.size()/pion.extent(1);
    minzenith=pion[0][0];
    maxzenith=pion[size-1][0];
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
    int iz=(int)((sizeZ-1)*((z-minzenith)/(maxzenith-minzenith)));
    int iE=(int)((sizeE-1)*((log10(E)-log10(minE))/(log10(maxE)-log10(minE))));
    //std::cout << "*** "  << z << " " << E <<" "  << iz << " "<< iE << " "  << std::endl;                                                    
    return pion[iz*sizeE+iE][2+type];
  }

  double flux_kaon(double z,double E, unsigned int type){
    int iz=(int)((sizeZ-1)*((z-minzenith)/(maxzenith-minzenith)));
    int iE=(int)((sizeE-1)*((log10(E)-log10(minE))/(log10(maxE)-log10(minE))));
    //    int iE=(int)((sizeE-1)*((E-minE)/(maxE-minE)));                                                                                     
    return kaon[iz*sizeE+iE][2+type];
  }
};

