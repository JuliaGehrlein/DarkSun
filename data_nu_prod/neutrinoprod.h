//this gets the neutrino fluxes from the data files from 1009.0224
#include "nuSQuIDS/nuSQuIDS.h"

class fluxes{

  /* std::string fluxpath="/lustre/juliag/nuSQuIDS-master_07_18/examples/Bodies/data_nu_prod";
  double minzenith,maxzenith,minE,maxE;
  int size;
  int sizeZ,sizeE;
 public:
  fluxes(){};
  fluxes(std::string name){
    SetFlux(name);
    }*/

  unsigned int arraysize;
  nusquids::marray<double,2> sun_model;
    sun_model=nusquids::quickread("/lustre/juliag/nuSQuIDS-master_07_18/examples/Bodies/data_nu_prod/AtProduction_neutrinos_e.dat");
  // sun_model = quickread(SUN_MODEL_LOCATION);
  arraysize = sun_model.extent(0);

  /*std::vector<double> sun_xh;
  std::vector<double> sun_radius;
  std::vector<double> sun_density;
  sun_radius.resize(arraysize);
  sun_density.resize(arraysize);
  sun_xh.resize(arraysize);

  for (unsigned int i=0; i < arraysize;i++){
    sun_radius[i] = sun_model[i][1];
    sun_density[i] = sun_model[i][3];
    sun_xh[i] = sun_model[i][6];
    }*/

  /* inter_density=AkimaSpline(sun_radius,sun_density);
  inter_xh=AkimaSpline(sun_radius,sun_xh);
  }*/
  /*
double Sun::rdensity(double x) const{
  // x is adimentional radius : x = 0 : center, x = 1 : radius                                                                                                  
  if (x < sun_radius[0]){
    return sun_density[0];
  } else if ( x > sun_radius[arraysize-1]){
    return 0;
  } else {
    return inter_density(x);
  }
}
double Sun::rxh(double x) const{
  // x is adimentional radius : x = 0 : center, x = 1 : radius                                                                                                  
  if (x < sun_radius[0]){
    return sun_xh[0];
  } else if ( x > sun_radius[arraysize-1]){
    return 0;
  } else {
    return inter_xh(x);
  }
}

double Sun::density(const GenericTrack& track_input) const
{
  double r = track_input.GetX()/(radius);
  return rdensity(r);
}
double Sun::ye(const GenericTrack& track_input) const 
 {
    double r = track_input.GetX()/(radius);
    return 0.5*(1.0+rxh(r));
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


  */

};
