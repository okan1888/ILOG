#ifndef _FILEHANDLER_HXX_
#define _FILEHANDLER_HXX_

#include "user_alarmobject.hxx"
#include "constants.hxx"
#include "Trace.hxx"
#include "Configuration.hxx"
#include <iostream.h>
#include <fstream.h>
#include <rw/bstream.h>
#include <rw/cstring.h>
#include <rw/tvvector.h>
#include <rw/tvmap.h>
#include <rw/ctoken.h>

class KeyVal {
public:
        KeyVal(const RWCString  &key, const RWCString  &val);
        RWCString  m_key;
        RWCString  m_val;
        KeyVal(); 
};

class FileHandler {
public:
    FileHandler();
    RWCString FindENDString(RWCString &addtext2, RWCString &opsiyon);
    RWCString FindSPIDString(RWCString &addtext3, RWCString &aaa);
    RWCString FindSPIDForOVO(RWCString &addtext3, RWCString &aaa); 
    RWCString LoadDxxTrunkid(RWCString &addtext);
    RWCString LoadPortLabelFromFile(RWCString nElementName, RWCString cardNo, RWCString &fileName);

    static RWCString SGSN_FILE_NAME;
    FileHandler(const RWCString &fileName);
    RWTValMap<RWCString, RWCString, less<RWCString> > SGSNMap;

    static FileHandler *getRefSGSN(const RWCString &fileName);
    static FileHandler *refSGSN;
    RWCString LoadFromFile(RWCString &addtext, int &filenum, RWCString &testtext, RWCString &fileName);
    bool loadNodeNameintoMap();
    RWCString LoadDXXIdNamefromMap(RWCString &text1, bool LoadMap);
    static FileHandler getConf();
    static FileHandler* getConfPtr();
    bool operator !() const
    {
        return true;
    }
       ~FileHandler();
public:
    KeyVal *m_fileVect[2]; 
    bool m_success;
    static RWTValMap<RWCString, RWCString, less<RWCString> > NodeNHash;
    static FileHandler* ref2; 
};


#endif
