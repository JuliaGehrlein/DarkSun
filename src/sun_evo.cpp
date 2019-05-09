#include <vector>
#include <iostream>
#include <nuSQuIDS/nuSQuIDS.h>
#include <iostream>
#include <fstream>
#include <string>
#include "exCross.h"

using namespace nusquids;
using namespace std;

//Test with a weird cross section defined in xsec.cpp


int main()
{

  squids::Const units;

  //vector<double> v; v.push_back(500); v.push_back(2000); v.push_back(5000);
  vector<double> v; v.push_back(500);
  for (size_t bla = 0; bla < v.size(); bla++) {

    double mx = v[bla]*units.GeV; //Neutrino initial energy
    int Nbins = 100;

    //Define cross section:
    // Simple test cross section
    double x = -10;
    std::shared_ptr<NeutrinoCrossSections> nu_CS = std::make_shared<LinearCrossSections>(x);
    // Cross section object used by default in nusquids
    std::shared_ptr<NeutrinoCrossSections> nu_CS_orig = std::make_shared<NeutrinoDISCrossSectionsFromTables>();

    //Energy range to work in
    double Emin = 0; double Emax = mx;
    double E_bw = (Emax-Emin)/Nbins; //energy bin width
    nuSQUIDS nus(linspace(E_bw/2.0,Emax-E_bw/2.0,Nbins),3,both,true,nu_CS);
    nuSQUIDS nus_orig(linspace(E_bw/2.0,Emax-E_bw/2.0,Nbins),3,both,true,nu_CS_orig);
    marray<double,1> E_range = nus.GetERange();

    nus.Set_ProgressBar(true); nus_orig.Set_ProgressBar(true);

    // mixing angles
    /* Leave oscillation parameters to default
    double th12=0.5861;
    double th13=0.1592;
    double th23=0.7051;
    double dm21=7.58e-5;
    double dm31=2.35e-3;
    double delta=0.0;

    // Set neutrino mixing angles
    nus.Set_MixingAngle(0,1,th12);
    nus.Set_MixingAngle(0,2,th13);
    nus.Set_MixingAngle(1,2,th23);
    // square mass differences
    nus.Set_SquareMassDifference(1,dm21);
    nus.Set_SquareMassDifference(2,dm31);
    // CP phase
    nus.Set_CPPhase(0,2,delta);
    */

    //Include tau regeneration and oscillations because it is mildly interesting
    nus.Set_TauRegeneration(true); nus_orig.Set_TauRegeneration(true);
    nus.Set_IncludeOscillations(true); nus_orig.Set_IncludeOscillations(true);

    //Propagate in the Sun
    std::shared_ptr<Sun> sun = std::make_shared<Sun>();
    std::shared_ptr<Sun::Track> track_sun = std::make_shared<Sun::Track>(0.0,sun->GetRadius());
    nus.Set_Body(sun); nus_orig.Set_Body(sun);
    nus.Set_Track(track_sun); nus_orig.Set_Track(track_sun);

    // Defining initial state. Sets both nu and anti-nu to one.
    // This is OK because the neutrinos and antineutrinos propagates independently without a considerable flux induced
    marray<double,3> ini_state{E_range.size(),2,3};
    for (size_t idx = 0; idx < ini_state.extent(0); idx++){
      for (size_t jdx = 0; jdx < ini_state.extent(1); jdx++){
        for (size_t kdx = 0; kdx < ini_state.extent(2); kdx++){
          ini_state[idx][jdx][kdx] = (idx == E_range.size()-1 && kdx == 0) ? 1 : 0;
        }
      }
    }

    // Actually sets the initial state
    nus.Set_initial_state(ini_state,flavor);
    nus_orig.Set_initial_state(ini_state,flavor);


    for (size_t idx = 0; idx < E_range.size(); idx++) {
      cout << E_range[idx]/mx << " " << nus.EvalFlavor(0,E_range[idx],0) << " " << nus.EvalFlavor(1,E_range[idx],0) << " " \
      << nus.EvalFlavor(2,E_range[idx],0) << " " << nus.EvalFlavor(0,E_range[idx],1) << " " << nus.EvalFlavor(1,E_range[idx],1) \
      << " " << nus.EvalFlavor(2,E_range[idx],1) << "\n";
    }

    //set precision
    //this can affect the running time A LOT
    double errors = 1e-3;
    nus.Set_h_max( 100.0*units.km ); nus_orig.Set_h_max( 100.0*units.km );
    nus.Set_rel_error(errors); nus_orig.Set_rel_error(errors);
    nus.Set_abs_error(errors); nus.Set_rel_error(errors);

    //evolution of the state
    nus.EvolveState();
    nus_orig.EvolveState();
    cout << "\n"; //To clear progress bar row

    //Writes test cross section to file [E f_nue f_numu f_nutau f_nubare]
    string file_name = "f_new_mx"+to_string((int)(mx/units.GeV))+"_x1e"+to_string((int) x)+".txt";
    ofstream file;
    file.open(file_name);
    for (size_t idx = 0; idx < E_range.size(); idx++) {
      file << E_range[idx]/mx << " " << nus.EvalFlavor(0,E_range[idx])*Nbins << " " << nus.EvalFlavor(1,E_range[idx])*Nbins \
      << " " << nus.EvalFlavor(2,E_range[idx])*Nbins << " " << nus.EvalFlavor(0,E_range[idx],1)*Nbins << " " << \
      nus.EvalFlavor(1,E_range[idx],1)*Nbins << " " << nus.EvalFlavor(2,E_range[idx],1)*Nbins << "\n";
    }
    file.close();

    //Writes to a textfile:
    file_name = "f_orig_mx"+to_string((int)(mx/units.GeV))+".txt";
    file.open(file_name);
    for (size_t idx = 0; idx < E_range.size(); idx++) {
      file << E_range[idx]/mx << " " << nus_orig.EvalFlavor(0,E_range[idx])*Nbins << " " << nus_orig.EvalFlavor(1,E_range[idx])*Nbins \
      << " " << nus_orig.EvalFlavor(2,E_range[idx])*Nbins << " " << nus_orig.EvalFlavor(0,E_range[idx],1)*Nbins << " " << \
      nus_orig.EvalFlavor(1,E_range[idx],1)*Nbins << " " << nus_orig.EvalFlavor(2,E_range[idx],1)*Nbins << "\n";
    }
    file.close();

  }

  return 0;
}
