
#ifndef  _USER_Parser_cxx
#define  _USER_Parser_cxx

#include "Parser.hxx"


Parser::Parser()
{
    //isLoaded = false;
    loadedConfig = 0;
    IMARegionMap.insert("IE","OC_LAB");


LAB_Specific_Problems  Parser::getSP(TeMIPAlarmObjectUser* alarm_ptr ){
    RWCString additionalText = "";
    if(alarm_ptr->getAdditionalText().IsPresent()){
        additionalText = alarm_ptr -> getAdditionalText().getString();
    }
    if(loadedConfig != Configuration::getCfgPtr()){
        loadFromFile();
    }
    //testParser2(additionalText);
    
    size_t length;
    size_t pos = additionalText.index( SpecificProblem_regexp,&length);
    if(pos != RW_NPOS){
        RWCString result = additionalText(pos,length-1);
	
        result.replace("\n"," ",RWCString::all);          //Change all \n to [space]
        result.replace("[ \t][ \t]+"," ",RWCString::all); //Change all multiple whitespace to single [space]
        result = result.strip(RWCString::both);           //stripp both ends
        
        Trace::getTrace().DEBUG("Parser::getSP SP = \"" + result + "\"");

        int int_SP;
        bool isValueInMap = SPMap.findValue(result,int_SP);
        if(isValueInMap ){
             Trace::getTrace().DEBUG("!!!!!!!!! Match !!!!!!!!!!!!!!");
             return (LAB_Specific_Problems ) int_SP;
        }
    }else{
        Trace::getTrace().DEBUG("Parser::getSP: SP       NOT FOUND !!!!!!!");
    }
    
    return  SP_UNDEFINED;


}

External_Alarm_Slogans  Parser::get_alarm_slogan(TeMIPAlarmObjectUser* alarm_ptr ){
    RWCString additionalText = "";
    if(alarm_ptr->getAdditionalText().IsPresent()){
        additionalText = alarm_ptr -> getAdditionalText().getString();
    }
   
     if(loadedConfig != Configuration::getCfgPtr()){
        loadFromFile();
    }

  

    RWCRExpr re =  RWCRExpr("MAINS F\\.");//
    int pos = additionalText.index(re);
    if( pos != RW_NPOS){
	Trace::getTrace().DEBUG(" Parser::get_alarm_slogan() found EXT_MAINS_F");
	return MAINS_FAILURE;
    }
    
    re =  EXTERNAL_ALARM_SLOGAN_AC_FAILURE_RegExp;
    pos = additionalText.index(re);
    if( pos != RW_NPOS){
	Trace::getTrace().DEBUG(" Parser::get_alarm_slogan() found EXT:AC_FAILURE");
	return AC_FAILURE;
    }

    //****************************************************************
    //       RADIO_X_CEIVER_ADMINISTRATION_MANAGED_OBJECT_FAULT alarms 
    //****************************************************************
    
    re =  RWCRExpr("MAINS FAILURE");RADIO_X_CEIVER_ADMINISTRATION_MANAGED_OBJECT_FAULT_SLOGAN_MAINS_F_RegExp;
    
    pos = additionalText.index(re);
    if( pos != RW_NPOS){
	Trace::getTrace().DEBUG(" Parser::get_alarm_slogan() found 2000:MAINS FAILURE");
	return MAINS_FAILURE ;
    }
    
    //****************************************************************
    //       RADIO_X_CEIVER_ADMINISTRATION_BTS_EXTERNAL_FAULT 
    //****************************************************************
    
    //re =   RWCRExpr("(AC)|(A/C)( POWER)* FAILURE");// 
    re = RADIO_X_CEIVER_ADMINISTRATION_BTS_EXTERNAL_FAULT_SLOGAN_AC_F_regExp;
    RWCString result = additionalText.match(re);
    if( result != ""){//pos != RW_NPOS){
	Trace::getTrace().DEBUG(" Parser::get_alarm_slogan() found 2000:AC_FAILURE = " + result);
	return AC_FAILURE;
    }
    
	Trace::getTrace().DEBUG(" Parser::get_alarm_slogan() found  ALARM_SLOGAN_UNDEFINED");
    return ALARM_SLOGAN_UNDEFINED;
  
}

void Parser::loadFromFile(){
   
    loadedConfig = Configuration::getCfgPtr();  
    isLoaded = true;  
    RWCRExpr* regexp;
    RWCString key;
    RWTValSlist<RWCString> errorList = RWTValSlist<RWCString>();
  
    

    //****************************************************************
    //       Specific problems
    //****************************************************************

    
    regexp =  &SpecificProblem_regexp;
    key =  CFG_Entry_SPECIFIC_PROBLEM_regExp; 
    if(!Configuration::getConfig().getRegExpValue(key,*regexp)){
        Trace::getTrace().DEBUG("Parser::loadFromFile(): failed to find value for: " + key);
        errorList.insert(key);
    }

    //****************************************************************
    //       Build up Specific problem MAP
    //****************************************************************

    RWCString nextSP = RWCString();
    key = CFG_Entry_FILE_PROCESS_UTILITY_Str;
    if(!Configuration::getConfig().getStringValue(key, nextSP)){
        Trace::getTrace().DEBUG("Parser::loadFromFile(): failed to find value for: " + key);
        errorList.insert(key);
    }else{
        SPMap.insert(nextSP,FILE_PROCESS_UTILITY_AUTOMATIC_TRANSFER_FAILURE);
    }


    key = CFG_Entry_CELL_LOGICAL_CH_AVAILABLE_Str; 
    if(!Configuration::getConfig().getStringValue(key,nextSP)){
        Trace::getTrace().DEBUG("Parser::loadFromFile(): failed to find value for: " + key);
        errorList.insert(key);
    }else{
        SPMap.insert(nextSP,CELL_LOGICAL_CHANNEL_AVAILABILITY_SUPERVISION);
    }

    key = CFG_Entry_EXTERNAL_ALARM_Str;
    if(!Configuration::getConfig().getStringValue(key,nextSP)){
        Trace::getTrace().DEBUG("Parser::loadFromFile(): failed to find value for: " + key);
        errorList.insert(key);
    }else{
        SPMap.insert(nextSP,EXTERNAL_ALARM);
    }

    key = CFG_Entry_RADIO_X_BTS_EXTERNAL_Str;
    if(!Configuration::getConfig().getStringValue(key, nextSP)){
        Trace::getTrace().DEBUG("Parser::loadFromFile(): failed to find value for: " + key);
        errorList.insert(key);
    }else{
        SPMap.insert(nextSP,RADIO_X_CEIVER_ADMINISTRATION_BTS_EXTERNAL_FAULT);
    }

    key = CFG_Entry_RADIO_X_MO_FAULT_Str;
    if(!Configuration::getConfig().getStringValue(key,nextSP)){
        Trace::getTrace().DEBUG("Parser::loadFromFile(): failed to find value for: " + key);
        errorList.insert(key);
    }else{
        SPMap.insert(nextSP,RADIO_X_CEIVER_ADMINISTRATION_MANAGED_OBJECT_FAULT);
    }

    key = CFG_Entry_DIP_FAULT_Str;
    if(!Configuration::getConfig().getStringValue(key,nextSP)){
        Trace::getTrace().DEBUG("Parser::loadFromFile(): failed to find value for: " + key);
        errorList.insert(key);
    }else{
        SPMap.insert(nextSP,DIGITAL_PATH_FAULT_SUPERVISION);
    }
    
    key = CFG_Entry_MO_FAULT_LIST_ALARM_Str;
    if(!Configuration::getConfig().getStringValue(key, nextSP)){
        Trace::getTrace().DEBUG("Parser::loadFromFile(): failed to find value for: " + key);
        errorList.insert(key);
    }else{
        SPMap.insert(nextSP,MO_FAULT_LIST_ALARM);
    }


   key = CFG_Entry_GB_INTERFACE_FAULT;
    if(!Configuration::getConfig().getStringValue(key, nextSP)){
        Trace::getTrace().DEBUG("Parser::loadFromFile(): failed to find value for: " + key);
        errorList.insert(key);
    }else{
        SPMap.insert(nextSP,GB_INTERFACE_FAULT_ALARM);
    }

   key = CFG_Entry_RP_GROUP_FAULT;
    if(!Configuration::getConfig().getStringValue(key, nextSP)){
        Trace::getTrace().DEBUG("Parser::loadFromFile(): failed to find value for: " + key);
        errorList.insert(key);
    }else{
        //SPMap.insert(nextSP,RP_GROUP_FAULT_ALARM);
        SPMap.insert(nextSP,RP_FAULT_ALARM);
    }

   key = CFG_Entry_RP_FAULT;
    if(!Configuration::getConfig().getStringValue(key, nextSP)){
        Trace::getTrace().DEBUG("Parser::loadFromFile(): failed to find value for: " + key);
        errorList.insert(key);
    }else{
        SPMap.insert(nextSP,RP_FAULT_ALARM);
    }




  regexp =  &EXTERNAL_ALARM_SLOGAN_MAINS_F_RegExp;// = RWCRExpr("MAINS F\.");
  key =  CFG_Entry_EXTERNAL_ALARM_slogan_MAINS_regExp;
  if(!Configuration::getConfig().getRegExpValue(key,*regexp)){
      Trace::getTrace().DEBUG("Parser::loadFromFile(): failed to find value for: " + key);
      errorList.insert(key);
  }

  regexp =  &EXTERNAL_ALARM_SLOGAN_AC_FAILURE_RegExp;// = RWCRExpr("AC FAILURE");
  key = CFG_Entry_EXTERNAL_ALARM_slogan_AC_RegExp;
  if(!Configuration::getConfig().getRegExpValue(key,*regexp)){
      Trace::getTrace().DEBUG("Parser::loadFromFile(): failed to find value for: " + key);
      errorList.insert(key);
  }

  //
  //----------------- Alarm slogans ---------------------------------
  //
  regexp = &RADIO_X_CEIVER_ADMINISTRATION_MANAGED_OBJECT_FAULT_SLOGAN_MAINS_F_RegExp;
  key  = CFG_Entry_RADIO_X_MO_FAULT_slogan_MAINS_F_REGEXP ;
  if(!Configuration::getConfig().getRegExpValue(key,*regexp)){
    Trace::getTrace().DEBUG("Parser::loadFromFile(): failed to find value for: " + key);
    errorList.insert(key);
  }

  //****************************************************************
  //       RADIO_X_CEIVER_ADMINISTRATION_BTS_EXTERNAL_FAULT alarms 
  //****************************************************************


  regexp = &RADIO_X_CEIVER_ADMINISTRATION_BTS_EXTERNAL_FAULT_SLOGAN_AC_F_regExp;
  key = CFG_Entry_RADIO_X_BTS_EXTERNAL_slogan_AC_F_regExp;
  if(!Configuration::getConfig().getRegExpValue(key,*regexp)){
    Trace::getTrace().DEBUG("Parser::loadFromFile(): failed to find value for: " + key);
    errorList.insert(key);
  }

  //****************************************************************
  //      DIP_FAULT alarms 
  //****************************************************************
  
  regexp =  &DIP_HEADER_RegExp;
  key = CFG_Entry_DIP_HEADER_RegExp;
      if(!Configuration::getConfig().getRegExpValue(key,*regexp)){
    Trace::getTrace().DEBUG("Parser::loadFromFile(): failed to find value for: " + key);
    errorList.insert(key);
  }

  regexp =  &DIP_RegExp;// = RWCRExpr("[0-9a-zA-Z]+ ");
  key =CFG_Entry_DIP_NAME_RegExp;
      if(!Configuration::getConfig().getRegExpValue(key,*regexp)){
    Trace::getTrace().DEBUG("Parser::loadFromFile(): failed to find value for: " + key);
    errorList.insert(key);
  }

  regexp =  &MSCName_RegExp;// = RWCRExpr
  key =  CFG_Entry_MSC_NAME_Regexp;
  if(!Configuration::getConfig().getRegExpValue(key,*regexp)){
    Trace::getTrace().DEBUG("Parser::loadFromFile(): failed to find value for: " + key);
    errorList.insert(key);
  }
  
  //****************************************************************
  //      OtherRegexps
  //****************************************************************  

    regexp =  &MO_MO_Fault_RegExp;// = RWCRExpr
    key = CFG_Entry_RADIO_X_MO_FAULT_MO_RegExp ;
    if(!Configuration::getConfig().getRegExpValue(key,*regexp)){
        Trace::getTrace().DEBUG("Parser::loadFromFile(): failed to find value for: " + key);
        errorList.insert(key);
    } 
    
    regexp =  &alarmSlogan_MO_Fault_RegExp;// = RWCRExpr
    key =  CFG_Entry_RADIO_X_MO_FAULT_Slogan_RegExp ;
    if(!Configuration::getConfig().getRegExpValue(key,*regexp)){
        Trace::getTrace().DEBUG("Parser::loadFromFile(): failed to find value for: " + key);
    errorList.insert(key);
    } 

    regexp =  &MO_Fault_Header_RegExp;// = RWCRExpr
    key = CFG_Entry_RADIO_X_MO_FAULT_Header_Regexp ;
    if(!Configuration::getConfig().getRegExpValue(key,*regexp)){
        Trace::getTrace().DEBUG("Parser::loadFromFile(): failed to find value for: " + key);
        errorList.insert(key);
    }  
    
    //***************************************
    //*   ALARM SLOGAN SYNTAX
    //***************************************/
    
    key =  CFG_Entry_EXTERNAL_ALARM_SYNTX_FILE;
    if(!Configuration::getConfig().getStringValue(key,EXTERNAL_ALARM_SLOGAN_FILE)){
        Trace::getTrace().DEBUG("Parser::loadFromFile(): failed to find value for: " + key);
        errorList.insert(key);
    }  

   key = CFG_Entry_EXTERNAL_ALARM_FORMAT_ERR_STR;
    if(!Configuration::getConfig().getStringValue(key,EXTERNAL_ALARM_SLOGAN_FORMAT_ERR_STR)){
        Trace::getTrace().DEBUG("Parser::loadFromFile(): failed to find value for: " + key);
        errorList.insert(key);
    }
   //--------------------------------------
   //*   Build up the syntax check regexp
   //--------------------------------------
	createExtAlarmSloganSyntax();

  if(errorList.entries() > 0) reportConfigErrors(errorList);

}

void Parser::reportConfigErrors(RWTValSlist<RWCString> errorList){
  RWTValSlistIterator<RWCString> itr(errorList);
  RWCString header = "Parser::LoadFromConfig() description on the following lines.\n\tThe following entries were not found in config:\n";
  RWCString trailer = "This problem will result in malfunction of some rules!!!";
  RWCString body = "";
  for(;itr();){
      body += "\t" + itr.key() + "\n";
  }
  Trace::getTrace().Error(header + body + trailer);
}

void Parser::configurationUpdated(Configuration* newCfg){
    isLoaded = false;
}

RWCString Parser::getDIP(TeMIPAlarmObjectUser* alarm_ptr ){
    RWCString additionalText = "";
    if(alarm_ptr->getAdditionalText().IsPresent()){
        additionalText = alarm_ptr -> getAdditionalText().getString();
    }
    size_t headerStartPos, headerLength, DIPStartPos, DIPEndPos;
    RWCString DIPName = "";
    headerStartPos = additionalText.index(DIP_HEADER_RegExp, &headerLength, 0);
    
    if(headerStartPos != RW_NPOS){ 
        // The header was found "DIP  DIPEND  FAULT   SECTION    HG   DATE  TIME"
        Trace::getTrace().DEBUG("Parser::getDIP: found HEADER");

        DIPStartPos = headerStartPos + headerLength;
	RWCString match = additionalText.match("[-a-zA-Z0-9]+[ ]",DIPStartPos);
       
        //if(DIPEndPos != RW_NPOS){
	if(match != ""){
             //DIPName = additionalText(DIPStartPos,DIPEndPos-DIPStartPos);
		DIPName = match.strip(RWCString::both);
             Trace::getTrace().DEBUG("Parser::getDIP: DIP = \"" + DIPName + "\"");
        }else{
            Trace::getTrace().DEBUG("Parser::getDIP: DIP NOT FOUND !!");
        }
    }else{
        Trace::getTrace().DEBUG("Parser::getDIP: DIP Header NOT FOUND !!");
    }
    
    return DIPName;
    
}
RWCString Parser::getMO_MANAGED_OBJECT_FAULT(TeMIPAlarmObjectUser* alarm_ptr){
    
    RWCString txt = "";
    if(alarm_ptr->getAdditionalText().IsPresent()){
        txt = alarm_ptr -> getAdditionalText().getString();
    }
   Trace::getTrace().DEBUG( "Parser::getMO_MANAGED_OBJECT_FAULT PARSING TXT=" + txt );
				
    size_t header_length; //txt.index(MO_Fault_Header_RegExp,&header_length);
    size_t header_start = txt.index("MO.*RSITE[ \t]*ALARM SLOGAN",&header_length);
    if(header_start != RW_NPOS){
        Trace::getTrace().DEBUG( "Parser::getMO_MANAGED_OBJECT_FAULT HEADER = " + txt(header_start, header_start+header_length));
        size_t MO_length;
        //size_t MO_start = txt.index(MO_MO_Fault_RegExp,&MO_length, header_start+header_length);
	RWCString match = txt.match(MO_MO_Fault_RegExp,header_start+header_length);
        if(match != ""){//MO_start != RW_NPOS){
	     Trace::getTrace().DEBUG( "Parser::getMO_MANAGED_OBJECT_FAULT match = " + match);
	     return match;
            //return txt(MO_start, MO_start + MO_length-1);
        }else{Trace::getTrace().DEBUG( "Parser::getMO_MANAGED_OBJECT_FAULT, HEADER OK but no MO...");}
    }else{Trace::getTrace().DEBUG( "Parser::getMO_MANAGED_OBJECT_FAULT No header");}
    return "Not-found";
}



RWCString Parser::getAlarm_slogan_MANAGED_OBJECT_FAULT(TeMIPAlarmObjectUser* alarm_ptr){
     RWCString txt = "";
    if(alarm_ptr->getAdditionalText().IsPresent()){
        txt = alarm_ptr -> getAdditionalText().getString();
    }

    RWCString slogan = txt.match(alarmSlogan_MO_Fault_RegExp);
    if(slogan != ""){
 	Trace::getTrace().DEBUG( "Parser::getAlarm_slogan_MANAGED_OBJECT_FAUL SLOGAN =" + slogan);
	 return slogan;
    }
   
    return "NOT FOUND";
}

RWCString Parser::getMSCName(TeMIPAlarmObjectUser* alarm_ptr ){
    RWCString MSCName = "";
    RWCString managedObjectStr = alarm_ptr->getManagedObject().getStringNatural();
    RWCString MSC_Serch_Str = " MSC ";
    
    size_t endPos = managedObjectStr.length();
    size_t startPos = managedObjectStr.index(MSC_Serch_Str,0);

    if(startPos == RW_NPOS){ 
        Trace::getTrace().Error("Failed to extract MSC from managedObj. Alarm ID = " + alarm_ptr->getId().getString() + 
                                    ", managedObject = " + managedObjectStr);
        return "MSC not found";
    }
    Trace::getTrace().DEBUG(" Parser::getMSCName Found " + MSC_Serch_Str);
    startPos += MSC_Serch_Str.length();
    
    RWCString tmp = managedObjectStr(startPos,endPos - startPos);
    MSCName = tmp.strip(RWCString::both);
        
    Trace::getTrace().DEBUG(" Parser::getMSCName Found MSCName = " + MSCName);
    return MSCName;
}


RWCString Parser::getIMARegion(TeMIPAlarmObjectUser* pAlarm){
	RWCString region_oc_name;


        RWCString m_addTxtIMA = "";
        if( pAlarm->getAdditionalText().IsPresent()){
                m_addTxtIMA = pAlarm->getAdditionalText().getString();
        }else{
                Trace::getTrace().Error("IMA: Parser::getIMARegion, Additional Text is not pressent !!!");
        }

        RWCString m_defStr = RWCString("Network Element=");
        size_t m_startPos = m_addTxtIMA.index(m_defStr,0);
        m_startPos += m_defStr.length();

        RWCString regionCode =  m_addTxtIMA(m_startPos, 2);
        Trace::getTrace().DEBUG("IMA: Parser::getIMARegion, parsing IMAAddTxt = " + m_addTxtIMA);


	
	if(IMARegionMap.findValue(regionCode,region_oc_name)){
		Trace::getTrace().DEBUG("IMA: Parser::getIMARegion, returning " +region_oc_name ); 
		return region_oc_name;
	} 
	Trace::getTrace().Error("IMA: Parser::getIMARegion, region code not found in map, returning admin oc");
	return ADMIN_OC_NAME;
}



bool Parser::createExtAlarmSloganSyntax(){
   
    ifstream file(EXTERNAL_ALARM_SLOGAN_FILE);
    if(!file){
        const RWCString& errorTxt = RWCString("Failed to open EXTERNAL ALARM info File: "); 
        Trace::getTrace().Error(errorTxt + EXTERNAL_ALARM_SLOGAN_FILE);
        return false;
    }
	RWCString regexpStr = "";
	bool isFirst = true;
	while(!file.eof()){  //read all line in the file
          char  buff[128];
          file.getline(buff,128);
	  RWCString line = RWCString(buff);

	  if(line.match("^[ \t]+$") != "")continue; // If this is a line with only whitespace, skipp it
	  if(line == "") continue;			 //Empty line are skpped as well
	  line = line.strip(RWCString::both);	    // strip it
          Trace::getTrace().DEBUG("Parser::createExtAlarmSloganSyntax() Line = " + line);
	  if(!isFirst){             //The first (LINE) shall not start with |
             regexpStr += "|"; 
          }
	  regexpStr += "(" + line + ")";
	  isFirst = false;
	}
	if(isFirst){ // if the file was empty or only contins whites, set the regext to ???
	     regexpStr = "SOME_NOT_MATCHING_STRING";
        }else{
	     regexpStr = "(" + regexpStr +")"; //Add parantesses around the whole string
        }                                       // This will result in ((LINE_1)|(LINE_2)|...|(LINE_n))
	Trace::getTrace().DEBUG("Parser::createExtAlarmSloganSyntax() the regexp = " + regexpStr);
	EXTERNAL_ALARM_SLOGAN_SYNTAX_RegExp = RWCRExpr(regexpStr);

    return true;	
}


RWCString  Parser::getExternalAlarmSloganStr(TeMIPAlarmObjectUser* alarm_ptr){
    RWCString additionalText = "";
    if(alarm_ptr->getAdditionalText().IsPresent()){
        additionalText = alarm_ptr -> getAdditionalText().getString();
    }
    
    RWCString result =  additionalText.match(EXTERNAL_ALARM_SLOGAN_SYNTAX_RegExp); 
    // This will return the alarm slogan. if not found, ""
    
    if (result == "") {
        result =  EXTERNAL_ALARM_SLOGAN_FORMAT_ERR_STR;
        Trace::getTrace().DEBUG(" Parser::getExternalAlarmSloganStr: NO match, additoinalext = " + additionalText);
    }
    Trace::getTrace().DEBUG(" Parser::getExternalAlarmSloganStr returning: " + result);
    return result;
}     

#endif




