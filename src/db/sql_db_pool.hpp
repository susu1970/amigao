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

#ifndef AMIGAO_DB_SQL_DB_POOL_H_
#define AMIGAO_DB_SQL_DB_POOL_H_

#include<mariadb/mysql.h>
#include<pthread.h>
#include<unistd.h>

#include"interface/db_pool.h"

extern MYSQL*maria_real_connect(MYSQL*mysql,std::string HOST,
				std::string USER,std::string PASSWD,
				std::string DB_NAME,unsigned int PORT,
				std::string UNIX_SOCKET,
				unsigned long CLIENT_FLAG);
extern int maria_real_query(MYSQL*mysql,std::string);

namespace amigao{
  class SqlDBPool:public DBPoolInterface{
    virtual void add_connection(int add_numbers)override{
      using namespace std;
      pthread_mutex_lock(&unused_lock);
      if(unused_pool->get_node_numbers()+add_numbers>MAX_IDLE){
	add_numbers=MAX_IDLE-unused_pool->get_node_numbers();
      }
      for(int i=0;i<add_numbers;++i){//adding...
	MYSQL*mysql=new MYSQL();
	mysql_init(mysql);
	if(!maria_real_connect(mysql,HOST,USER,PASSWD,
			       DB_NAME,PORT,UNIX_SOCKET,CLIENT_FLAG)){
	  pthread_mutex_unlock(&unused_lock);
	  exit(1);
	}
	string set_names="set names utf8mb4";
	maria_real_query(mysql,set_names);
	unused_pool->add_node((void*)mysql);
      }
      pthread_mutex_unlock(&unused_lock);
    }
    virtual void delete_connection(int delete_numbers)override{
      //delete from unused
      pthread_mutex_lock(&unused_lock);
      if(unused_pool->get_node_numbers()-delete_numbers<MIN_IDLE){
	delete_numbers=unused_pool->get_node_numbers()-MIN_IDLE;
      }
      for(int i=0;i<delete_numbers;++i)
	unused_pool->delete_node();
      pthread_mutex_unlock(&unused_lock);
    }
  public:
    explicit SqlDBPool(std::string sql_db_conf_path_):DBPoolInterface(sql_db_conf_path_){
      add_connection(MIN_IDLE);
    }
    virtual void*get_connection()override{
    again:
      pthread_mutex_lock(&unused_lock);
      pthread_mutex_lock(&used_lock);
      if(used_pool->get_node_numbers()==MAX_CONN){
	pthread_mutex_unlock(&used_lock);
	pthread_mutex_unlock(&unused_lock);
	sleep(SLEEP);
	goto again;
      }
      if(unused_pool->get_node_numbers()<=MIN_IDLE){
	pthread_mutex_unlock(&unused_lock);
	pthread_mutex_unlock(&used_lock);
	add_connection(MIN_IDLE-unused_pool->get_node_numbers()+1);
	goto again;
      }
      void *tmp_p=unused_pool->pop_connection2(used_pool);
      pthread_mutex_unlock(&used_lock);
      pthread_mutex_unlock(&unused_lock);
      return tmp_p;
    }       
    virtual void recycle_connection(void*connection)override{
      pthread_mutex_lock(&used_lock);
      if(!(used_pool->if_exist(connection))){
	pthread_mutex_unlock(&used_lock);
	return;
      }
      pthread_mutex_lock(&unused_lock);
      if(unused_pool->get_node_numbers()==MAX_IDLE){
	used_pool->delete_node(connection);
	pthread_mutex_unlock(&used_lock);
	pthread_mutex_unlock(&unused_lock);
	return;
      }
      used_pool->pop_connection2(unused_pool,connection);
      pthread_mutex_unlock(&used_lock);
      pthread_mutex_unlock(&unused_lock);
    }
    int get_used_pool_node_numbers(){
      return used_pool->get_node_numbers();
    }
    int get_ununsed_pool_node_numbers(){
      return unused_pool->get_node_numbers();
    }
    ~SqlDBPool(){
      pthread_mutex_lock(&unused_lock);
      pthread_mutex_lock(&used_lock);
      while(used_pool->get_node_numbers()>0){
	MYSQL*mysql=(MYSQL*)used_pool->get_connection();
	mysql_close(mysql);
	delete mysql;
	mysql=nullptr;
	used_pool->delete_node();
      }
      while(unused_pool->get_node_numbers()>0){
	MYSQL*mysql=(MYSQL*)unused_pool->get_connection();
	mysql_close(mysql);
	delete mysql;
	mysql=nullptr;
	unused_pool->delete_node();
      }
      pthread_mutex_unlock(&unused_lock);
      pthread_mutex_unlock(&used_lock);
    }
  };
}

#endif
