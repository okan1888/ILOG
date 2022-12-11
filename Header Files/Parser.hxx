

#ifndef LAB_Parser_hxx
#define LAB_Parser_hxx

#include <rw/re.h>
#include<rw/tvslist.h>
#include <rw/tvmap.h>
class Configuration;


class TeMIPAlarmObjectUser;

class Parser{


public:
    LAB_Specific_Problems getSP(TeMIPAlarmObjectUser* alarm_ptr );
    External_Alarm_Slogans get_alarm_slogan(TeMIPAlarmObjectUser* alarm_ptr );
    RWCString getDIP(TeMIPAlarmObjectUser* alarm_ptr );
    RWCString getMSCName(TeMIPAlarmObjectUser* alarm_ptr );
    void configurationUpdated(Configuration* newCfg);
    bool isLoaded;
    Parser();
    static Parser common;
    RWCString getMO_MANAGED_OBJECT_FAULT(TeMIPAlarmObjectUser* alarm_ptr);
    RWCString getAlarm_slogan_MANAGED_OBJECT_FAULT(TeMIPAlarmObjectUser* alarm_ptr);
    RWCString getExternalAlarmSloganStr(TeMIPAlarmObjectUser* alarm_ptr);
    RWCString getIMARegion(TeMIPAlarmObjectUser* alarm_ptr);

private:
    RWTValMap<RWCString, int, less<RWCString> > SPMap;
    RWTValMap<RWCString, RWCString, less<RWCString> > IMARegionMap;

    Configuration* loadedConfig;
    RWCRExpr SpecificProblem_regexp;

    RWCRExpr EXTERNAL_ALARM_SLOGAN_MAINS_F_RegExp;
    RWCRExpr EXTERNAL_ALARM_SLOGAN_AC_FAILURE_RegExp;

    RWCRExpr RADIO_X_CEIVER_ADMINISTRATION_MANAGED_OBJECT_FAULT_SLOGAN_MAINS_F_RegExp;
    RWCRExpr RADIO_X_CEIVER_ADMINISTRATION_BTS_EXTERNAL_FAULT_SLOGAN_AC_F_regExp;

    RWCRExpr DIP_HEADER_RegExp;
    RWCRExpr DIP_RegExp;
    RWCRExpr MSCName_RegExp;
    RWCRExpr MO_MO_Fault_RegExp;            //[-a-zA-Z0-9][ ]
    RWCRExpr alarmSlogan_MO_Fault_RegExp;   //((ABIS PATH UNAVAIL)|(BTS EXTERNAL)| ...|(TS SYNC FAULT)) 
    RWCRExpr MO_Fault_Header_RegExp;               //MO[ \tA-Z]*RSITE[ ]+ALARM SLOGAN[ ]\n

    RWCRExpr EXTERNAL_ALARM_SLOGAN_SYNTAX_RegExp;
    RWCString EXTERNAL_ALARM_SLOGAN_FILE;
    RWCString EXTERNAL_ALARM_SLOGAN_FORMAT_ERR_STR;

    void reportConfigErrors(RWTValSlist<RWCString> errorList);
    bool createExtAlarmSloganSyntax(); 
    void loadFromFile();
   


}

#endif


