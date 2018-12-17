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
  //currently not used!!
      //enter date in day, month, year
   double d=elapsedtime(2, 2, 2013);
   // zenith angle sun at xºN, yºE (for S or W minus sign)
       cout << a(-60,d,13) << "\n";


    //definition state (number flavours, neutrino or antineutrino)
 unsigned int Nneutrinos=3;
  nuSQUIDS nus_sun(Nneutrinos,neutrino);

  // mixing angles
  double th12=0.563942;
  double th13=0.154085;
  double th23=0.785398;
  double th24=0.2;
  double th34=0.1;
  double th14=0.05;
  double dm21=7.65e-05;
  double dm31=0.00247;
  double dm41=0.1;
  double delta=0.0;
  double delta24=0.0;
  double delta34=0.0;


  nus_sun.Set_MixingAngle(0,1,th12);
  nus_sun.Set_MixingAngle(0,2,th13);
  nus_sun.Set_MixingAngle(1,2,th23);

  // square mass differences
  nus_sun.Set_SquareMassDifference(1,dm21);
  nus_sun.Set_SquareMassDifference(2,dm31);

  // CP phase

 nus_sun.Set_CPPhase(0,2,delta);

 //for 4 neutrinos
 //Do the same for vac & earth states!!
 if(Nneutrinos>3){
   nus_sun.Set_MixingAngle(0,3,th14);
   nus_sun.Set_MixingAngle(1,3,th24);
   nus_sun.Set_MixingAngle(2,3,th34);
   nus_sun.Set_SquareMassDifference(3,dm41);
   nus_sun.Set_CPPhase(1,3,delta24);
   nus_sun.Set_CPPhase(2,3,delta34);
 };

 // Define a Const object to handle units
 squids::Const units;

 //final txt file
 std::ofstream file("fluxes_flavor_final.txt");

 //propagation through the Sun
 // using the standard solar model

  std::shared_ptr<Sun> sun = std::make_shared<Sun>();
 std::shared_ptr<Sun::Track> track_sun = std::make_shared<Sun::Track>(0.0,sun->GetRadius());

 nus_sun.Set_Body(sun);
 nus_sun.Set_Track(track_sun);

 // // set initial energy in GeV
  double Einit=15*units.MeV;
 // for(double Einit=35*units.GeV;Einit<=350*units.GeV;Einit=Einit+50*units.GeV){
 nus_sun.Set_E(Einit);

 //enter DM mass in GeV
 double massdm;
 massdm=10;

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

// Lets set the initial state
 // if(Nneutrinos==3)
 marray<double,1> ini_state({Nneutrinos},{fluxe.flux_pion(massdm,log10(Einit/massdm),channel),fluxmu.flux_pion(massdm,log10(Einit/massdm),channel),fluxtau.flux_pion(massdm,log10(Einit/massdm),channel)});
  //}
  //else
  //{marray<double,1> ini_state({Nneutrinos},{fluxe.flux_pion(massdm,log10(Einit/massdm),channel),fluxmu.flux_pion(massdm,log10(Einit/massdm),channel),fluxtau.flux_pion(massdm,log10(Einit/massdm),channel),0.0});
  //}
//set the initial sterile flux by hand

 // marray<double,1> ini_state({Nneutrinos},{fluxe.flux_pion(massdm,log10(Einit/massdm),channel),0,0});


 nus_sun.Set_initial_state(ini_state,flavor);

 std::cout << "In state Sun" << std::endl;
 for (double EE : nus_sun.GetERange()){
   std::cout << EE/units.GeV << " ";
   for(int i = 0; i < Nneutrinos; i++){
     std::cout << nus_sun.EvalFlavor(i) << " ";
   }
   std::cout << std::endl;
 }

 //set precision
 //this affects the running time

   nus_sun.Set_h_max( 200.0*units.km );
 nus_sun.Set_rel_error(1.0e-12);
 nus_sun.Set_abs_error(1.0e-12);

 //evolution of the state

 nus_sun.EvolveState();

 // // Output the result
 std::cout << "Out state Sun" << std::endl;
 for (double EE : nus_sun.GetERange()){
   std::cout << EE/units.GeV << " ";
   for(int i = 0; i < Nneutrinos; i++){
     std::cout << nus_sun.EvalFlavor(i) << " ";
   }
   std::cout << std::endl;
 };

 //propagtion through vacuum

 std::shared_ptr<Vacuum> vacuum = std::make_shared<Vacuum>();

 //distance Sun-earth= 1 AU=149.598 *10^6 km, the distances varies by 1.6% over a year (149 597 870 700 m)
 double baseline_2 = 149597870.0*units.km;
 std::shared_ptr<Vacuum::Track> track_vac = std::make_shared<Vacuum::Track>(baseline_2);
 nuSQUIDS nus_vac(Nneutrinos,neutrino);

 nus_vac.Set_Body(vacuum);
 nus_vac.Set_Track(track_vac);
 nus_vac.Set_E(Einit);

 nus_vac.Set_MixingAngle(0,1,th12);
 nus_vac.Set_MixingAngle(0,2,th13);
 nus_vac.Set_MixingAngle(1,2,th23);
 nus_vac.Set_SquareMassDifference(1,dm21);
 nus_vac.Set_SquareMassDifference(2,dm31);
 nus_vac.Set_CPPhase(0,2,delta);

 if(Nneutrinos>3){
   nus_vac.Set_MixingAngle(0,3,th14);
   nus_vac.Set_MixingAngle(1,3,th24);
   nus_vac.Set_MixingAngle(2,3,th34);
   nus_vac.Set_SquareMassDifference(3,dm41);
   nus_vac.Set_CPPhase(1,3,delta24);
   nus_vac.Set_CPPhase(2,3,delta34);
 };

 // initial state is final state from the Sun
 if(Nneutrinos==3)
 ini_state = {nus_sun.EvalFlavor(0),nus_sun.EvalFlavor(1),nus_sun.EvalFlavor(2)};
 else
   ini_state = {nus_sun.EvalFlavor(0),nus_sun.EvalFlavor(1),nus_sun.EvalFlavor(2),nus_sun.EvalFlavor(3)};

 nus_vac.Set_initial_state(ini_state,flavor);

 std::cout << "In state vacuum" << std::endl;
 for (double EE : nus_vac.GetERange()){
   std::cout << EE/units.GeV << " ";
   for(int i = 0; i < Nneutrinos; i++){
     std::cout << nus_vac.EvalFlavor(i) << " ";
   }
   std::cout << std::endl;
 }
 nus_vac.EvolveState();

 // Output the result
 std::cout << "Out state vacuum" << std::endl;
 for (double EE : nus_vac.GetERange()){
   std::cout << EE/units.GeV << " ";
   for(int i = 0; i < Nneutrinos; i++){
     std::cout << nus_vac.EvalFlavor(i) << " ";
   }
   std::cout << std::endl;
 }

  //propagation through the earth
 //set the zenith angle
  double phi = acos(-1.0);

  std::shared_ptr<EarthAtm> earth_atm = std::make_shared<EarthAtm>();
  std::shared_ptr<EarthAtm::Track> earth_atm_track = std::make_shared<EarthAtm::Track>(phi);
  nuSQUIDS nus_earth(Nneutrinos,neutrino);
  nus_earth.Set_Body(earth_atm);
  nus_earth.Set_Track(earth_atm_track);

  // setup integration settings
  nus_earth.Set_rel_error(1.0e-20);
  nus_earth.Set_abs_error(1.0e-20);

    nus_earth.Set_E(Einit);

  nus_earth.Set_MixingAngle(0,1,th12);
  nus_earth.Set_MixingAngle(0,2,th13);
  nus_earth.Set_MixingAngle(1,2,th23);
  nus_earth.Set_SquareMassDifference(1,dm21);
  nus_earth.Set_SquareMassDifference(2,dm31);
  nus_earth.Set_CPPhase(0,2,delta);

  if(Nneutrinos>3){
    nus_earth.Set_MixingAngle(0,3,th14);
    nus_earth.Set_MixingAngle(1,3,th24);
    nus_earth.Set_MixingAngle(2,3,th34);
    nus_earth.Set_SquareMassDifference(3,dm41);
    nus_earth.Set_CPPhase(1,3,delta24);
    nus_earth.Set_CPPhase(2,3,delta34);
  };

  if(Nneutrinos==3)
  ini_state = {nus_vac.EvalFlavor(0),nus_vac.EvalFlavor(1),nus_vac.EvalFlavor(2)};
  else
    ini_state = {nus_vac.EvalFlavor(0),nus_vac.EvalFlavor(1),nus_vac.EvalFlavor(2),nus_vac.EvalFlavor(3)};

  nus_earth.Set_initial_state(ini_state,flavor);

  std::cout << "In state earth" << std::endl;
  for (double EE : nus_earth.GetERange()){
    std::cout << EE/units.GeV << " ";
    for(int i = 0; i < Nneutrinos; i++){
      std::cout << nus_earth.EvalFlavor(i) << " ";
    }
    std::cout << std::endl;
  }

  nus_earth.EvolveState();

  // Output the result
  std::cout << "Out state earth" << std::endl;
  for (double EE : nus_earth.GetERange()){
    std::cout << EE/units.GeV << " ";
    for(int i = 0; i < Nneutrinos; i++){
      std::cout << nus_earth.EvalFlavor(i) << " ";
    }
    std::cout << std::endl;
  }



//write hdf5 file with final flux
nus_earth.WriteStateHDF5("flux_final_dm.hdf5");

 //Writing to the file!
 file << Einit/units.GeV;
 for(int fl=0; fl<Nneutrinos; fl++){
   file<<" "<< nus_earth.EvalFlavor(fl)<<" ";
 }
  file << std::endl;

 return 0;
}
