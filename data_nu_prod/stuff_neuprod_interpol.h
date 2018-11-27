#include "nuSQuIDS/nuSQuIDS.h"

#include <gsl/gsl_interp.h>
#include <gsl/gsl_spline.h>

namespace nusquids{

class fluxes{
  nusquids::marray<double,2> pion;
  nusquids::marray<double,2> kaon;

  double minmass,maxmass,minE,maxE,minflux,maxflux;
  int size,arraysize;
  int sizeZ,sizeE;

  std::vector<double> dm_mass;
  std::vector<double> dm_energy;
  std::vector<double> dm_flux;

  std::string fluxpath="/lustre/juliag/nuSQuIDS-master_07_18/examples/Bodies/data_nu_prod";
 public:
  fluxes(){};
  fluxes(std::string name){
    SetFlux(name);
  }
  void SetFlux(std::string name){
    pion=nusquids::quickread(fluxpath+"/AtProduction_neutrinos_"+name+".dat");
    kaon=nusquids::quickread(fluxpath+"/AtProduction_neutrinos_"+name+".dat");

arraysize = pion.extent(0);

dm_mass.resize(arraysize);
dm_energy.resize(arraysize);
dm_flux.resize(arraysize);

for (unsigned int i=0; i < arraysize;i++){
  dm_mass[i] = pion[i][0];
  dm_energy[i] = pion[i][1];
  dm_flux[i] = pion[i][2];
 }

minmass = dm_mass[0];
maxmass = dm_mass[arraysize-1];
minflux = dm_flux[0];
maxflux = dm_flux[arraysize-1];
minE = dm_energy[0];
maxE = dm_energy[arraysize-1];

 double * earth_radius;
 /// \brief Earth density array                                                                                                                                                
 double * earth_density;

/// \brief Earth radius position array                                                                                                                                           
  earth_radius =new double[arraysize];
 /// \brief Earth density array                                                                                                                                                   
  earth_density =new double[arraysize];
 /// \brief Density gsl spline.                                                                                                                                                   
 gsl_spline * inter_density;
 /// \brief Density gsl spline auxiliary pointer.                                                                                                                                 
 gsl_interp_accel * inter_density_accel;

 for (unsigned int i=0; i < arraysize;i++){
   earth_radius[i] = pion[i][0];
   earth_density[i] = pion[i][1];
 
 }

 inter_density = gsl_spline_alloc(gsl_interp_akima,arraysize);
 inter_density_accel = gsl_interp_accel_alloc ();
 gsl_spline_init (inter_density,earth_radius,earth_density,arraysize);

 std::cout<<minmass<< " "<<maxmass<<" "<<minE<< " "<<maxE<<" "<<minflux<< " "<<maxflux<<std::endl;



 //inter_energy=AkimaSpline(dm_mass,dm_energy);
//inter_ye=AkimaSpline(earth_radius,earth_ye);

/*double Sun::rdensity(double x) const{
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
  }*/
  }
};
}
