#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include "EventLoop/OutputStream.h"
#include <AsgTools/MsgStream.h>
#include <AsgTools/MsgStreamMacros.h>
#include "xAODCore/AuxInfoBase.h"
#include "xAODCore/ShallowAuxInfo.h"
#include "xAODCore/ShallowCopy.h"

// convert charge code(string) to real charge (float)
inline float getCharge(const xAOD::Vertex* pair) {

  std::string ChargeCode= pair->auxdataConst<std::string>("ChargeCode");
  float charge=-999;
  if(ChargeCode.length() !=2 && ChargeCode.length() != 4) return charge;

  charge=0;
  for(int i=0; i<(int)ChargeCode.length(); i++) {
    if(ChargeCode[i]=='+') charge += 1;
    else if(ChargeCode[i]=='-') charge += -1;
  } 

  return charge;
}
inline float convCharge(std::string ChargeCode) {

  float charge=0;
  for(int i=0; i<(int)ChargeCode.length(); i++) {
    if(ChargeCode[i]=='+') charge += 1;
    else if(ChargeCode[i]=='-') charge += -1;
  } 

  return charge;
}

// retrieve chi2
inline double getChi2(const xAOD::Vertex* pair) {

  double chi2=-999;
  chi2=pair->auxdataConst<float>("chiSquared");

  return chi2;
}

// retrieve ChargeCode
inline std::string getQC(const xAOD::Vertex* pair) {

  std::string cc="";
  cc=pair->auxdataConst<std::string>("ChargeCode");

  return cc;
}

// retrieve CombinationCode
inline std::string getCC(const xAOD::Vertex* pair) {

  std::string cc="";
  cc=pair->auxdataConst<std::string>("CombinationCode");

  return cc;
}

// retrieve mass
inline float getMass(const xAOD::Vertex* pair, int type=0) {

  double mass=-999;
  if(type==0) // PAIR
    mass=pair->auxdataConst<float>("PAIR_mass");
  else if(type==1) // QUAD
    mass=pair->auxdataConst<float>("QUAD_mass");

  return mass;
}

// check if a quad contains a pair
inline bool matchUpsi(std::string CC, std::string upsiCC, int& upsi_trk_ind1, int& upsi_trk_ind2) {
  
  bool matched=true;
  for(int i=0; i<(int)upsiCC.length(); i++) {
    bool matched_tmp=false;
    for(int j=0; j<(int)CC.length(); j++) {
      if(CC[j] == upsiCC[i]) {
        matched_tmp = true;
        if(i==0) upsi_trk_ind1=j;
        if(i==1) upsi_trk_ind2=j;
        break;
      }
    }
    matched &= matched_tmp;
  } 
  return matched;
}

