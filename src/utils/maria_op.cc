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
// Author: susu1970@yandex.com 
//

#include<mariadb/mysql.h>
#include<string>
#include<syslog.h>
#include<iostream>

using namespace std;

  
int maria_real_query(MYSQL* mysql,string query){
  int ret;
  if(ret=mysql_real_query(mysql,query.c_str(),query.size())){
    cerr<<"---------<maria_real_query error>-------\n"<<query<<"\n--------</maria_real_query error>---------"<<endl;
    syslog(LOG_ERR,"---------<maria_real_query error>-------\n%s\n--------</maria_real_query error>---------\n",query.c_str());
  }
  return ret;
}
MYSQL*maria_real_connect(MYSQL*mysql,string HOST,string USER,string PASSWD,
			 string DB_NAME,unsigned int PORT,string UNIX_SOCKET,
			 unsigned long CLIENT_FLAG){
  MYSQL*ret=mysql_real_connect(mysql,HOST.c_str(),USER.c_str(),PASSWD.c_str(),
			       DB_NAME.c_str(),PORT,UNIX_SOCKET.c_str(),CLIENT_FLAG);
  if(!ret){
    syslog(LOG_ERR,"------<maria_real_connect error>------\nhost: %s\nuser: %s\ndb_name: %s\nport: %ud\n------</maria_real_connect error>\n",HOST.c_str(),USER.c_str(),DB_NAME.c_str(),PORT);    
    cerr<<"------<maria_real_connect error>------\nhost: "<<HOST<<"\nuser: "<<USER<<"\ndb_name: "<<DB_NAME<<"\nport: "<<PORT<<"\n-------</maria_real_connect error>------"<<endl;
  }
  return ret;
}
