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

#ifndef AMIGAO_INTERFACE_DB_POOL_H_
#define AMIGAO_INTERFACE_DB_POOL_H_

#include<string>
#include<pthread.h>

#include"db/db_pool_list.hpp"
#include"db/db_info.hpp"

namespace amigao{
  class DBPoolInterface:public DBInfo{
  protected:
    DBPoolList*unused_pool=nullptr;
    DBPoolList*used_pool=nullptr;
    pthread_mutex_t unused_lock;//to make thread-safety
    pthread_mutex_t used_lock;
  public:
    DBPoolInterface(std::string db_conf_path_):DBInfo(db_conf_path_){
      unused_pool=new DBPoolList();
      used_pool=new DBPoolList();
      pthread_mutex_init(&unused_lock,nullptr);
      pthread_mutex_init(&used_lock,nullptr);
    }
    DBPoolInterface(int MAX_CONN_,int MAX_IDLE_,int MIN_IDLE_,int SLEEP_,
	   std::string HOST_,std::string USER_,
	   std::string PASSWD_,std::string DB_NAME_,
	   std::string UNIX_SOCKET_,
	   unsigned long CLIENT_FLAG_,
	   unsigned int PORT_)
      :DBInfo(MAX_CONN_,MAX_IDLE_,MIN_IDLE_,SLEEP_,
	      HOST_,USER_,
	      PASSWD_,DB_NAME_,
	      UNIX_SOCKET_,
	      CLIENT_FLAG_,
	      PORT_){    
      unused_pool=new DBPoolList();
      used_pool=new DBPoolList();
      pthread_mutex_init(&unused_lock,nullptr);
      pthread_mutex_init(&used_lock,nullptr);
    }
    virtual void* get_connection()=0;
    virtual void recycle_connection(void*)=0;
    virtual void add_connection(int add_numbers)=0;
    virtual void delete_connection(int delete_numbers)=0;
    virtual ~DBPoolInterface(){
      pthread_mutex_destroy(&unused_lock);
      pthread_mutex_destroy(&used_lock);
      delete unused_pool;
      delete used_pool;
    }
  };
}


#endif
