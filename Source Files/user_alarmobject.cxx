//
//
//
//                   ****  COPYRIGHT NOTICE ****
//
//==========================================================================//
//                      Code of the TeMIPAlarmObjectUser Class              //
//==========================================================================//
#ifndef _EXP_user_alarmobject_cxx
#define _EXP_user_alarmobject_cxx

#include "user_alarmobject.hxx"


const TEMIP_T_Unsigned32 EXP_K_CLEARANCE_TIME_STAMP = 52;
const TEMIP_T_Unsigned32 EXP_K_ARG_SHOW_VALUES = 1;

RWCString TeMIPAlarmObjectUser::LIST_FILE_NAME = RWCString("CFG_FILE_NOT_FOUND");
RWCString TeMIPAlarmObjectUser::SGSN_FILE_NAME = RWCString("CFG_FILE_NOT_FOUND");

TeMIPAlarmObjectUser::TeMIPAlarmObjectUser(EXPPopulationRequest * pr):TeMIPAlarmObject(pr),
    primaryAlarmId(0),
    TC_SpecificProblem(SP_UNDEFINED),
    ext_alarm_slogan(ALARM_SLOGAN_UNDEFINED),
    //hasSecondaryAlarms(false),
    isHidden(false),
    _ruleState(UNSPECIFIED_RULE_STATE),
    real_additionalText(),
    real_userText(),
    creatingListAlarm(false),
    myListAlarmID(-1),
    BTSName(""),
    BSCName(""),
 
    isHandledByRule10(false),
    IMANewOC(""),
    _isIMARegionAO(false),
    handled_by_IMA_sync(false),
    _isIMAAO(false),
    _isAdminAO(false),
    GPRSFile(""),
    TrunkId(""),
    m_clearanceTimeStamp(TEMIP_K_NULL_PTR),
    tmp_alarmTypeCheckText(""),
    tmp_R8_extractAlarmType(""),
    tmp_BSCName(""),
    tmp_CELLName(""),
    tmp_xsystemResult(""),
    tmp_NodeName("")
    
{
    Trace::getTrace().DEBUG ( RWCString("One Alarm was created, Starttime = ") + Constants::startTime.asString() ); 
    bool stringFound = Configuration::getConfig().getStringValue(CFG_Entry_Const_str_HIDE_STRING,_HIDE_STRING);
    if(!stringFound){
        _HIDE_STRING = RWCString("ERR: Hide-string NOT found in cfg-file");
    }
    bool result1 false;
    RWCString key1 = CFG_Entry_LOADSGSNFILE_CONFIG_FILE;
    result1 = Configuration::getConfig().getStringValue(key1,SGSN_FILE_NAME);
    if(!result1) Trace::getTrace().Error("Failed to find value from configFile: " + key1 + ", Using default: " + SGSN_FILE_NAME);
}

bool TeMIPAlarmObjectUser::operator ==  (const TeMIPAlarmObjectUser& rhs)const{
    bool result =  (*(getId().getTUnsigned32()) == *(rhs.getId().getTUnsigned32()));
    return result;
}

TeMIPAlarmObjectUser::~TeMIPAlarmObjectUser()
{
    //secondaryAlarms.removeAll();
    if (m_clearanceTimeStamp) delete m_clearanceTimeStamp;
    
}

//This macro will permit
// to perform IsPresent() in the rules to test whether it is valued
// like for most of the TeMIPAlarmObject slots
const EXPAbsTime& TeMIPAlarmObjectUser::getClearanceTimeStamp() const RETURN_OBJECT_OR_SENTINEL(m_clearanceTimeStamp,EXPAbsTime)



const RWCString TeMIPAlarmObjectUser::getHideString(){
    return _HIDE_STRING;
}

void TeMIPAlarmObjectUser::setHideString(){
    RWCString userText = "";
    //if(getUserText().IsPresent()){
        userText = safeGetUserText();  //.getString();
    //}
  userText  = _HIDE_STRING + "\n" + userText;
   safeSetUserText(userText);
}

void  TeMIPAlarmObjectUser::removeHideString(){

   if(real_userText.match(_HIDE_STRING +"\n") != ""){
        real_userText.replace(_HIDE_STRING + "\n","");
        SetUserText(real_userText);
        //CHNO
        Trace::getTrace().DEBUG("TeMIPAlarmObjectUser:removeHideString, AlarmID = "+ getId().getString() +" Setting UT = " + real_userText);
        Trace::getTrace().DEBUG("HIDE String was found and removed");
   }else{
        Trace::getTrace().DEBUG("HIDE String was NOT Found !!!");
   }
   //setUserText(real_userText);
   //safeSetUserText(userText);

}

RWCString TeMIPAlarmObjectUser::getExternalAlarmSloganStr(){
	RWCString result = parser.getExternalAlarmSloganStr(this);
	if(result == "") result = "ERROR ...";
	return result;
}

void TeMIPAlarmObjectUser::addSecondaryAlarm(TeMIPAlarmObjectUser* secAlarm){
    secondaryAlarms.insert(secAlarm);
}

void  TeMIPAlarmObjectUser::appendSecondaryAlarmToAdditionalText(TeMIPAlarmObjectUser* secAlarm){
    RWCString textToadd = "";
    if(secAlarm->getAdditionalText().IsPresent()){
        textToadd = secAlarm->getAdditionalText().getString();
     }
    
    Trace::getTrace().DEBUG(" TeMIPAlarmObjectUser::appendSec... TextBefore: " +   real_additionalText);

    safeSetAdditionalText( real_additionalText + "\n" + textToadd);
        
    Trace::getTrace().DEBUG(" TeMIPAlarmObjectUser::appendSec... TextAfter: " +  real_additionalText);
}
 
RWCString TeMIPAlarmObjectUser::gotOVOStr()
{
     RWCString sonuc=RWCString("No Value Found!");
     RWCString secenek1=RWCString("LS             SPID");
     if ( theFile.m_success )
     {
      RWCString addText4 = getAdditionalText().getString();
      RWCString usertext4 = theFile.FindSPIDForOVO(addText4, secenek1);
      Trace::getTrace().DEBUG(" TeMIPAlarmObjectUser::gotOVOStr... " +  usertext4);
      return (const char*) usertext4;
     }
return sonuc;
}


void TeMIPAlarmObjectUser::gotDXXNodeIdName()
{
       RWCString usertext1;
       RWCString addText = real_additionalText;
       RWCString manObject = getManagedObject().getString();
       size_t pos1 = manObject.first("_");
       size_t pos2 = manObject.index("_",pos1 +1);
       size_t lengt = manObject.length();
       RWCString num = manObject(pos2+1,lengt-pos2-1);
//       usertext1 = theFile.LoadDXXIdName(num, filename);
       usertext1 = theFile.LoadDXXIdNamefromMap(num, true);
       addText = usertext1 + " " + addText;
       SetAdditionalText(addText);
}



RWCString TeMIPAlarmObjectUser::gotDXXAddIdent()
{
       RWCString addText = real_additionalText;
       size_t pos1 = addText.index("TrunkId=");
       size_t lengt = addText.mbLength();
       pos1+=8;
       RWCString addIdent = addText(pos1, lengt - pos1-1);
       addIdent = " TId=" + addIdent;
       return addIdent;
}

void TeMIPAlarmObjectUser::gotDXXCircuitName()
{
   if ( theFile.m_success )
   {
       RWCString usertext1;
       RWCString addText = real_additionalText;
       RWCString manObject = getManagedObject().getString();
       size_t pos1 = manObject.first("_");
       size_t pos2 = manObject.index("_",pos1 +1);
       size_t lengt = manObject.length();
       RWCString num = manObject(pos2+1,lengt-pos2-1);
       usertext1 = theFile.LoadDXXIdNamefromMap(num, true);
       RWCString bbbbb = theFile.LoadDxxTrunkid(addText);
       bbbbb = usertext1 + bbbbb;
       SetAdditionalText(bbbbb);
   }
}




RWCString TeMIPAlarmObjectUser::getPortLabel(const char *filenam) {
    RWCString addtext = real_additionalText;
    RWCString fileName=RWCString(filenam);
    RWCString networkElement= RWCString("Network Element=");
    size_t pos1=addtext.index(networkElement);
    pos1+=networkElement.length();
    RWCString nElementName=addtext(pos1,10);
    RWCString shelfinfo = RWCString("Port Shelf ");
    size_t pos3=addtext.index(shelfinfo);
    pos3+=shelfinfo.length();
    RWCString sourcenr=RWCString(" ;SourceNr=");
    size_t pos4=addtext.index(sourcenr);
    RWCString shelf= addtext(pos3, pos4-pos3);
    RWCString card= RWCString("Card=STM-1ElectricT2U_");
    size_t pos5=addtext.index(card);
    pos5+=card.length();
    RWCString cardno=addtext(pos5,1);
    RWCString shelfCard=shelf+RWCString("-")+cardno;
    RWCString portLabel = theFile.LoadPortLabelFromFile(nElementName,shelfCard,fileName);

return portLabel;
}
 
bool TeMIPAlarmObjectUser::findString(const char *str)
{
        RWCString addText = getAdditionalText().getString(); 
        if ( addText.contains(str) )
                return true;
        else
                return false;
}

bool TeMIPAlarmObjectUser::findManObject(const char *str1)
{
        RWCString manObject = getManagedObject().getString();
        if ( manObject.contains(str1) )
                return true;
        else
                return false;
}

void TeMIPAlarmObjectUser::gotVectValue(int fileNum, const char *str2)
{
        RWCString addText = real_additionalText;
        RWCString testtext = RWCString(str2);
        RWCString txt2=RWCString("not loaded");
        bool result false;
        RWCString key = CFG_Entry_LOADFROMFILE_CONFIG_FILE;
        result = Configuration::getConfig().getStringValue(key,LIST_FILE_NAME);
        if(!result) Trace::getTrace().Error("Failed to find value from configFile: " + key + ", Using default: " + LIST_FILE_NAME);
        if ( theFile.m_success )
         {
          RWCString bbbbb = theFile.LoadFromFile(addText, fileNum, testtext, LIST_FILE_NAME);
          SetAdditionalText(bbbbb);
         }
        else
         {
          SetAdditionalText(txt2);
         }
}

RWCString TeMIPAlarmObjectUser::gotBlockSup(const char *opsiyon)
{
       RWCString usertext1, usertext2;
       RWCString addText3 = real_additionalText;
       RWCString secenek=RWCString(opsiyon);
       RWCString secenek1=RWCString("Rule11");
       RWCString secenek2=RWCString("Rule15");
       if (secenek == secenek1)
       {
       usertext1 = RWCString(" (R#11)");
       usertext2 = theFile.FindENDString(addText3, secenek);
       usertext2 += usertext1;
       return usertext2;
       }
       if (secenek == secenek2)
       {
       usertext1 = RWCString(" (R#15)");
       usertext2 = theFile.FindENDString(addText3, secenek);
       usertext2 += usertext1;
       return usertext2;
       }
       return usertext2;
}

RWCString TeMIPAlarmObjectUser::gotCCITT7(const char *opsiyon)
{  
       RWCString secenek=RWCString(opsiyon);
       RWCString sonuc=RWCString("No Value Found!") ;
       RWCString secenek1=RWCString("LS             SPID");
       RWCString secenek2=RWCString("DEST           SPID");
   if ( theFile.m_success )
   {
     if ( secenek == secenek1)
     {  
       RWCString addText4 = getAdditionalText().getString(); 
       RWCString usertext4 = theFile.FindSPIDString(addText4, secenek1);
       return (const char*) usertext4;
     }
     if ( secenek == secenek2)
     { 
       RWCString addText4 = getAdditionalText().getString();
       RWCString usertext4 = theFile.FindSPIDString(addText4, secenek2);
     
       return (const char*) usertext4;
     }
   }
return sonuc; 
}

//Rule10+
RWCString TeMIPAlarmObjectUser::extractAlarmTypeCheckText(const char* filename1, const char* filename2)
{
        size_t pos_start, pos_end, pos_emg;
        RWCString addText = real_additionalText;
        RWCString severity = getSeverity().getString();
        RWCString severitymajor = RWCString("Major");
        RWCString severitycritical = RWCString("Critical");
        RWCString secenek3=RWCString("Rule10");
        RWCString usertext = RWCString("");
        RWCString usertext1 = theFile.FindENDString(addText, secenek3);
        size_t boy = usertext1.length();
        if (severity == severitycritical)
        {
        RWCString dosyaismi = RWCString(filename1);
        ifstream file1(dosyaismi);
        if (!file1){
           tmp_alarmTypeCheckText = usertext1;
           Trace::getTrace().DEBUG(" extractAlarmTypeCheckText (!file1) returning:\"" + usertext1 +"\"");
           return tmp_alarmTypeCheckText;
           }
        RWTValVector<RWCString> utext(10);
        size_t k=0;
        size_t indis=1, indis2=0;
        RWCString textt;
        while ( (usertext1((indis+2),3) != "END") )
        {
        indis=usertext1.index("\n",indis2);
        textt = usertext1(indis2, indis - indis2);
        utext[k] = textt.replace(RWCRExpr("    "), "" ,RWCString::all);
        textt = utext[k];
        indis2=indis+2;
        ++k;
        }
        size_t length_arr = k-1;
        if (length_arr > 0)
         {
           int m=0;
           RWCString deneme;
           RWCString ut1 = utext[0];
           RWCString ut2 = utext[1];
           RWCString letter11 = ut1(m+1,1);
           RWCString letter12 = ut2(m,1);
         while ( letter11 == letter12 )
             {
               ++m;
               letter11 = ut1(m+1,1);
               letter12 = ut2(m,1);
             }
         if ( m > 0)
          {
            pos_emg = m - 1;
            deneme=utext[0];
            int j=1;
            while ( j <= length_arr )
            {
            size_t logth=utext[j].length();
            RWCString deneme1=utext[j](pos_emg,logth-pos_emg);
           deneme = deneme + deneme1;
            utext[j]=deneme1;
            ++j;
            }
          }
          else
           {  pos_emg = 0;
              deneme =utext[0];
              int j=1;
            while ( j <= length_arr )
            {
            size_t logth=utext[j].length();
            RWCString deneme1=utext[j](0,logth);
            deneme = deneme + " " + deneme1;
            utext[j]=deneme1;
            ++j;
            }
          }
         usertext=deneme;
        }
        else { usertext=utext[0];}
        for ( int j=0; j<= (k-1);j++)
        {
        RWCString str2=utext[j];
        char buf[30];
        RWCString str1;
        RWCString check=RWCString("not_existing");
        ifstream file2(dosyaismi);
        while (!file2.eof())
          {
          file2.getline(buf,30);
          str1=buf;
          if (str1 != "")
           {
          if (str2.contains(str1))
            {
             check=RWCString("existing");
            }
           }
          }
        if ( check == "not_existing" )
         {
           tmp_alarmTypeCheckText = usertext + RWCString(" (warning)");
           Trace::getTrace().DEBUG(" extractAlarmTypeCheckText (check == not_existing) returning:\"" + tmp_alarmTypeCheckText +"\"");
           return tmp_alarmTypeCheckText;
         }
        }
     
        tmp_alarmTypeCheckText = usertext;
        Trace::getTrace().DEBUG(" extractAlarmTypeCheckText returning:\"" + tmp_alarmTypeCheckText +"\"");
        return tmp_alarmTypeCheckText;
        }
        else if(severity == severitymajor)
        {
        RWCString dosyaismi = RWCString(filename2);
        ifstream file1(dosyaismi);
        if (!file1){
           Trace::getTrace().DEBUG(" extractAlarmTypeCheckText (!file1) returning:\"" + usertext1 +"\"");	
           tmp_alarmTypeCheckText = usertext1;
           return tmp_alarmTypeCheckText;
        }
        RWTValVector<RWCString> utext1(10);
        size_t k=0;
        size_t indis=1, indis2=0;
        RWCString textt;
        while ( (usertext1((indis+2),3) != "END") )
        {
        indis=usertext1.index("\n",indis2);
        textt = usertext1(indis2, indis -  indis2);
        utext1[k] = textt.replace(RWCRExpr("    "), "" ,RWCString::all);
        textt = utext1[k];
        indis2=indis+2;
        ++k;
        }
        size_t length_arr = k-1;
        if (length_arr > 0)
         {
           int m=0;
           RWCString deneme;
           RWCString ut1 = utext1[0];
           RWCString ut2 = utext1[1];
           RWCString letter11 = ut1(m+1,1);
           RWCString letter12 = ut2(m,1);
         while ( letter11 == letter12 )
             {
               ++m;
               letter11 = ut1(m+1,1);
               letter12 = ut2(m,1);
             }
         if ( m > 0)
          {
            pos_emg = m - 1;
            deneme=utext1[0];
            int j=1;
            while ( j <= length_arr )
            {
            size_t logth=utext1[j].length();
            RWCString deneme1=utext1[j](pos_emg,logth-pos_emg);
           deneme = deneme + deneme1;
            utext1[j]=deneme1;
            ++j;
            }
          }
          else
           {  pos_emg = 0;
              deneme =utext1[0];
              int j=1;
            while ( j <= length_arr )
            {
            size_t logth=utext1[j].length();
            RWCString deneme1=utext1[j](0,logth);
            deneme = deneme + " " + deneme1;
            utext1[j]=deneme1;
            ++j;
            }
          }
         usertext=deneme;
        }
        else { usertext=utext1[0];}
        for ( int j=0; j<= (k-1);j++)
        {
        RWCString str2=utext1[j];
        char buf[30];
        RWCString str1;
        RWCString check=RWCString("not_existing");
        ifstream file2(dosyaismi);
        while (!file2.eof())
          {
          file2.getline(buf,30);
          str1=buf;
          if (str1 != "")
           {
          if (str2.contains(str1))
            {
             check=RWCString("existing");
            }
           }
          }
         if ( check == "not_existing" )
         {
           tmp_alarmTypeCheckText = usertext + RWCString(" (warning)");
        //   SetUserText(sonuc);   
           Trace::getTrace().DEBUG(" RULE#10 checkalarmtype:" + tmp_alarmTypeCheckText);
           Trace::getTrace().DEBUG(" extractAlarmTypeCheckText (check == \"not_existing\") returning:\"" + tmp_alarmTypeCheckText +"\"");	
           return tmp_alarmTypeCheckText;
         }
        }
        Trace::getTrace().DEBUG(" RULE#10 checkalarmtype:" + usertext);
     //   SetUserText(usertext);
       //RWCString *returnStr = new  RWCString(usertext);
     	Trace::getTrace().DEBUG(" extractAlarmTypeCheckText  returning:\"" + usertext +"\"");	
        tmp_alarmTypeCheckText = usertext;
        return tmp_alarmTypeCheckText;
        }
tmp_alarmTypeCheckText += RWCString(" (warning)");
Trace::getTrace().DEBUG(" extractAlarmTypeCheckText  returning:\"" + tmp_alarmTypeCheckText +"\"");
return tmp_alarmTypeCheckText;
}



bool TeMIPAlarmObjectUser::removeSecondayAlarm(TeMIPAlarmObjectUser* secAlarm){
   
    TeMIPAlarmObjectUser* result = secondaryAlarms.remove(secAlarm);
    return result != NULL;
   
}

TeMIPAlarmObjectUser* TeMIPAlarmObjectUser::getSecondaryAlarmAt(int index){
    return secondaryAlarms[index];
}

int TeMIPAlarmObjectUser::getSecondaryAlarmCount(){
    int count = secondaryAlarms.entries();
    return count;
}

bool  TeMIPAlarmObjectUser::removeFromAlarmText(TeMIPAlarmObjectUser* secAlarm){
    if(!secAlarm->getAdditionalText().IsPresent()) return false;

    RWCString textToRemove = secAlarm->getAdditionalText().getString();
    size_t pos = real_additionalText.index(textToRemove);
    if(pos == RW_NPOS){
        return false;
    }
    Trace::getTrace().DEBUG(" TeMIPAlarmObjectUser::removeFromAlarmText Removing text:" +  textToRemove );
    Trace::getTrace().DEBUG(" TeMIPAlarmObjectUser::removeFromAlarmText orgTxt before= " +real_additionalText );
   

    real_additionalText.remove(pos-1,textToRemove.length()+1);
    safeSetAdditionalText(real_additionalText); 
    
    Trace::getTrace().DEBUG(" TeMIPAlarmObjectUser::removeFromAlarmText  Txt after = "  + real_additionalText);
    Trace::getTrace().DEBUG(" TeMIPAlarmObjectUser::removeFromAlarmText _Txt after = "  + _additionalText->getString() );
    return true;
}


int TeMIPAlarmObjectUser::getTC_SpecificProblemInt(){
    return (int) TC_SpecificProblem;
}
int TeMIPAlarmObjectUser::getExternal_Alarm_SloganInt(){
    return (int)  ext_alarm_slogan;
}

unsigned long  TeMIPAlarmObjectUser::getPrimaryAlarmId(){return primaryAlarmId;}

void TeMIPAlarmObjectUser:: setPrimaryAlarmId(unsigned long id){primaryAlarmId = id;}

bool TeMIPAlarmObjectUser::getHasSecondary(){  return !secondaryAlarms.isEmpty();}

bool  TeMIPAlarmObjectUser::getIsHidden(){return isHidden;}

void  TeMIPAlarmObjectUser::setIsHidden(bool hide){isHidden = hide;}
    
int  TeMIPAlarmObjectUser::getRuleState(){return _ruleState;}

void  TeMIPAlarmObjectUser::setRuleState(int newState){_ruleState = newState;}


void TeMIPAlarmObjectUser::OnPostBuild()
{
    isOld = false;
    if(_eventTime->getTAbsTime()->GetValue() < Constants::startTime){
	Trace::getTrace().DEBUG("TeMIPAlarmObjectUser::OnPostBuild: this is an old alarm ");
	isOld = true;
    }
     if(getAdditionalText().IsPresent()){
         real_additionalText = _additionalText->getString();
     }
    if(getUserText().IsPresent()){
	real_userText = getUserText().getString();
    }
    if(isCleared())  setClearanceTimeStampFromTeMIPCall();

    if(setIsAdminAO()){
	Trace::getTrace().DEBUG("TeMIPAlarmObjectUser::onPostBuild, Admin Alarm");	
    }
    else if(setIsIMAAlarm()){
	if(!_isIMARegionAO){ 
		parseIMAAlarm();
		Trace::getTrace().DEBUG("TeMIPAlarmObjectUser::onPostBuild, IMA: RAW AO, new OC = " + IMANewOC);
	}else{
		Trace::getTrace().DEBUG("TeMIPAlarmObjectUser::onPostBuild, IMA: Region AO");
	}
    }else{
    	parseOSSAlarm();
    }
}

void TeMIPAlarmObjectUser::OnPostUpdateCleared(){
    setClearanceTimeStampFromTeMIPCall();   
}


// Perform the "show operation_context <myOC> alarm_object <ao> Clearance Time Stamp" to get the value.
void TeMIPAlarmObjectUser::setClearanceTimeStampFromTeMIPCall()
{


   Trace::getTrace().DEBUG("TeMIPAlarmObjectUser::setClearanceTimeStampFromTeMIPCall");
//A TAL sychronous call is used
    TSyncCall showCall(TEMIP_K_VERB_SHOW,
                       *getAlarmObject().getTEntitySpec(),
                       TEMIP_K_PRT_STATUS);
    showCall.MakeCall();
    const TReply & showReply = showCall.GetReply();
    Trace::getTrace().DEBUG("TeMIPAlarmObjectUser::setClearanceTimeStampFromTeMIPCall: showReply = " + showReply.ToString());
    
   if(showReply.GetReplyType() == TAL_K_RESPONSE){
       const TAttributeList & argShowValues = 
           (const TAttributeList&)showReply[EXP_K_ARG_SHOW_VALUES];
       
       if(argShowValues.IsPresent()){    
            const TAbsTime& timeStamp = (const TAbsTime&)argShowValues[52]; 
            Trace::getTrace().DEBUG("TeMIPAlarmObjectUser::setClearanceTimeStampFromTeMIPCall: timeStamp = " 
                                    + timeStamp.ToString());

            if(timeStamp.IsPresent()){
                m_clearanceTimeStamp = new EXPAbsTime(new TAbsTime(timeStamp), 
                                                      TRUE);
                Trace::getTrace().DEBUG("TeMIPAlarmObjectUser::setClearanceTimeStampFromTeMIPCall: TimeStamp = " 
                                        +  m_clearanceTimeStamp->getString());
            } 
            else{
                Trace::getTrace().DEBUG("TeMIPAlarmObjectUser::setClearanceTimeStampFromTeMIPCall: Problem with call execution! \"SHOW " +
                                        getAlarmObject().getString() + 
                                        " Clearance Time Stamp\" " );
                        
            }
        } 
    }

    Trace::getTrace().DEBUG("TeMIPAlarmObjectUser::setClearanceTimeStampFromTeMIPCall: DONE");
}



void  TeMIPAlarmObjectUser::parseOSSAlarm()
{
    TC_SpecificProblem = parser.getSP(this);
    
    if(TC_SpecificProblem ==  SP_UNDEFINED) return;
    RWCString severity =this->getSeverity().getString();
    Trace::getTrace().DEBUG("Parsed one alarm");
    
    RWCString key = "";
    bool result;
    RWCString manObject = getManagedObject().getString();
    RWCString bts_str = RWCString(" BTS ");
    switch( TC_SpecificProblem){
   
    case FILE_PROCESS_UTILITY_AUTOMATIC_TRANSFER_FAILURE:
	if(!Configuration::getConfig().isRuleActive(16)){
		 TC_SpecificProblem = SP_UNDEFINED;
		break;
	}
        Trace::getTrace().DEBUG("TeMIPAlarmObjectUser::parseOSSAlarm FILE_PROCESS_UTILITY_AUTOMATIC_TRANSFER_FAILURE");
        key = CFG_Entry_FILE_PROCESS_UTILITY_TransientTime;
        result = Configuration::getConfig().getIntValue(key,transientWaitingTime);
        if(!result){ 
            transientWaitingTime = 60 * 30;
            Trace::getTrace().DEBUG("TeMIPAlarmObjectUser::parseOSSAlarm(): Failed to get Config value for:" + key);
        }

        _ruleState = NOT_TRANSIENT;

	break;


 
    case RADIO_X_CEIVER_ADMINISTRATION_MANAGED_OBJECT_FAULT:

	if(!Configuration::getConfig().isRuleActive(10)){
		 TC_SpecificProblem = SP_UNDEFINED;
		break;
	}

        Trace::getTrace().DEBUG("TeMIPAlarmObjectUser::parseOSSAlarm RADIO_X_CEIVER_ADMINISTRATION_MANAGED_OBJECT_FAULT");
        parseEXTERNAL_ALARM();
        if(ext_alarm_slogan == AC_FAILURE){
        key = CFG_Entry_BTS_EXTERNAL_FAULT_AC_FAIL_TransientTime;
        Trace::getTrace().DEBUG("TeMIPAlarmObjectUser::parseOSSAlarm RADIO_X_CEIVER_ADMINISTRATION_MANAGED_OBJECT_FAULT AC FAILURE");
        }
        else {
        Trace::getTrace().DEBUG("TeMIPAlarmObjectUser::parseOSSAlarm RADIO_X_CEIVER_ADMINISTRATION_MANAGED_OBJECT_FAULT MAINS FAILURE");
        }
        result = Configuration::getConfig().getIntValue(key,transientWaitingTime);
        if(!result){ 
            transientWaitingTime = 60 * 5;
            Trace::getTrace().DEBUG("TeMIPAlarmObjectUser::parseOSSAlarm(): Failed to get Config value for:" + key);
        }
        Trace::getTrace().DEBUG("TeMIPAlarmObjectUser::parseOSSAlarm(): RADIO_X_CEIVER_ADMINISTRATION_MOFAULT -DONE");

        if(ext_alarm_slogan == AC_FAILURE){
                _ruleState =  SEND_TO_TRANSIENT_FILTER;
        }else if(ext_alarm_slogan == MAINS_FAILURE){
                _ruleState =  SEND_TO_TRANSIENT_FILTER;
        }else{
                _ruleState = NOT_TRANSIENT;
        }

	break;
   
    case  RADIO_X_CEIVER_ADMINISTRATION_BTS_EXTERNAL_FAULT:

        if(!Configuration::getConfig().isRuleActive(17)){
                 TC_SpecificProblem = SP_UNDEFINED;
                break;
        }
        if(getManagedObject().getStringNatural().match(" BSC ") == ""){
                Trace::getTrace().DEBUG(" This is NOT a  BSC !!! mo = " + getManagedObject().getStringNatural() );
                TC_SpecificProblem = SP_UNDEFINED;
                break;
        }
        Trace::getTrace().DEBUG(" TeMIPAlarmObjectUser::parseOSSAlarm RADIO_X_CEIVER_ADMINISTRATION_BTS_EXTERNAL_FAULT");
        parseEXTERNAL_ALARM();
        if(ext_alarm_slogan == AC_FAILURE){
        key = CFG_Entry_BTS_EXTERNAL_FAULT_TransientTime;
        Trace::getTrace().DEBUG(" TeMIPAlarmObjectUser::parseOSSAlarm RADIO_X_CEIVER_ADMINISTRAT... AC FAILURE");
        }
        else {
        Trace::getTrace().DEBUG(" TeMIPAlarmObjectUser::parseOSSAlarm RADIO_X_CEIVER_ADMINISTRAT... MAINS FAILURE");
        }
        result = Configuration::getConfig().getIntValue(key,transientWaitingTime);
        if(!result){
                transientWaitingTime = 60 * 5;
                Trace::getTrace().DEBUG("TeMIPAlarmObjectUser::parseOSSAlarm(): Failed to get Config value for:" + key);
        }
       
        if(ext_alarm_slogan == AC_FAILURE){
                _ruleState =  SEND_TO_TRANSIENT_FILTER;
        }else if(ext_alarm_slogan == MAINS_FAILURE){
                _ruleState =  SEND_TO_TRANSIENT_FILTER;
        }else{
                _ruleState = NOT_TRANSIENT;
        }

        break;



    case EXTERNAL_ALARM:

       if(!Configuration::getConfig().isRuleActive(8)){
                 TC_SpecificProblem = SP_UNDEFINED;
                break;
        }

        if((getManagedObject().getStringNatural().match(" MSC ") != "") || (getManagedObject().getStringNatural().match(" TSC ") != "") || (getManagedObject().getStringNatural().match(" HLR ") != ""))
        {
                Trace::getTrace().DEBUG(" This is SS Node !!! mo = " + getManagedObject().getStringNatural() );
                TC_SpecificProblem = EXTERNAL_ALARM;
                break;
        }
        Trace::getTrace().DEBUG(" TeMIPAlarmObjectUser::parseOSSAlarm EXTERNAL_ALARM");
        if(ext_alarm_slogan == AC_FAILURE){
        key = CFG_Entry_BTS_EXTERNAL_FAULT_AC_FAIL_TransientTime;
        }
        else {
        key =  CFG_Entry_BTS_EXTERNAL_FAULT_AC_FAIL_TransientTime;
        }
        result = Configuration::getConfig().getIntValue(key,transientWaitingTime);
        if(!result){
                transientWaitingTime = 60 * 5;
                Trace::getTrace().DEBUG("TeMIPAlarmObjectUser::parseOSSAlarm(): Failed to get Config value for:" + key);
        }
        parseEXTERNAL_ALARM();
        if(ext_alarm_slogan == AC_FAILURE && !(manObject.contains(bts_str))){
                _ruleState =  SEND_TO_TRANSIENT_FILTER;
        }else if(ext_alarm_slogan == MAINS_FAILURE && !(manObject.contains(bts_str))){
                _ruleState =  SEND_TO_TRANSIENT_FILTER;
        }else{
                _ruleState = NOT_TRANSIENT;
        }
        break;

 
    case SP_UNDEFINED:
        Trace::getTrace().DEBUG("TeMIPAlarmObjectUser::parseOSSAlarm Alarm is UNDEF, Not interresting");
        waitingTime2 = 0;
        transientWaitingTime = 0;

    }
    
}
void TeMIPAlarmObjectUser::parseIMAAlarm(){
	IMANewOC = parser.getIMARegion(this);
}
bool TeMIPAlarmObjectUser::setIsAdminAO(){
	if(_isAdminAO) return true;

	const EXPLatin1String oc_name_latinStr =  getOcName();
	RWCString oc_name = RWCString(oc_name_latinStr.getString());
	return (_isAdminAO = oc_name.contains(ADMIN_OC_NAME));
}

bool TeMIPAlarmObjectUser::setIsIMAAlarm(){
	if(_isIMAAO) return true;

	const EXPLatin1String oc_name_latinStr =  getOcName();
	RWCString oc_name = RWCString(oc_name_latinStr.getString());
	

	if(_isIMAAO = oc_name.contains(IMA_RAW_OC_NAME)){
		Trace::getTrace().Error("TeMIPAlarmObjectUser::setIsIMAAlarm(), This is a RAW AO !!!");
		 return true; // this is a raw AO	
	}
	//_isIMARegionAO = oc_name.contains("OC_[A-Z]*_SDH");    // this is not a raw AO
	_isIMARegionAO =  oc_name.contains("OC_ISE_SDH") ||
			oc_name.contains("OC_LAB") ||
			oc_name.contains("OC_PROD") ||
			oc_name.contains("OC_TEST");

	if(_isIMARegionAO){
		Trace::getTrace().Error("TeMIPAlarmObjectUser::setIsIMAAlarm(), This is a REGION AO !!!");
	}

	_isIMAAO = _isIMARegionAO || _isIMAAO;
	return _isIMAAO;
}

void  TeMIPAlarmObjectUser::parseEXTERNAL_ALARM(){
    ext_alarm_slogan = parser.get_alarm_slogan(this);
    Trace::getTrace().DEBUG("Parsed one EXTERNAL_ALARM alarm");
    switch(ext_alarm_slogan ){

    case MAINS_FAILURE:
	 Trace::getTrace().DEBUG("TeMIPAlarmObjectUser::parseEXTERNAL_ALARM() Found MAINS_FAILURE ");
	break;

    case AC_FAILURE:
	 Trace::getTrace().DEBUG("TeMIPAlarmObjectUser::parseEXTERNAL_ALARM() Found AC_FAILURE ");
	break;

    case  ALARM_SLOGAN_UNDEFINED:
	 Trace::getTrace().DEBUG("TeMIPAlarmObjectUser::parseEXTERNAL_ALARM() Found  ALARM_SLOGAN_UNDEFINED");
    }

}

void  TeMIPAlarmObjectUser::extractDIPInfo(){
    other_DIP = "OTHER DIP NOT FOUND";
    other_MSC =  "OTHER MSC NOT FOUND";  
    other_link = "";   

    DIP = parser.getDIP(this);
    bool DIPFound = (DIP != "");
    if(!DIPFound)DIP = "NOT FOUND";   


    MSC = parser.getMSCName(this);
    bool MSCNameFound = (MSC != "");
    if(!MSCNameFound) MSC = "NOT FOUND";
    
    if(DIPFound && MSCNameFound){
    	DIPInfo info = DIPHandler::getRef()->findInfoFor(MSC,DIP);
    	other_DIP = info.DIP;
    	other_MSC = info.SWITCH;  
    	other_link = info.link;
    }
}

void  TeMIPAlarmObjectUser::extractOtherEndDIPInfo(){

}


void  TeMIPAlarmObjectUser::parseBTS_EXTERNAL(){
	//Parser::get_alarm_slogan(this )
}

// LAB 
RWCString TeMIPAlarmObjectUser::getBSC()
{
	RWCString manObject1 = getManagedObject().getString();
	Trace::getTrace().DEBUG(" getBSC(): returning: " + tmp_BSCName);
	return tmp_BSCName;
}

RWCString TeMIPAlarmObjectUser::getNodeName(){
        
        RWCString NodeName = getManagedObject().getString();
        RWCString Search_Str = " AXE ";
        RWCString arti4 = "XXXX";
        size_t EndPosition = NodeName.length();
        size_t StartPosition = NodeName.index(Search_Str,0);
        if ( StartPosition == RW_NPOS )
        {
         Search_Str = " axe ";
         StartPosition = NodeName.index(Search_Str,0);
        }
        StartPosition += Search_Str.length();
        StartPosition += arti4.length();

        RWCString tmp = NodeName(StartPosition,EndPosition - StartPosition);
        tmp_NodeName = tmp.strip(RWCString::both);

        return tmp_NodeName;
}

void TeMIPAlarmObjectUser::reloadSGSN(){FileHandler::getRefSGSN(SGSN_FILE_NAME)->refreshSGSN(SGSN_FILE_NAME);}
//void TeMIPAlarmObjectUser::reloadSGSN(){FileHandler::getRefSGSN()->refreshSGSN();}

RWCString TeMIPAlarmObjectUser::getSGSNNode()
{
        RWCString m_key = getBSC();
        //RWCString m_SGSNName = theSGSNFile.getSGSNNodeName(m_key);
        RWCString m_SGSNName = FileHandler::getRefSGSN(SGSN_FILE_NAME)->getSGSNNodeName(m_key);
        //RWCString m_SGSNName = FileHandler::getRefSGSN()->getSGSNNodeName(m_key);
        return (const char*) m_SGSNName;
}


RWCString TeMIPAlarmObjectUser::getCustomAddText(const char *pStartStr, const char *pEndStr){
        RWCString m_additionalTxt = getAdditionalText().getString();
        RWCString Error = RWCString("Search String Not Found");
        RWCString m_startStr = (const char *) pStartStr;
        RWCString m_endStr = (const char *) pEndStr;

        size_t m_startPos = m_additionalTxt.index(m_startStr,0);
        if(m_startPos == RW_NPOS) {
                Trace::getTrace().Error("OSS: TeMIPAlarmObjectUser::getCustomAddText, No first string in Add Text");
                return Error;
        }
        size_t m_endPos = m_additionalTxt.index(m_endStr,0);
        if(m_endPos == RW_NPOS) {
                Trace::getTrace().Error("OSS: TeMIPAlarmObjectUser::getCustomAddText, No first string in Add Text");
                return Error;
        }

        m_startPos += m_startStr.length() +1;

        RWCString m_desiredStr1 = m_additionalTxt(m_startPos, m_endPos - m_startPos );
 //       RWCString m_desiredStr = m_desiredStr1.strip(RWCString::both);

        return m_desiredStr1;
}


RWCString TeMIPAlarmObjectUser::cmdexec(const char *str)
{
        int i;      
        i = system(str);
        RWCString sonuc1=RWCString("Command run succesfully");
        RWCString sonuc2=RWCString("Error executing command");
        if (i==-1)
        {
          return sonuc2;
        }
        else
        {
          return sonuc1;
        }
}

RWCString TeMIPAlarmObjectUser::xsystem(char *cmd)
{
  
  tmp_xsystemResult = "xsystem Error";
  FILE *fp;

  if((fp = popen(cmd,"r")) != NULL)
  {
    tmp_xsystemResult = "";
    char ch;
    while((ch=getc(fp))!=EOF) //&& ch!='\n'
    {
    	if(ch < 1 || ch > 255) continue; // non printable CHAR
    	tmp_xsystemResult += ch;
      
    }
  }

  pclose(fp);

  return tmp_xsystemResult;
  //return tmp;
}



void TeMIPAlarmObjectUser::recreateExtAlarm(){
          RWCString m_BTSName = extractBTSFromEXTERNAL_ALARM();
          RWCString m_managedObject = getManagedObject().getString();
          RWCString m_newMO = m_managedObject + " BTS " + m_BTSName;
          TeMIPNewAlarmObject m_newAlarm = TeMIPNewAlarmObject(*this);
          m_newAlarm.setManagedObject(m_newMO);
          m_newAlarm.SubmitAlarm();
}


// LAB


//ALI

RWCString TeMIPAlarmObjectUser::getBTSName(){
	
	RWCString managedObjectStr = getManagedObject().getStringNatural();
        RWCString BTS_Serch_Str = " BTS ";

        size_t endPos = managedObjectStr.length();
        size_t startPos = managedObjectStr.index(BTS_Serch_Str,0);

        if(startPos == RW_NPOS){
            Trace::getTrace().Error("Rule #26: Failed to extract BTS from managedObj. Alarm ID = " +getId().getString() +
                                    ", managedObject = " + managedObjectStr);
        return "";
        }
        Trace::getTrace().DEBUG("TeMIPAlarmObjectUser::getBTSName() Found " + BTS_Serch_Str);
        startPos += BTS_Serch_Str.length();
        RWCString tmpBTSName = managedObjectStr(startPos,endPos - startPos);
        BTSName = tmpBTSName.strip(RWCString::both);

    
    Trace::getTrace().DEBUG("TeMIPAlarmObjectUser::getBTSName() Found BTSName = " + BTSName);
    return BTSName;

}

RWCString TeMIPAlarmObjectUser::getCellName(){
        RWCString m_additionalTxt_Cell = getAdditionalText().getString();
        RWCString m_searc_str = RWCString("CELL       SCTYPE   CHTYPE   CHRATE   SPV");
        size_t m_startPos_Cell = m_additionalTxt_Cell.index(m_searc_str,0);
        RWCString Error_Cell = RWCString("Search String Not Found");

        if(m_startPos_Cell == RW_NPOS) {
                Trace::getTrace().Error("OSS: TeMIPAlarmObjectUser::getCellName, No first string in Add Text");
                return Error_Cell;
        }
        m_startPos_Cell += m_searc_str.length();
	m_startPos_Cell++;
	m_startPos_Cell++;

        RWCString m_CellName1 = m_additionalTxt_Cell(m_startPos_Cell,6);
        //RWCString m_CellName = m_CellName1.strip(RWCString::both);
	tmp_CELLName = m_CellName1.strip(RWCString::both);
        //return (const char*) m_CellName;
        Trace::getTrace().DEBUG("getCellName(): returning: " + tmp_CELLName);
        return tmp_CELLName;
}

RWCString TeMIPAlarmObjectUser::getTarih(){
        RWCString m_Tarih = (const char *) system("date");
//	RWCString m_Tarih1 = (RWCString) m_Tarih;
        return m_Tarih;
}


//ALI

//ERKAN

bool TeMIPAlarmObjectUser::setGPRSAlarm(){
        RWCString addtext = getAdditionalText().getString();
    size_t pos1 = addtext.index("TrunkId=");
    size_t pos2 = addtext.index(":",pos1);
    pos1+=8;
    RWCString tId = addtext(pos1, pos2 - pos1);
    RWCString TrunkId = tId;
    RWCString tIdFile =  tId + ".txt";
    RWCString path = RWCString("/var/opt/expertdata/circuitFiles/GPRS/");
    tIdFile = path + tIdFile;
    ifstream file(tIdFile);
    if (file){
                GPRSFile = setGPRSFile(tIdFile);
                return true;
                return (_isGPRSAO = true);
    }
                return false;
                return (_isGPRSAO = false);
}



RWCString TeMIPAlarmObjectUser::setGPRSFile(const char *trunkFile){
        GPRSFile  = RWCString(trunkFile);
        return GPRSFile;
}


RWCString TeMIPAlarmObjectUser::LoadMessage()  //2
{
    RWCString str;
    RWCString str1;
    RWCString str2;
    RWCString tIdFile = GPRSFile;
    char buf[100];
    ifstream file(tIdFile);
    char buf11[100];
     while (!file.eof())
    {
      file.getline(buf11,100);
      str2=buf11;
      if (str2 != "")
       {
         str += str2 + "\n" ;
       }
    }
   RWCString tId = GPRSFile;
    size_t pos1 = tId.first("S");
    size_t pos2 = tId.first(".");
    tId = tId(pos1+2,pos2-pos1-2);
    str1 =  tId + " TRUNK PROBLEM : " + "\n" + str  ;
return str1;
}

//ERKAN



void TeMIPAlarmObjectUser::createIMACopyAlarm(){

	TeMIPNewAlarmObject newAlarm = TeMIPNewAlarmObject(*this);
	newAlarm.setParentOC(IMANewOC);
	
	if(!getManagedObject().IsPresent()){
		Trace::getTrace().Error("TeMIPAlarmObjectUser::createIMACopyAlarm(), Managed Obj is not pressent !!!");
		return;
	}

	newAlarm.CreateAO(true);

}

void TeMIPAlarmObjectUser::submitClear(){
	TUnsigned32  notifId;
 	TEnumeration severity;
 	TUnsigned32 corrNotifId;
 	TSetOf  notifSet; // Attributes in Record
 	TRecord  corrNotifRec; // Record
 	TSetOf  correlatedNotifications;
	TeMIPNewAlarmObject newAlarm = TeMIPNewAlarmObject(*this);

	severity = S_Clear;
	newAlarm.setSeverity(&severity);
	
	if(!getNotificationIdentifier().IsPresent()){
		Trace::getTrace().Error("TeMIPAlarmObjectUser::submitClear(), notif ID is not pressent !!!");
 		return;
	}
	if(!getManagedObject().IsPresent()){
		Trace::getTrace().Error("TeMIPAlarmObjectUser::submitClear(), Managed Obj is not pressent !!!");
		return;
	}



	//-----------------------------
	// Set the corr  notif
   	//-----------------------------
	corrNotifId = getNotificationIdentifier().getLong();
	
  	notifSet.SetID(2);
  	notifSet.Add(corrNotifId);
  	corrNotifRec.Add(notifSet);
  	correlatedNotifications.SetID(6);
  	correlatedNotifications.Add(corrNotifRec);
  	newAlarm.setCorrelatedNotifications(&correlatedNotifications);

	newAlarm.SubmitAlarm(true);
}



void TeMIPAlarmObjectUser::create_MO_FAULT_ListAlarm(RWCString listMOName){
    TeMIPNewAlarmObject newAlarm = TeMIPNewAlarmObject(*this);
    RWCString txt = RWCString("SpecificProblem: LIST:RADIO X-CEIVER ADMINISTRATION\nMANAGED OBJECT FAULT\n");
    txt += MO_FAULT_Listalarm_header;
    const TLatin1String* newAddText = new TLatin1String(txt.data());
    newAlarm.setAdditionalText(newAddText);
    TUnsigned32 newNotId =  TUnsigned32(getNotificationIdentifier().getLong() + 10000000);
    
    newAlarm.setNotificationIdentifier( (const TUnsigned32*) &newNotId);
    if(!listMOName.isNull()) {
	newAlarm.setManagedObject(listMOName);//"GSM_OSS .Expert_GSM_OSS");
	Trace::getTrace().DEBUG("create_MO_FAULT_ListAlarm(...): Setting MO = " + listMOName);
    }else{
	Trace::getTrace().DEBUG("create_MO_FAULT_ListAlarm(...): listMOName was null");
    }
    
    newAlarm.SubmitAlarm();

}

void TeMIPAlarmObjectUser::add_to_MO_FAULT_ListAlarm(TeMIPAlarmObjectUser* alarmPtr){
   
  

    RWCString appendStr =  createListLine(alarmPtr);
    
    if(real_additionalText.length() + appendStr.length() > 65534){ //65535 is the max size of additonal text (ASN.1)
      Trace::getTrace().Error("Rule#6: Failed to add alarm to compact list, max size of AdditionalText is reached. ListAlarm ID = " + 
                              getId().getString());
      return;
    }
    real_additionalText += "\n"  + appendStr;
    safeSetAdditionalText( real_additionalText);
   
}
void TeMIPAlarmObjectUser::add_clearence_to_MO_FAULT_ListAlarm(TeMIPAlarmObjectUser* alarmPtr){
	TAbsTime clearingTime = TAbsTime(RWTime());
	add_clearence_to_MO_FAULT_ListAlarm(alarmPtr,&clearingTime);
}

void  TeMIPAlarmObjectUser::add_clearence_to_MO_FAULT_ListAlarm(TeMIPAlarmObjectUser* alarmPtr,const TAbsTime* clearTimePtr){
    Trace::getTrace().DEBUG("TeMIPAlarmObjectUser::add_clearence_to_MO_FAULT_ListAlarm -Enter");
    size_t Field_length_CTime =  12;
    RWCString ClearTime = clearTimePtr->GetValue().asString("%d/%m-%H:%M");

    ClearTime.resize(Field_length_CTime);
    Trace::getTrace().DEBUG("TeMIPAlarmObjectUser::add_clearence_to_MO_FAULT_ListAlarm ClearTime = " + ClearTime);

    RWCString str_id = alarmPtr->getId().getString();
    size_t startPos = real_additionalText.index( RWCRExpr("\n[ \t]*0*" +  str_id)); //serch for start of line 
    if(startPos == RW_NPOS){
      Trace::getTrace().Error("line with alarm ID = " + str_id + " Not Found in list alarm, ID = " + getId().getString());
      return;
    }
    //we have found the line to add the clearense time 

    size_t replace_start, replace_length;
    // serch for the last N/A until NEWL, starting at startPos
    replace_start =  real_additionalText.index(RWCRExpr("NOT_CLR[\t ]*"), &replace_length,startPos ); 
    if( replace_start == RW_NPOS || replace_length == 0){
      Trace::getTrace().DEBUG("NOT_CLR[\\t ]*\\n not found in additionaltext,Alarm ID = " +  str_id + 
                              ",list alarm ID = " + getId().getString());
      return;
    }
    Trace::getTrace().DEBUG("TeMIPAlarmObjectUser::add_clearence_to_MO_FAULT_ListAlarm Setting additionalText");
    real_additionalText.replace(replace_start,replace_length , ClearTime);
    
    safeSetAdditionalText( real_additionalText);
        
   
}

RWCString TeMIPAlarmObjectUser::createListLine(TeMIPAlarmObjectUser* alarm_ptr){
    
    size_t Field_length_ID = 9;
    size_t Field_length_MO = 14;
    size_t Field_length_Sev = 4;
    size_t Field_length_Slogan = 9; //The longest alarm slogan is CON_QUEUE_CONGESTION...
    size_t Field_length_EVTime = 12;
    size_t Field_length_CTime =  12;

    RWCString severity = alarm_ptr->getSeverity().getString();
    RWCString alarmSlogan = parser.getAlarm_slogan_MANAGED_OBJECT_FAULT(alarm_ptr); 
    RWCString eventTime =  alarm_ptr->getEventTime().getTAbsTime()->GetValue().asString("%d/%m-%H:%M");
    RWCString str_id = alarm_ptr->getId().getString();
    RWCString MO = parser.getMO_MANAGED_OBJECT_FAULT( alarm_ptr);
    RWCString ClearTime = "NOT_CLR";
   
    size_t paddingSize = Field_length_ID - str_id.length();
    if(paddingSize > 0) str_id.prepend('0',paddingSize );
    //str_id.resize(Field_length_ID);
    MO.resize(Field_length_MO);
    severity.resize(Field_length_Sev);
    alarmSlogan.resize(Field_length_Slogan);
    eventTime.resize(Field_length_EVTime);
    ClearTime.resize(Field_length_CTime);

    RWCString line = "" +  str_id + MO + severity +" " + alarmSlogan + " " +  eventTime + " " +ClearTime;
    return line;
}



RWCString TeMIPAlarmObjectUser::getSubContractorString(){
    
  Trace::getTrace().Enter("TeMIPAlarmObjectUser::getSubContractorString()");
    RWCString BSC_BTS_Arr[2];
    if(extractBSC_BTSName(getManagedObject().getStringNatural() ,BSC_BTS_Arr)){  
        Trace::getTrace().DEBUG("TeMIPAlarmObjectUser::getSubContractorString(), found BSC= \"" +
                                BSC_BTS_Arr[0] + "\"  BTS = \"" + BSC_BTS_Arr[1] + "\"");
        return SubContractor::getRef()->findInfoFor(BSC_BTS_Arr[0],BSC_BTS_Arr[1]);
    }
    return "PARSE-ERROR: BSC or BTS not found in managed Obj.";
}



bool TeMIPAlarmObjectUser::extractBSC_BTSName(const RWCString& managedObjectStr, RWCString result[]){
    RWCString BSC_Serch_Str = " BSC ";
    RWCString BTS_Serch_Str = " BTS ";
    size_t BTSstartPos,  BTSEndPos, BSCEndPos;

    size_t BSCstartPos = managedObjectStr.index(BSC_Serch_Str,0);
    if(BSCstartPos == RW_NPOS){ 
        Trace::getTrace().Error("Rule #7 Failed to extract BSC from managedObj. Alarm ID = " +getId().getString() + 
                              ", managedObject = " + managedObjectStr);
        return false;
    }
    BSCstartPos += BSC_Serch_Str.length();
    
    BTSstartPos = managedObjectStr.index(BTS_Serch_Str,BSCstartPos);
    if(BTSstartPos == RW_NPOS){ 
        Trace::getTrace().Error("Rule #7 Failed to extract BTS from managedObj. Alarm ID = " + getId().getString() + 
                              ", managedObject = " + managedObjectStr);
        return false;
    }
    BSCEndPos = BTSstartPos;
    BTSstartPos += BTS_Serch_Str.length();
    BTSEndPos = managedObjectStr.length();

    RWCString BSC = managedObjectStr(BSCstartPos,BSCEndPos - BSCstartPos);
    RWCString BTS = managedObjectStr(BTSstartPos,BTSEndPos - BTSstartPos);
    result[0] = BSC.strip(RWCString::both);
    result[1] = BTS.strip(RWCString::both);

    return true;
}

RWCString  TeMIPAlarmObjectUser::extractBTSFromEXTERNAL_ALARM(){
        RWCString additionalText = real_additionalText;
        RWCString endString = (const char *) RWCString(" END\n"); 
        size_t pos1 = additionalText.index(endString);
        RWCString str1= additionalText(pos1,1);
        while (str1 != "\n")
        {
         pos1=pos1 -1;
         str1= additionalText(pos1,1);
        }
        pos1-=1;
        size_t pos2=pos1;
        str1= additionalText(pos1,1);
        while (str1 != "\n")
        {
         pos1=pos1 -1;
         str1= additionalText(pos1,1);
        }
        pos1+=2;
      RWCString BTS_str = additionalText(pos1, pos2 - pos1 + 1);
      if (BTS_str == "") {
            BTS_str = "Not found";
        }else{
            Trace::getTrace().DEBUG("extractBTSFromEXTERNAL_ALARM() First RAW BTS = " + BTS_str);
            BTS_str = BTS_str.replace(RWCRExpr("[\n\t ]+END[\n\t ]*"),"",RWCString::all); //trim end
            Trace::getTrace().DEBUG("extractBTSFromEXTERNAL_ALARM() second RAW BTS = " + BTS_str);
            BTS_str = BTS_str.replace(RWCRExpr("[\n\t ]"), "" ,RWCString::all); //trim beginning
            Trace::getTrace().DEBUG("extractBTSFromEXTERNAL_ALARM() final  BTS_name = " + BTS_str);
        }
        BTSName = BTS_str;
    return BTSName;
}

RWCString TeMIPAlarmObjectUser::extractBSCFromEXTERNAL_ALARM(){
    if(BSCName == ""){
        RWCString managedObjectStr = getManagedObject().getStringNatural();
        RWCString BSC_Serch_Str = " BSC ";

        size_t endPos = managedObjectStr.length();
        size_t startPos = managedObjectStr.index(BSC_Serch_Str,0);

        if(startPos == RW_NPOS){ 
            Trace::getTrace().Error("Rule #8: Failed to extract BSC from managedObj. Alarm ID = " +getId().getString() + 
                                    ", managedObject = " + managedObjectStr);
        return "";
        }
        Trace::getTrace().DEBUG("TeMIPAlarmObjectUser::extractBSCFromEXTERNAL_ALARM() Found " + BSC_Serch_Str);
        startPos += BSC_Serch_Str.length();
        RWCString tmpBSCName = managedObjectStr(startPos,endPos - startPos);
        BSCName = tmpBSCName.strip(RWCString::both);
        
    }
    Trace::getTrace().DEBUG("TeMIPAlarmObjectUser::extractBSCFromEXTERNAL_ALARM() Found BSCName = " + BSCName);
    return BSCName;
}

RWCString TeMIPAlarmObjectUser::safeGetUserText(){
	return real_userText;
}

void TeMIPAlarmObjectUser::safeSetUserText(const RWCString& newText){
	real_userText = newText;
	SetUserText(real_userText);
	//CHNO
        Trace::getTrace().DEBUG("TeMIPAlarmObjectUser:safeSetUserText, AlarmID = "+ getId().getString() +" Setting UT = " + newText);
}

RWCString TeMIPAlarmObjectUser::safeGetAdditionalText(){
	return real_additionalText;
}

void TeMIPAlarmObjectUser::safeSetAdditionalText(const RWCString& newText){
	real_additionalText = newText;
	SetAdditionalText(real_additionalText);
}

void TeMIPAlarmObjectUser::safeReplaceAdditionalText(const RWCString& oldStr,const RWCString& newStr ){
	real_additionalText.replace(oldStr,newStr);
	SetAdditionalText(real_additionalText);
}


Parser TeMIPAlarmObjectUser::parser = Parser();
FileHandler TeMIPAlarmObjectUser::theFile=FileHandler();
FileHandler TeMIPAlarmObjectUser::theSGSNFile=FileHandler(SGSN_FILE_NAME);
//Tralil TeMIPAlarmObjectUser::theTralil=Tralil();
//FileHandler TeMIPAlarmObjectUser::theFile=FileHandler("confFile",";"); 
RWCString TeMIPAlarmObjectUser::MO_FAULT_Listalarm_header = RWCString(MO_FAULT_LIST_HEADER);

#endif














