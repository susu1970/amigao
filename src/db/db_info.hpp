// Copyright 2020 amigao. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Author: 758293230@qq.com
//

#ifndef AMIGAO_DB_DB_INFO_H_
#define AMIGAO_DB_DB_INFO_H_

#include<sstream>

#include<utils/ini_parser.hpp>

namespace amigao{
  //contains database connection information
  class DBInfo{
  protected:
    //database connection arguments
    int MAX_CONN=100,MAX_IDLE=30,MIN_IDLE=3,SLEEP=3;
    std::string HOST="",USER="",PASSWD="",DB_NAME="",UNIX_SOCKET="";
    unsigned long CLIENT_FLAG=0;
    unsigned int PORT=3306;
    //end database connection arguments
    void set_db_conf_path(std::string db_conf_path_){
      using namespace std;
      IniParser iniParser(db_conf_path_);
      stringstream ss;
      auto result=iniParser.get_result();
      if(result.find("MAX_CONN")!=result.end()){
	ss<<result["MAX_CONN"];
	ss>>MAX_CONN;
	ss.clear();
      }
      if(result.find("MAX_IDLE")!=result.end()){
	ss<<result["MAX_IDLE"];
	ss>>MAX_IDLE;
	ss.clear();
      }
      if(result.find("MIN_IDLE")!=result.end()){
	ss<<result["MIN_IDLE"];
	ss>>MIN_IDLE;
	ss.clear();
      }
      if(result.find("SLEEP")!=result.end()){
	ss<<result["SLEEP"];
	ss>>SLEEP;
	ss.clear();
      }
      if(result.find("HOST")!=result.end())
	HOST=result["HOST"];
      if(result.find("USER")!=result.end())
	USER=result["USER"];
      if(result.find("PASSWD")!=result.end())
	PASSWD=result["PASSWD"];
      if(result.find("DB_NAME")!=result.end())
	DB_NAME=result["DB_NAME"];
      if(result.find("UNIX_SOCKET")!=result.end())
	UNIX_SOCKET=result["UNIX_SOCKET"];
      if(result.find("CLIENT_FLAG")!=result.end()){
	ss<<result["CLIENT_FLAG"];
	ss>>CLIENT_FLAG;
	ss.clear();
      }
      if(result.find("PORT")!=result.end()){
	ss<<result["PORT"];
	ss>>PORT;
	ss.clear();
      }
      if(MIN_IDLE<1)
	MIN_IDLE=1;
      if(MAX_IDLE<MIN_IDLE)
	MAX_IDLE=MIN_IDLE+1;
      if(MAX_CONN<1)
	MAX_CONN=1;
      if(SLEEP<0)
	SLEEP=3;
    }
  public:
    DBInfo(int MAX_CONN_,int MAX_IDLE_,int MIN_IDLE_,int SLEEP_,
	   std::string HOST_,std::string USER_,
	   std::string PASSWD_,std::string DB_NAME_,
	   std::string UNIX_SOCKET_,
	   unsigned long CLIENT_FLAG_,
	   unsigned int PORT_)
      :MAX_CONN(MAX_CONN_),MAX_IDLE(MAX_IDLE_),MIN_IDLE(MIN_IDLE_),
       SLEEP(SLEEP_),HOST(HOST_),USER(USER_),PASSWD(PASSWD_),
       DB_NAME(DB_NAME_),UNIX_SOCKET(UNIX_SOCKET_),
       CLIENT_FLAG(CLIENT_FLAG_),PORT(PORT_){
    }
    DBInfo(std::string db_conf_path_){
      set_db_conf_path(db_conf_path_);
    }
  };
}

#endif
