#include "nuSQuIDS/nuSQuIDS.h"
#include <vector>
#include <iostream>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
using namespace nusquids;

class fluxes{
  nusquids::marray<double,2> pion;
  nusquids::marray<double,2> kaon;

  double minmass,maxmass,minE,maxE,minflux,maxflux;
  int size,arraysize;
  int sizeZ,sizeE;
 
  std::string fluxpath="/lustre/juliag/nuSQuIDS-master_11_18/nuSQuIDS-master/examples/DarkSun/data_nu_prod";
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

class writeout{
                                                                                                                                          
  squids::Const units;
 
 public: 
   //Writing to the txt file!
  void writetxtfile( nuSQUIDS &nus, string filename,double E,int Nneu){
  ofstream outputFile;
  outputFile.open(filename+".txt", std::ios_base::app);
  outputFile << E/units.GeV;
  for(int fl=0; fl<Nneu; fl++){
    outputFile<<" "<< nus.EvalFlavor(fl)<<" ";
  }
  outputFile << "\n";
  }

  //write hdf5 file with final flux                                                                                                                                   
  void writehdf5file(nuSQUIDS &nus, string filename){
  nus.WriteStateHDF5(filename+".hdf5");
  }

};

/*class DarkSun{


 public:

   //initalise: mixing parameters, number of neutrinos etc 
  void SetUp(nuSQUIDS &nus){
    //definition state (number flavours, neutrino or antineutrino)                                                                                                       
    unsigned int Nneutrinos=3;
     nus(Nneutrinos,neutrino);

    // mixing angles                                                                                                                                                     
    double th12=0.563942;
    double th13=0.154085;
    double th23=0.785398;
    double dm21=7.65e-05;
    double dm31=0.00247;
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

  }

 
}
*/
