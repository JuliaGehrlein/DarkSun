#include <vector>
#include <iostream>
#include <nuSQuIDS/nuSQuIDS.h>
#include "exBody.h"
#include <iostream>
#include <fstream>
#include <string>
#include "data_nu_prod/stuff_neuprod.h"
#include "data_nu_prod/sunpos.h"
#include "data_nu_prod/logo.h"
using namespace nusquids;
using namespace std;

int main()
{

  //print logo
  printlogo();

  //function to calculate  the zenith angle of the Sun
  //enter date in day, month, year
  //double d=elapsedtime(2, 2, 2013);
  // zenith angle sun at xºN, yºE (for S or W minus sign)
  // cout << a(-60,d,13) << "\n";

  //definition state (number flavours, neutrino or antineutrino)
  unsigned int Nneutrinos=3;
  nuSQUIDS nus_evo(Nneutrinos,neutrino);

  // mixing angles
  double th12=0.563942;
  double th13=0.154085;
  double th23=0.785398;
  double dm21=7.65e-05;
  double dm31=0.00247;
  double delta=0.0;

  // Set neutrino mixing angles
  nus_evo.Set_MixingAngle(0,1,th12);
  nus_evo.Set_MixingAngle(0,2,th13);
  nus_evo.Set_MixingAngle(1,2,th23);
  // square mass differences
  nus_evo.Set_SquareMassDifference(1,dm21);
  nus_evo.Set_SquareMassDifference(2,dm31);
  // CP phase
  nus_evo.Set_CPPhase(0,2,delta);

  // Define a Const object to handle units
  squids::Const units; // I love this

  //final txt file
  ofstream file("fluxes_flavor_final.txt");

  //propagation through the Sun
  // using the standard solar model

  std::shared_ptr<Sun> sun = std::make_shared<Sun>();
  std::shared_ptr<Sun::Track> track_sun = std::make_shared<Sun::Track>(0.0,sun->GetRadius());

  nus_evo.Set_Body(sun);
  nus_evo.Set_Track(track_sun);

  double Emin=10*units.MeV;
  double Emax=100*units.MeV;
  for(double Einit=Emin; Einit<=Emax; Einit+=(Emax-Emin)/10){
  // set initial energy in GeV
  //double Einit=15*units.MeV;
  nus_evo.Set_E(Einit);

  //enter DM mass in GeV
  double massdm = 10;

  fluxes fluxe("e");
  fluxes fluxmu("mu");
  fluxes fluxtau("tau");
  //enter energy in log10 [GeV]
  // std::cout<<fluxe.flux_pion(150,-1.5,10)<<std::endl;

  //DM annihilation channel, starting from 1
  int channel;
  channel=9;
  double lE;
  lE=log10(Einit);

  // Lets define the initial state
  marray<double,1> ini_state({Nneutrinos},{fluxe.flux_pion(massdm,log10(Einit/massdm),channel),fluxmu.flux_pion(massdm,log10(Einit/massdm),channel),fluxtau.flux_pion(massdm,log10(Einit/massdm),channel)});

  // Actually sets the initial state
  nus_evo.Set_initial_state(ini_state,flavor);
  cout << "In state Sun" << "\n";
  for (double EE : nus_evo.GetERange()){
    cout << EE/units.GeV << " ";
    for(int i = 0; i < Nneutrinos; i++){
      cout << nus_evo.EvalFlavor(i) << " ";
    }
    cout << "\n";
  }

  //set precision
  //this affects the running time
  nus_evo.Set_h_max( 50.0*units.km );
  nus_evo.Set_rel_error(1.0e-12);
  nus_evo.Set_abs_error(1.0e-12);

  //evolution of the state
  nus_evo.EvolveState();

  // Output the result
  cout << "Out state Sun" << endl;
  for (double EE : nus_evo.GetERange()){
    cout << EE/units.GeV << " ";
    for(int i = 0; i < Nneutrinos; i++){
      cout << nus_evo.EvalFlavor(i) << " ";
    }
    cout << "\n";
  };

  //propagation through vacuum
  std::shared_ptr<Vacuum> vacuum = std::make_shared<Vacuum>();

  //distance Sun-earth= 1 AU=149.598 *10^6 km, the distances varies by 1.6% over a year (149 597 870 700 m)
  double baseline_2 = 149597870.0*units.km;
  std::shared_ptr<Vacuum::Track> track_vac = std::make_shared<Vacuum::Track>(baseline_2);

  //Redefine
  nus_evo.Set_Body(vacuum);
  nus_evo.Set_Track(track_vac);

  // WHY IS THIS NECESSARY?????????
  ini_state = {nus_evo.EvalFlavor(0),nus_evo.EvalFlavor(1),nus_evo.EvalFlavor(2)};
  nus_evo.Set_initial_state(ini_state,flavor);

  // Print initial fluxes for propagation between Sun and Earth
  cout << "In state vacuum" << "\n";
  for (double EE : nus_evo.GetERange()){
    std::cout << EE/units.GeV << " ";
    for(int i = 0; i < Nneutrinos; i++){
      std::cout << nus_evo.EvalFlavor(i) << " ";
    }
    std::cout << "\n";
  }
  nus_evo.EvolveState();

  // Print fluxes after propagation
  cout << "Out state vacuum" << "\n";
  for (double EE : nus_evo.GetERange()){
    std::cout << EE/units.GeV << " ";
    for(int i = 0; i < Nneutrinos; i++){
      std::cout << nus_evo.EvalFlavor(i) << " ";
    }
    std::cout << "\n";
  }

  writeout at1AU;
  string name1AU="1AU_flux";
  at1AU.writetxtfile(nus_evo,name1AU,Einit,Nneutrinos);
  at1AU.writehdf5file(nus_evo,name1AU);

  //propagation through the earth
  //set the zenith angle
  double phi = acos(-1.0); // Eternal winter!!!

  std::shared_ptr<EarthAtm> earth_atm = std::make_shared<EarthAtm>();
  std::shared_ptr<EarthAtm::Track> earth_atm_track = std::make_shared<EarthAtm::Track>(phi);
  //  cout << earth_atm_track->GetBaseline()/units.km << " ...? \n";

  nus_evo.Set_Body(earth_atm);
  nus_evo.Set_Track(earth_atm_track);

  // AND WHY IS THIS NECESSARY???????
  ini_state = {nus_evo.EvalFlavor(0),nus_evo.EvalFlavor(1),nus_evo.EvalFlavor(2)};
  nus_evo.Set_initial_state(ini_state,flavor);

  // Print the flux that enters the Earth
  cout << "In state earth" << "\n";
  for (double EE : nus_evo.GetERange()){
    cout << EE/units.GeV << " ";
    for(int i = 0; i < Nneutrinos; i++){
      cout << nus_evo.EvalFlavor(i) << " ";
    }
    cout << "\n";
  }

  //Perform evolution through the Earth
  nus_evo.EvolveState();

  // Print the flux that leaves the Earth
  cout << "Out state earth" << "\n";
  for (double EE : nus_evo.GetERange()){
    cout << EE/units.GeV << " ";
    for(int i = 0; i < Nneutrinos; i++){
      cout << nus_evo.EvalFlavor(i) << " ";
    }
    cout << "\n";
  }

  /*
  //write hdf5 file with final flux
  nus_evo.WriteStateHDF5("flux_final_dm.hdf5");

  //Writing to the file!
  file << Einit/units.GeV;
  for(int fl=0; fl<Nneutrinos; fl++){
    file<<" "<< nus_evo.EvalFlavor(fl)<<" ";
  }
  file << "\n";
  */
  writeout final;
  string name="final_flux";
  final.writetxtfile(nus_evo,name,Einit,Nneutrinos);
  final.writehdf5file(nus_evo,name);
  }
  return 0;
}
