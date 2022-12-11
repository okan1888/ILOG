#ifndef _FILEHANDLER_cxx
#define _FILEHANDLER_cxx

#include "filehandler.hxx"

FileHandler* FileHandler::ref2 = 0;
RWTValMap<RWCString, RWCString, less<RWCString> > FileHandler::NodeNHash;


KeyVal::KeyVal()
{}

KeyVal::KeyVal(const RWCString  &key, const RWCString  &val)   
{
	m_key = key;
        m_val = val;
}	

FileHandler::FileHandler()
{
	m_success=true;
}

FileHandler::~FileHandler()
{ }

RWCString FileHandler::LoadFromFile(RWCString &addtext, int &filenum, RWCString &testtext, RWCString &fileName)
{ if ( testtext == "Rule11")
  {
    RWCString str, str1, key, val, cccc,aaaa,sonuc1;
    RWTValVector<RWCString> fileNames(10);
    char buf[100];
    int m;
    KeyVal *deger=new KeyVal();
    ifstream file(fileName);
    if (!file){
       return addtext;
        }
    int i=0;
        while (!file.eof()) {
        file.getline(buf,100);
        str1=buf;
        if (str1 != "") {
        fileNames[i]=str1;
        i++;
        }
        }
        file.close();
        KeyVal pKeyValVect[100];
        RWCString soon=fileNames[filenum];
        ifstream file1(soon);
        if (!file1) {
           return addtext;
        }
        char buf11[100];
        int k = 0;
         while (!file1.eof()) {
        file1.getline(buf11,100);
        RWCString str=buf11;
        size_t pos = str.first(";;");
     if (str != "") {
     if (pos == RW_NPOS) {
                return addtext;
            }
          key = str(0, pos);
          size_t nokta = str.mbLength();
          val = str((pos+2),nokta);
          pKeyValVect[k]=KeyVal(key,val);
           k++;
           }
     else {
       m_fileVect[0]=pKeyValVect;
         }
       }
     deger=*(m_fileVect+0);
     m=0;
     while (m < 20)
     {
      cccc= (*(deger+0)).m_key;
      aaaa= (*(deger+0)).m_val;
      if (addtext.contains(cccc))
                {
                        addtext.replace(cccc,aaaa);
                }
      m=m+1;
      deger=deger+1;
     }
  }
return addtext;
}


RWCString FileHandler::FindENDString(RWCString &text1, RWCString &opsiyon)
{
   RWCString str2, aaa, str3, strbts;
   aaa=(const char *) RWCString(" END\n");
 if ( opsiyon == "Rule11")
 { 
   size_t pos1 = text1.index(aaa);
   if (pos1 == RW_NPOS)
          {
                str2 = RWCString("No value found");
                return str2;
          }
   pos1=pos1 - 2;
   str2=text1(pos1,1);
   while ( str2 != "\n")
   {
     pos1 = pos1 - 1;
     str2=text1(pos1,1);
   }
   str3=text1((pos1+2),10);
   return str3;
 }
 if ( opsiyon == "Rule15")
 {
   size_t pos1 = text1.index(aaa);
   if (pos1 == RW_NPOS)
          {
                str2 = RWCString("No value found");
                return str2;
          }
   pos1=pos1 - 2;
   size_t pos3 = pos1;
   str2=text1(pos1,1);
   while ( str2 != "\n")
   {
     pos1 = pos1 - 1;
     str2=text1(pos1,1);
   }
   str3=text1((pos1+1),(pos3-pos1));
   pos1=pos1 - 2;
   size_t pos2 = pos1;
   str2=text1(pos1,1);
   while ( str2 != "\n")
   {
     pos1 = pos1 - 1;
     str2=text1(pos1,1);
   }
   RWCString str4=text1((pos1+1),(pos2-pos1));
   RWCString ok=RWCString("->");
   str4+=ok;
   str4+=str3; 
   return str4;
 }
 if ( opsiyon == "Rule8")
 {
   RWCString str3, str4, bbb, strbts, stremg, stralarmt;

   bbb=(const char *) RWCString(" EXTERNAL ALARM\n");
   size_t posext1 = text1.index(bbb,20);
   posext1 = posext1 + bbb.length()+1;
   size_t pos1 = posext1;
   str2=text1(pos1,1); 
   str3=text1(pos1+1,1);
   while ( !(str3 == "\n" || (str2 == " " && str3 == " ") ) )
   {
      pos1 = pos1  + 1 ;
      str2=text1(pos1,1);
      str3=text1(pos1+1,1);
   }
   stremg = text1(posext1, pos1 - posext1 + 1);
   pos1=pos1+1;
   str2=text1(pos1,1);
   while ( str2 == " " )
   {
      pos1 = pos1  + 1 ;
      str2=text1(pos1,1);
   }
   if (str2 == "\n")
    {
     return stremg;
    }
   size_t posbas=pos1;
   str2=text1(pos1,1);
   str4=text1((pos1+1),1);
   while ( !((str2 == " ") && ( (str4 == "0") || (str4 == "1") || (str4 == "2") || (str4 == "3") || (str4 == "4") || (str4 == "5") || (str4 == "6") || (str4 == "7") || (str4 == "8") || (str4 == "9") ) ) && (str4 != "\n") )
   {
      pos1 = pos1  + 1 ;
      str2=text1(pos1,1);
      str4=text1((pos1+1),1);
   }
   stralarmt=text1( posbas, pos1 - posbas );
   RWCString virgul=RWCString(", ");
   stremg+=virgul;
   stremg+=stralarmt; 
   return stremg;
 }       
if ( opsiyon == "Rule10" )
 {
   RWCString bbb=(const char *) RWCString(" EXTERNAL ALARM\n");
   RWCString ccc=(const char *) RWCString(" END\n");
   size_t pos1 = text1.index(bbb);
   size_t pos2 = text1.index(ccc);
   if ((pos1 == RW_NPOS) || (pos2 == RW_NPOS))
          {
                str2 = RWCString("No value found");
                return str2;
          }
   str3 = text1(pos1+16,pos2-pos1-12);
   return str3;
 }
 if ( opsiyon == "Rule8Part2" )
 {
   RWCString bbb=(const char *) RWCString(" EXTERNAL ALARM\n");
   size_t pos1 = text1.index(bbb);
   size_t pos2 = text1.index(aaa);
   if ((pos1 == RW_NPOS) || (pos2 == RW_NPOS))
          {
                str2 = RWCString("No value found");
                return str2;
          }
   str3 = text1(pos1+16,pos2-pos1-16);
   return str3;
 }
return text1;
}

RWCString FileHandler::FindSPIDString(RWCString &addtext3, RWCString &aaa)
{
   RWCString str4, aaa2, str5, str6, str7;
   aaa2=(const char *) aaa;
   size_t pos3 = addtext3.index(aaa2);
   if (pos3 == RW_NPOS)
          {
                str4 = RWCString("No value found!!");
                return str4;
          }
pos3 = pos3 + 1;
str4=addtext3(pos3,1);
while ( str4 != "\n")
   {
   pos3 = pos3 + 1;
   str4=addtext3(pos3,1);
   }
   pos3+=1;// ekled�k
str5=addtext3((pos3+1),7);
RWCString ok=RWCString("->");
str5+=ok;
pos3 = pos3 + 8;
str4=addtext3(pos3,1);
while ( str4 == " ")
   {
   pos3 = pos3 + 1;
   str4=addtext3(pos3,1);
   }
size_t pos4 = pos3;
pos3 = pos3 + 1;
str4=addtext3(pos3,1);
while ( !(str4 == " " || str4 == "\n") )
   {
   pos3 = pos3 + 1;
   str4=addtext3(pos3,1);
   }
str6=addtext3(pos4,(pos3-pos4));
str5+=str6;
return str5;
}

RWCString FileHandler::FindSPIDForOVO(RWCString &addtext3, RWCString &aaa)
{
   RWCString str4, aaa2, str5, str6, str7;
   aaa2=(const char *) aaa;
   size_t pos3 = addtext3.index(aaa2);
   if (pos3 == RW_NPOS)
          {
                str4 = RWCString("No value found!!");
                return str4;
          }
pos3 = pos3 + 1;
str4=addtext3(pos3,1);
while ( str4 != "\n")
   {
   pos3 = pos3 + 1;
   str4=addtext3(pos3,1);
   }
   pos3+=1;//
str5=addtext3((pos3+1),7);
RWCString ok=RWCString("->");
str5+=ok;
pos3 = pos3 + 8;
str4=addtext3(pos3,1);
while ( str4 == " ")
   {
   pos3 = pos3 + 1;
   str4=addtext3(pos3,1);
   }
size_t pos4 = pos3;
pos3 = pos3 + 1;
str4=addtext3(pos3,1);
while ( !(str4 == " " || str4 == "\n") )
   {
   pos3 = pos3 + 1;
   str4=addtext3(pos3,1);
   }
str6=addtext3(pos4,(pos3-pos4));
str5+=str6;
pos3 = pos3 + 1;
str4=addtext3(pos3,1);
while ( str4 == " ")
   {
   pos3 = pos3 + 1;
   str4=addtext3(pos3,1);
   }
pos4 = pos3;
pos3 = pos3 + 1;
str4=addtext3(pos3,1);
while ( !(str4 == " " || str4 == "\n") )
   {
   pos3 = pos3 + 1;
   str4=addtext3(pos3,1);
   }
str6=addtext3(pos4,(pos3-pos4));
str5=str5+ " " + str6;
return str5;
}


FileHandler *FileHandler::refSGSN = 0;


FileHandler::FileHandler(const RWCString &fileName){loadSGSNInfo(fileName);}
//FileHandler::FileHandler(const RWCString &fileName){reloadConfig();}

FileHandler *FileHandler::getRefSGSN(const RWCString &fileName){
    if(refSGSN == 0) refSGSN = new FileHandler(fileName);
    return refSGSN;
}

void FileHandler::refreshSGSN(const RWCString &fileName){
//void FileHandler::refreshSGSN(){
    FileHandler *oldRef = refSGSN;
   // refSGSN =  new FileHandler(SGSN_FILE_NAME);
    refSGSN =  new FileHandler(fileName);
    delete oldRef;
}


bool FileHandler::loadSGSNInfo(const RWCString &m_fileName){
/

        //ifstream SGSNFile(SGSN_FILE_NAME);
        ifstream SGSNFile(m_fileName);
        if(!SGSNFile){
                //const RWCString& m_errorTxt = RWCString("Failed to open SGSN File: ");
                const RWCString& m_errorTxt = RWCString("SGSN2");
                return m_errorTxt;
        }

        while(!SGSNFile.eof()){
                char  buff[32];
                RWCString m_line, m_keyStr, m_valueStr;

                SGSNFile.getline(buff,32);
                m_line = RWCString(buff);

                m_keyStr = m_line(0,7);
                m_valueStr = m_line(8,5);

                SGSNMap.insert(m_keyStr,m_valueStr);

        }
        SGSNFile.close();
        return true;
}

RWCString FileHandler::getSGSNNodeName(const RWCString &r_keyStr)
{
        RWCString m_result = "SGSN2";
        RWCString m_key = r_keyStr;

        if(SGSNMap.findValue(m_key,m_result))
                return (const char*)  m_result;
        else
        //      loadSGSNInfo();
//      SGSNMap.findValue(m_key,m_result);
        return (const char*)  m_result;
}



RWCString FileHandler::LoadDXXIdNamefromMap(RWCString &text1, bool LoadMap)
{
    RWCString result;
    if(NodeNHash.findValue(text1, result)){
            Trace::getTrace().DEBUG("FileHandler::LoadNodeNamefromMap NodeId was found in MAP");
            return result;
        }
    else {
     if (LoadMap)
      {
       if(loadNodeNameintoMap()){
        return LoadDXXIdNamefromMap(text1, false);
       }
      }
    }         
    RWCString error = RWCString("No nodename found for Nodeid: ") + text1; 
return error;
}


//Rule#19 Last Part
RWCString FileHandler::LoadDxxTrunkid(RWCString &addtext)
{
    RWCString str;
// Extracting trunkid information
    size_t pos1 = addtext.index("TrunkId="); 
    size_t pos2 = addtext.index(":",pos1);
    pos1+=8;
    RWCString tId = addtext(pos1, pos2 - pos1);
    RWCString tIdFile =  tId + ".txt";
// Opening file
    RWCString path = RWCString("/var/opt/expertdata/circuitFiles/");
    tIdFile = path + tIdFile;
    char buf[100];
    ifstream file(tIdFile);
    if (!file){
       return addtext;
    }
    char buf11[100];
    addtext.append("\n");
// Getting circuit names
    while (!file.eof())
    {
      file.getline(buf11,100);
      str=buf11;
      str=str.replace(RWCRExpr("    "), "" ,RWCString::all);
      if (str != "")
       {
         addtext.append(str);
         addtext.append(";");
       }
    }
addtext=addtext.replace(RWCRExpr(" "), "" ,RWCString::all);
return addtext;
}

//Rule#35
RWCString FileHandler::LoadPortLabelFromFile(RWCString nElementName, RWCString cardNo, RWCString &fileName)
{
    RWCString str, noValue, key1, key2, val;
    noValue=RWCString("No Value Found!");
    char buf[100];
    ifstream file(fileName);
    if (!file){
       return noValue;
        }
    char buf11[100];
    while (!file.eof())
    {
     file.getline(buf11,100);
     str=buf11;
     if (str != "")
     {
          size_t pos = str.first(",");
          if (pos == RW_NPOS)
          {
                return noValue;
          }
          key1 = str(0, pos);
          if (nElementName == key1)
          {
             size_t pos2 = str.index(",",pos+1); 
             if (pos2 == RW_NPOS)
             {
                 return noValue;
             }
             key2 = str((pos+1),(pos2-pos-1));
             if (cardNo == key2)
             {
             size_t nokta = str.mbLength();
             val = str((pos2+1),(nokta-pos2-1));
             return val;
             }
          }

      }
     }
return noValue;
}


bool FileHandler::loadNodeNameintoMap(){
    RWCString fileName1 = RWCString("/var/opt/expertdata/nodelistout11");
    RWCString str2;
    ifstream NodeNameFile(fileName1);
    if(!NodeNameFile){
        const RWCString& errorTxt = RWCString("Failed to open NodeNameFile: ");
        Trace::getTrace().Error(errorTxt + fileName1 );
    }
    while(!NodeNameFile.eof()){
         char  buff[512];
        NodeNameFile.getline(buff,512);
        str2= buff;
    if(str2 != ""){  
        size_t pos1 = str2.index(";;");
        RWCString key1 = str2(0,pos1-1);
        size_t lengt = str2.length();
        RWCString val1 = str2(pos1+3,lengt-pos1-3);
        if (!NodeNHash.insert(key1,val1))
             return false;
    }
    }
    //All data in file is loaded into the NodeNMap
return true;
}



FileHandler FileHandler::getConf(){

    return *FileHandler::getConfPtr();
}

FileHandler* FileHandler::getConfPtr(){
    if(ref2 == 0){
        ref2 = new FileHandler();
    }
    return ref2;
}

#endif
