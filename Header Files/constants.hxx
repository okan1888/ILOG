#ifndef constats_hxx
#define constats_hxx

#include <rw/re.h>
#include <exp_tal_types.hxx>



#define CFG_Entry_SPECIFIC_PROBLEM_regExp                      "REGEXP.SPECIFIC_PROBLEM_REGEXP"
#define CFG_Entry_FILE_PROCESS_UTILITY_TransientTime           "INT.FILE_PROCESS_UTILITY_TRANSIENT_TIME"
#define CFG_Entry_BLOCKING_SUPERVISION_TransientTime           "INT.BLOCKING_SUPERVISION_TRANSIENT_TIME"
#define CFG_Entry_CCITT7_TransientTime                         "INT.CCITT7_TRANSIENT_TIME"
#define CFG_Entry_CELL_LOGICAL_CH_AVAILABLE_TransientTime      "INT.CELL_LOGICAL_CH_AVAILABILITY_SUPERVISION_TRANSIENT_TIME"

#define CFG_Entry_EXTERNAL_ALARM_TransientTime                "INT.EXTERNAL_ALARM_TRANSIENT_TIME"
#define CFG_Entry_EXTERNAL_ALARM_cleranceDelayTime            "INT.EXTERNAL_ALARM_CLEARENCE_DELAY_TIME"
    
#define CFG_Entry_RADIO_X_BTS_EXTERNAL_TransientTime          "INT.RADIO_X_BTS_EXTERNAL_TRANSIENT_TIME"
#define CFG_Entry_RADIO_X_BTS_EXTERNAL_cleranceDelayTime      "INT.RADIO_X_BTS_EXTERNAL_CLEARENCE_DELAY_TIME"
#define CFG_Entry_RADIO_X_MO_FAULT_TransientTime              "INT.RADIO_X_MO_FAULT_TRANSIENT_TIME"
#define CFG_Entry_DIP_FAULT_TransientTime                     "INT.DIP_FAULT_TRANSIENT_TIME"


#define MO_FAULT_LIST_HEADER                                    "[Id_______MO____________Sev_Alarminfo_Ev-Time______C-Time__]"  

#define ADMIN_OC_NAME						"OC_ADMIN"
#define IMA_RAW_OC_NAME						"OC_IMA"

// Alarm severity
#define	S_Indeterminate					0  // Severity Indeterminate
#define S_Critical					1  // Severity Critical
#define S_Major						2  // Severity Major
#define S_Minor						3  // Severity Minor
#define S_Warning					4  // Severity Warning
#define S_Clear						5  // Severity Clear
#define S_Undefined					10 // Severity Undefined

;
enum LAB_Specific_Problems{
    FILE_PROCESS_UTILITY_AUTOMATIC_TRANSFER_FAILURE = 0,
    EXTERNAL_ALARM, 
    RADIO_X_CEIVER_ADMINISTRATION_MANAGED_OBJECT_FAULT,
    CELL_LOGICAL_CHANNEL_AVAILABILITY_SUPERVISION,
    RADIO_X_CEIVER_ADMINISTRATION_BTS_EXTERNAL_FAULT,
    DIGITAL_PATH_FAULT_SUPERVISION,
    MO_FAULT_LIST_ALARM,
    SP_UNDEFINED
};
 

enum External_Alarm_Slogans{
     MAINS_FAILURE,
     AC_FAILURE,
     ALARM_SLOGAN_UNDEFINED
};

enum RuleStates{
    UNSPECIFIED_RULE_STATE,
    SEND_TO_TRANSIENT_FILTER,
    TRANSIENT_WAITING,
    TRANSIENT,
    NOT_TRANSIENT,
    CORRELATING,
    CORRELATED
};

class Constants{

public:
   
    static void loadFromFile();
    Constants();
    static RWTime startTime; //

};

#endif








