/*----------------------
   Copyright (C): OpenGATE Collaboration

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/

/*  Optical Photons: V. Cuplov -  2012
         - Revision 2012/09/17  /gate/output/root/setRootOpticalFlag functionality added.
           Set the flag for Optical ROOT output.
*/


#include "GateToRootMessenger.hh"

#ifdef G4ANALYSIS_USE_ROOT

#include "GateToRoot.hh"
#include "GateOutputMgr.hh"
#include "GateCoincidenceDigi.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//--------------------------------------------------------------------------
GateToRootMessenger::GateToRootMessenger(GateToRoot* gateToRoot)
  : GateOutputModuleMessenger(gateToRoot)
  , m_gateToRoot(gateToRoot)
{
//  G4cout << " Constructor GateToRootMessenger\n";
  G4String cmdName;

  cmdName = GetDirectoryName()+"reset";
  ResetCmd = new G4UIcmdWithoutParameter(cmdName,this);
  ResetCmd->SetGuidance("Reset the output");

  cmdName = GetDirectoryName()+"setFileName";
//  G4cout << " cmdName setFileName = " << cmdName << Gateendl;
  SetFileNameCmd = new G4UIcmdWithAString(cmdName,this);
  SetFileNameCmd->SetGuidance("Set the name of the output ROOT data file");
  SetFileNameCmd->SetParameterName("Name",false);

  cmdName = GetDirectoryName()+"setRootHitFlag";
  RootHitCmd = new G4UIcmdWithABool(cmdName,this);
  RootHitCmd->SetGuidance("Set the flag for Hits ROOT output");
  RootHitCmd->SetGuidance("1. true/false");

  cmdName = GetDirectoryName()+"setRootNtupleFlag";
  RootNtupleCmd = new G4UIcmdWithABool(cmdName,this);
  RootNtupleCmd->SetGuidance("Set the flag for Ntuples ROOT output");
  RootNtupleCmd->SetGuidance("1. true/false");

// optical data
  cmdName = GetDirectoryName()+"setRootOpticalFlag";
  RootOpticalCmd = new G4UIcmdWithABool(cmdName,this);
  RootOpticalCmd->SetGuidance("Set the flag for Optical ROOT output");
  RootOpticalCmd->SetGuidance("1. true/false");
// optical data

  cmdName = GetDirectoryName()+"setRootRecordFlag";
  RootRecordCmd = new G4UIcmdWithABool(cmdName,this);
  RootRecordCmd->SetGuidance("Set the flag for Histogram ROOT output");
  RootRecordCmd->SetGuidance("1. true/false");


  cmdName = GetDirectoryName()+"setSaveRndmFlag";
  SaveRndmCmd = new G4UIcmdWithABool(cmdName,this);
  SaveRndmCmd->SetGuidance("Set the flag for change the seed at each Run");
  SaveRndmCmd->SetGuidance("1. true/false");

  cmdName = GetDirectoryName()+"setCoincidenceMask";
  CoincidenceMaskCmd = new G4UIcommand(cmdName,this);
  CoincidenceMaskCmd->SetGuidance("Set the mask for the coincidence ASCII output");
  CoincidenceMaskCmd->SetGuidance("Sequence of 0 / 1");

  m_coincidenceMaskLength = 100;
  for (G4int iMask=0; iMask<m_coincidenceMaskLength; iMask++) {
    G4UIparameter* MaskParam = new G4UIparameter("mask",'b',true);
    MaskParam->SetDefaultValue(false);
    CoincidenceMaskCmd->SetParameter(MaskParam);
  }

  cmdName = GetDirectoryName()+"setSingleMask";
  SingleMaskCmd = new G4UIcommand(cmdName,this);
  SingleMaskCmd->SetGuidance("Set the mask for the single ASCII output");
  SingleMaskCmd->SetGuidance("Sequence of 0 / 1");

  m_singleMaskLength = 100;
  for (G4int iMask=0; iMask<m_singleMaskLength; iMask++) {
    G4UIparameter* MaskParam = new G4UIparameter("mask",'b',true);
    MaskParam->SetDefaultValue(false);
    SingleMaskCmd->SetParameter(MaskParam);
  }

}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
GateToRootMessenger::~GateToRootMessenger()
{
  delete ResetCmd;

  delete RootHitCmd;
  delete RootNtupleCmd;
  delete RootOpticalCmd;
  delete SetFileNameCmd;
  delete CoincidenceMaskCmd;
  delete SingleMaskCmd;
  delete SaveRndmCmd;
  for (size_t i = 0; i<OutputChannelCmdList.size() ; ++i)
    delete OutputChannelCmdList[i];
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
void GateToRootMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{

  if( command == ResetCmd ) {
    m_gateToRoot->Reset();
  } else if (command == SetFileNameCmd) {
    m_gateToRoot->SetFileName(newValue);
  } else if (command == RootHitCmd) {
    m_gateToRoot->SetRootHitFlag(RootHitCmd->GetNewBoolValue(newValue));
  } else if (command == SaveRndmCmd) {
    m_gateToRoot->SetSaveRndmFlag(SaveRndmCmd->GetNewBoolValue(newValue));
  } else if (command == RootNtupleCmd) {
    m_gateToRoot->SetRootNtupleFlag(RootNtupleCmd->GetNewBoolValue(newValue));
  } else if (command == RootOpticalCmd) {
    m_gateToRoot->SetRootOpticalFlag(RootOpticalCmd->GetNewBoolValue(newValue));
  } else if (command == RootRecordCmd) {
	  m_gateToRoot->SetRecordFlag(RootRecordCmd->GetNewBoolValue(newValue));
	} else if ( IsAnOutputChannelCmd(command) ) {

    ExecuteOutputChannelCmd(command,newValue);
	}else if( command == CoincidenceMaskCmd ) {

    std::vector<G4bool> maskVector;
    const char* newValueChar = newValue;
    //LF
    //std::istrstream is((char*)newValueChar);
    std::istringstream is((char*)newValueChar);
    //
    G4int tempIntBool;
    G4int tempBool;
    maskVector.clear();
    for (G4int iMask=0; iMask<m_coincidenceMaskLength; iMask++) {
      is >> tempIntBool; // NB: is >> bool does not work, so we put is to an integer and we copy the integer to the bool
      tempBool = tempIntBool;
      maskVector.push_back(tempBool);
      //      G4cout << "[GateToASCIIMessenger::SetNewValue] iMask: " << iMask << " maskVector[iMask]: " << maskVector[iMask] << Gateendl;
    }
    GateCoincidenceDigi::SetCoincidenceASCIIMask( maskVector );

  } else if (command == SingleMaskCmd) {

    std::vector<G4bool> maskVector;
    const char* newValueChar = newValue;
    //LF
    //std::istrstream is((char*)newValueChar);
    std::istringstream is((char*)newValueChar);
    //
    G4int tempIntBool;
    G4int tempBool;
    maskVector.clear();
    for (G4int iMask=0; iMask<m_singleMaskLength; iMask++) {
      is >> tempIntBool; // NB: is >> bool does not work, so we put is to an integer and we copy the integer to the bool
      tempBool = tempIntBool;
      maskVector.push_back(tempBool);
      //      G4cout << "[GateToASCIIMessenger::SetNewValue] iMask: " << iMask << " maskVector[iMask]: " << maskVector[iMask] << Gateendl;
    }
    GateSingleDigi::SetSingleASCIIMask( maskVector );
  
  } else {
    GateOutputModuleMessenger::SetNewValue(command,newValue);
  }

}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
void GateToRootMessenger::CreateNewOutputChannelCommand(GateToRoot::VOutputChannel* anOutputChannel)
{

  GateMessage("OutputMgr", 5, " GateToRootMessenger::CreateNewOutputChannelCommand -- begin \n";);

  G4String cmdName;

  m_outputChannelList.push_back(anOutputChannel);

  G4String channelName = anOutputChannel->m_collectionName;
  cmdName = GetDirectoryName()+"setRoot" + channelName + "Flag";

  G4UIcmdWithABool * newCmd = new G4UIcmdWithABool(cmdName,this) ;
  G4String aGuidance = "Set the flag for ROOT output of " + channelName + ".";
  newCmd->SetGuidance(aGuidance.c_str());
  newCmd->SetGuidance("1. true/false");
  OutputChannelCmdList.push_back( newCmd );
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
G4bool GateToRootMessenger::IsAnOutputChannelCmd(G4UIcommand* command)
{
  for (size_t i = 0; i<OutputChannelCmdList.size() ; ++i)
    if ( command == OutputChannelCmdList[i] )
      return true;
  return false;
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
void GateToRootMessenger::ExecuteOutputChannelCmd(G4UIcommand* command, G4String newValue)
{
  for (size_t i = 0; i<OutputChannelCmdList.size() ; ++i){
    if ( command == OutputChannelCmdList[i] ) {
      m_outputChannelList[i]->SetOutputFlag( OutputChannelCmdList[i]->GetNewBoolValue(newValue) );
      break;
    }
    }
}
//--------------------------------------------------------------------------

#endif
