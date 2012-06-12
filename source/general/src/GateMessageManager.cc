/*----------------------
   GATE version name: gate_v6

   Copyright (C): OpenGATE Collaboration

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See GATE/LICENSE.txt for further details
----------------------*/


 
//-----------------------------------------------------------
/**
 * \file
 * \brief class MessageManager : Manages the messages displayed by Gate (code)
 */
//-----------------------------------------------------------

#include "GateMessageManager.hh"

//-----------------------------------------------------------
GateMessageManager::GateMessageManager()
  : mMaxMessageLength(8), mAllLevel(0)
{
  std::string key;

  // LG : Change the messages here by meaningfull messages for Gate
  key ="Core";
  mMessageLevel[key] = 0;
  mMessageHelp[key] = "Messages generated by the core classes of the lib";
  if (mMaxMessageLength<key.length()) mMaxMessageLength = key.length();

  key ="Physic";
  mMessageLevel[key] = 0;
  mMessageHelp[key] = "Messages related to physics list";
  if (mMaxMessageLength<key.length()) mMaxMessageLength = key.length();

  key ="Cuts";
  mMessageLevel[key] = 0;
  mMessageHelp[key] = "Messages related to physics list";
  if (mMaxMessageLength<key.length()) mMaxMessageLength = key.length();

  key ="Actor";
  mMessageLevel[key] = 0;
  mMessageHelp[key] = "Messages related to actors";
  if (mMaxMessageLength<key.length()) mMaxMessageLength = key.length();

  key ="Image";
  mMessageLevel[key] = 0;
  mMessageHelp[key] = "Messages related to images";
  if (mMaxMessageLength<key.length()) mMaxMessageLength = key.length();
  
  key ="Actions";
  mMessageLevel[key] = 0;
  mMessageHelp[key] = "Messages related to user actions";
  if (mMaxMessageLength<key.length()) mMaxMessageLength = key.length();

  key ="SD";
  mMessageLevel[key] = 0;
  mMessageHelp[key] = "Messages related to sensitive detectors";
  if (mMaxMessageLength<key.length()) mMaxMessageLength = key.length();

  key ="Step";
  mMessageLevel[key] = 0;
  mMessageHelp[key] = "Messages related to steps";
  if (mMaxMessageLength<key.length()) mMaxMessageLength = key.length();

  key = "Help";
  mMessageLevel[key] = 1;
  mMessageHelp[key] = "Help messages";
  if (mMaxMessageLength<key.length()) mMaxMessageLength = key.length();

  key = "Error";
  mMessageLevel[key] = 1;
  mMessageHelp[key] = "Error messages";
  if (mMaxMessageLength<key.length()) mMaxMessageLength = key.length();

  key = "Warning";
  mMessageLevel[key] = 1;
  mMessageHelp[key] = "Warning messages";
  if (mMaxMessageLength<key.length()) mMaxMessageLength = key.length();

  key = "Output";
  mMessageLevel[key] = 1;
  mMessageHelp[key] = "Output messages";
  if (mMaxMessageLength<key.length()) mMaxMessageLength = key.length();

  key = "Debug";
  mMessageLevel[key] = 0;
  mMessageHelp[key] = "Debug messages";
  if (mMaxMessageLength<key.length()) mMaxMessageLength = key.length();

  key = "Config";
  mMessageLevel[key] = 0;
  mMessageHelp[key] = "Configuration related messages";
  if (mMaxMessageLength<key.length()) mMaxMessageLength = key.length();

  key = "Data";
  mMessageLevel[key] = 0;
  mMessageHelp[key] = "Data related messages";
  if (mMaxMessageLength<key.length()) mMaxMessageLength = key.length();

  key = "GeometryDebug";
  mMessageLevel[key] = 0;
  mMessageHelp[key] = "Geometry construction messages";
  if (mMaxMessageLength<key.length()) mMaxMessageLength = key.length();

  key = "Geometry";
  mMessageLevel[key] = 0;
  mMessageHelp[key] = "Geometry construction messages";
  if (mMaxMessageLength<key.length()) mMaxMessageLength = key.length();
    
  key = "Beam";
  mMessageLevel[key] = 0;
  mMessageHelp[key] = "Beam related messages";
  if (mMaxMessageLength<key.length()) mMaxMessageLength = key.length();

  key = "Volume";
  mMessageLevel[key] = 0;
  mMessageHelp[key] = "Beam related messages";
  if (mMaxMessageLength<key.length()) mMaxMessageLength = key.length();

  key = "Time";
  mMessageLevel[key] = 0;
  mMessageHelp[key] = "Time related messages";
  if (mMaxMessageLength<key.length()) mMaxMessageLength = key.length();

  key = "Materials";
  mMessageLevel[key] = 0;
  mMessageHelp[key] = "Materials related messages";
  if (mMaxMessageLength<key.length()) mMaxMessageLength = key.length();

  key = "Move";
  mMessageLevel[key] = 0;
  mMessageHelp[key] = "Motion related messages";
  if (mMaxMessageLength<key.length()) mMaxMessageLength = key.length();

  key = "Repeater";
  mMessageLevel[key] = 0;
  mMessageHelp[key] = "Repeater related messages";
  if (mMaxMessageLength<key.length()) mMaxMessageLength = key.length();

  key = "Acquisition";
  mMessageLevel[key] = 0;
  mMessageHelp[key] = "Acquisition related messages";
  if (mMaxMessageLength<key.length()) mMaxMessageLength = key.length();

  mEnableG4Message = true;
  key = "G4";
  mMessageLevel[key] = 0;
  mMessageHelp[key] = "G4 messages";
  if (mMaxMessageLength<key.length()) mMaxMessageLength = key.length();

  pMessenger = new GateMessageMessenger("/gate",this);
}
//-----------------------------------------------------------

//-----------------------------------------------------------
GateMessageManager* GateMessageManager::GetInstance() 
{ 
  static GateMessageManager* m = 0;
  if (!m) m = new GateMessageManager();
  return m; 
}
//-----------------------------------------------------------

//-----------------------------------------------------------
void GateMessageManager::RegisterMessageType(std::string key, 
					     std::string help,
					     unsigned char default_level) 
{
  GetInstance()->mMessageLevel[key] = default_level;
  GetInstance()->mMessageHelp[key] = help;
  if (GetInstance()->mMaxMessageLength<key.length()) 
    GetInstance()->mMaxMessageLength = key.length();
}
//-----------------------------------------------------------

//-----------------------------------------------------------
void GateMessageManager::SetMessageLevel(std::string key, 
					 unsigned char level) 
{
  std::map<std::string,int>::iterator i;
  if (key==std::string("All")) {
    GetInstance()->mAllLevel = level;
      
    for (i=GetInstance()->mMessageLevel.begin();
	 i!=GetInstance()->mMessageLevel.end();
	 ++i) 
      (*i).second = level;
      
  }
  else {
    i = GetInstance()->mMessageLevel.find(key);
    if (i!=GetInstance()->mMessageLevel.end()) {
      (*i).second = level;
    }
    else {
      GateError("MessageManager::SetMessageLevel : message type=<"
		<<key<<"> unregistered");
    }
  }
}
//-----------------------------------------------------------

//-----------------------------------------------------------
int GateMessageManager::GetMessageLevel(std::string key) 
{
  int l = GetInstance()->mAllLevel;
  std::map<std::string,int>::iterator i = 
    GetInstance()->mMessageLevel.find(key);
  if (i!=GetInstance()->mMessageLevel.end()) {
    if ( (*i).second > l ) l = (*i).second;      
  }
  return l;
}
//-----------------------------------------------------------

//-----------------------------------------------------------
void GateMessageManager::PrintInfo() 
{
  GateMessage("Help",1,"-----------------------------------------------------------------------------" 
	      << Gateendl);
  GateMessage("Help",1, "Category");
  for (int k=0;
       k<(int)(GetInstance()->mMaxMessageLength-8);
       k++) {
    GateMessageCont("Help",1," "); 
  }
  GateMessageCont("Help",1,"Level  Nature" << G4endl);
  std::map<std::string,int>::iterator i;
  std::map<std::string,std::string>::iterator j;  
  for (i=GetInstance()->mMessageLevel.begin(),
	 j=GetInstance()->mMessageHelp.begin();
       i!=GetInstance()->mMessageLevel.end();++i,++j) {
    GateMessage("Help",1, (*i).first);
    for (int k=0;
	 k<(int)(GetInstance()->mMaxMessageLength+2-(*i).first.length());
	 k++) {
      GateMessageCont("Help",1," ");
    }
    GateMessageCont("Help",1, (*i).second << "\t" 
		    << (*j).second << G4endl);
  }
  GateMessage("Help",1,"-----------------------------------------------------------------------------" 
	      << G4endl);
}
//-----------------------------------------------------------

//-----------------------------------------------------------
void GateMessageManager::EnableG4Messages(bool b) 
{
  mEnableG4Message = b;
}
//-----------------------------------------------------------

//-----------------------------------------------------------
G4int GateMessageManager::ReceiveG4cout (G4String s) 
{ 
  if (mEnableG4Message) std::cout << "[G4] " << s;
  return 0;
}
//-----------------------------------------------------------

//-----------------------------------------------------------
std::string GateMessageManager::GetSpace(int n) 
{ 
  std::string s; 
  for (int i=0; i<n; i++) s.insert(0," "); 
  return s; 
}
//-----------------------------------------------------------

//-----------------------------------------------------------
G4int GateMessageManager::ReceiveG4cerr (G4String s) 
{ 
  std::cerr << "\t[G4-cerr] " << s; 
  // Check if this error is 'command not found' (or related) to stop Gate
  bool isMacroError = false;
  std::string::size_type i = s.find("***** COMMAND NOT FOUND <", 0);
  isMacroError = isMacroError || (i != std::string::npos);
  i = s.find("***** Illegal application state <", 0);
  isMacroError = isMacroError || (i != std::string::npos);
  i = s.find("***** Illegal parameter (", 0);
  isMacroError = isMacroError || (i != std::string::npos);
  i = s.find("***** Can not open a macro file <", 0);
  isMacroError = isMacroError || (i != std::string::npos);
  if (isMacroError) { 
    std::cerr << "[Gate] Sorry, error in a macro command : abort." << std::endl;
    exit(-1);
  }      
  // Ciao
  return 0;
}
//-----------------------------------------------------------
