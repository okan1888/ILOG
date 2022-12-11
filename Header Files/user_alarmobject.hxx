#ifndef _EXP_user_alarmobject_hxx
#define _EXP_user_alarmobject_hxx
 
#include "constants.hxx"
#include "Parser.hxx"
#include "Trace.hxx"
#include <rw/rwtime.h>
#include "subContractor.hxx"
#include <rw/re.h>
#include "DIPHandler.hxx"
#include "filehandler.hxx"

#include <rw/tvslist.h>
;

struct DIPInfo;

class TeMIPAlarmObjectUser : public TeMIPAlarmObject

{

private:
    LAB_Specific_Problems TC_SpecificProblem;
    External_Alarm_Slogans ext_alarm_slogan;
    
    //  For correlations:
    unsigned long primaryAlarmId; 
    //bool hasSecondaryAlarms;
    RWTPtrDlist<TeMIPAlarmObjectUser>  secondaryAlarms;

    //Transient handling:
    bool isHidden;
    bool isTransient;
    //LAB
    int _ruleState;

   
    RWCString DIP;
    RWCString MSC;
    RWCString BSC;
    
    RWCString other_DIP;
    RWCString other_MSC;
    RWCString other_link;

    RWCString IMANewOC;

    void extractDIPInfo();
    void extractOtherEndDIPInfo();
    
    RWCString siteName;
    RWCString BTSName;
    RWCString BSCName;
    
    RWCString _HIDE_STRING;
    RWCString real_additionalText;
    RWCString real_userText;

    static RWCString LIST_FILE_NAME;
    static RWCString SGSN_FILE_NAME;

    static RWCString MO_FAULT_Listalarm_header;
    RWCString createListLine(TeMIPAlarmObjectUser* alarmPtr);
    bool extractBSC_BTSName(const RWCString& managedObjectStr, RWCString result[]);
    RWCString tmp_alarmTypeCheckText;
    RWCString tmp_R8_extractAlarmType;
    RWCString tmp_BSCName;
    RWCString tmp_CELLName;
    RWCString tmp_xsystemResult;
    RWCString tmp_NodeName;


    //------------------------
    //    IMA stuff
    //------------------------
    bool _isIMARegionAO;
    bool _isIMAAO;
  
    
    bool setIsIMAAlarm();
    void parseIMAAlarm();
	
    //----------------------
    //    Common
    //----------------------
    bool _isAdminAO;
    bool setIsAdminAO();

    EXPAbsTime *m_clearanceTimeStamp;

    void setClearanceTimeStampFromTeMIPCall();

public:
    //--------------------------
    //  public IMA stuff
    //-------------------------
    bool isIMARegionAO(){return _isIMARegionAO;}	
    bool isIMAAO(){return _isIMAAO;}    
    void createIMACopyAlarm();
    
    //-------------------------
    bool isAdminAO(){return _isAdminAO;}
    void submitClear();
    
    const EXPAbsTime& getClearanceTimeStamp() const;

   // overridden to set Clearance Time Stamp on the fly when
   // the alarm is cleared while Expert is running
    void OnPostUpdateCleared(); 


    bool handled_by_IMA_sync;
    bool isHandledByRule2;
    bool isHandledByRule7;
    bool isHandledByRule8;
    bool isHandledByRule9;
    bool isHandledByRule10;
    bool isHandledByRule11;
    bool isHandledByRule17;
    bool isHandledByRule19;
    bool isHandledByRule26;
    bool isHandledByRule27;
    bool isHandledByRule33;
    bool isHandledByRule38;
    bool isHandledByRule39;
    bool isHandledByRule40;
    bool isHandledByRule41;
    bool isHandledByRule22;
    bool isHandledByRule2OVO;

    void safeSetUserText(const RWCString& newText);//RWCString newText);
    RWCString safeGetUserText();
    
    RWCString safeGetAdditionalText();
    void safeSetAdditionalText(const RWCString& newText);
    void safeReplaceAdditionalText(const RWCString& oldStr,const RWCString& newStr );

    RWCString extractBTSFromEXTERNAL_ALARM();
    RWCString extractBSCFromEXTERNAL_ALARM();
    RWCString getSubContractorString();
    bool creatingListAlarm;
    long myListAlarmID;
    //bool isAlarmIDInTOC(long id);
    const RWCString getHideString();
    RWCString getDIPName(){return DIP;}
    RWCString getMSCName(){return MSC;}
    RWCString getBSCName(){return BSC;}

    RWCString getMSC() {return MSC;}
    RWCString getDIP() {return DIP;}
    
    RWCString getOtherEndDIPName(){return other_DIP;}
    RWCString getOtherEndMSCName(){return other_MSC;}
    RWCString getOtherEndLink(){return other_link;}

    RWCString getSiteName(){return siteName;}

   
    RWCString xsystem(char* cmd);
    RWCString cmdexec(const char *str);
    RWCString getBSC();
    RWCString getNodeName();
    RWCString getSGSNNode();
    void reloadSGSN();
    RWCString getCustomAddText(const char *pStartStr, const char *pEndStr);
    void recreateExtAlarm();

    bool findString(const char *str);
    bool findManObject(const char *str1);
    void gotVectValue(int fileNum, const char *str2);
    RWCString gotOVOStr();
    RWCString extractAlarmType(const char* filename1, const char* filename2);
    RWCString extractAlarmTypeCheckText(const char* filename1, const char* filename2);
    void gotDXXNodeIdName(); 
    RWCString gotDXXAddIdent();
    RWCString getPortLabel(const char *filenam);
    void gotDXXCircuitName();

    RWCString getCellName();
    RWCString getBTSName();

    bool _isGPRSAO;
    RWCString TrunkId;
    bool setGPRSAlarm();
    RWCString  setGPRSFile(const char *);
    RWCString LoadMessage();
    RWCString GPRSFile;


    void setHideString();
    void removeHideString();
   
    bool operator == (const TeMIPAlarmObjectUser& rhs)const;
    bool isOld;
    bool isAddedToList;
    int transientWaitingTime;
    int waitingTime2;

    void create_MO_FAULT_ListAlarm(RWCString listMOName);
    void add_to_MO_FAULT_ListAlarm(TeMIPAlarmObjectUser* alarmPtr);
    void add_clearence_to_MO_FAULT_ListAlarm(TeMIPAlarmObjectUser* alarmPtr, const TAbsTime* clearTimePtr);
    void add_clearence_to_MO_FAULT_ListAlarm(TeMIPAlarmObjectUser* alarmPtr);
    
    int getTC_SpecificProblemInt();
    int getExternal_Alarm_SloganInt();
    RWCString getExternalAlarmSloganStr();
    bool getHasSecondary();

    unsigned long getPrimaryAlarmId();
    void setPrimaryAlarmId(unsigned long id);
    
    bool getIsHidden();
    void setIsHidden(bool hide);
    
    int getRuleState();
    void setRuleState(int newState);
    
    static Parser parser;
    static FileHandler theFile;
    static FileHandler theSGSNFile;

    void OnPostBuild();
    void parseOSSAlarm();
    void parseEXTERNAL_ALARM();
    void parseBTS_EXTERNAL();

    TeMIPAlarmObjectUser(EXPPopulationRequest * pr);   
    virtual ~TeMIPAlarmObjectUser();
    
    void addSecondaryAlarm(TeMIPAlarmObjectUser* secAlarm);
    void appendSecondaryAlarmToAdditionalText(TeMIPAlarmObjectUser* secAlarm);
    bool removeSecondayAlarm(TeMIPAlarmObjectUser* secAlarm);
    TeMIPAlarmObjectUser* getSecondaryAlarmAt(int index);
    int getSecondaryAlarmCount();
    bool removeFromAlarmText(TeMIPAlarmObjectUser* secAlarm);
    DECLARE_ILR_CLASS("user_model.TeMIPAlarmObjectUser")
};


#endif 








