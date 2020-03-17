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

#ifndef AMIGAO_DB_SQL_DB_OPERATION_H_
#define AMIGAO_DB_SQL_DB_OPERATION_H_

#include"interface/db_operation.h"
#include"db/sql_db_pool.hpp"

#include<fstream>
#include<map>
#include<algorithm>
#include<sstream>

extern std::string crypt(const std::string&text);
extern std::string decrypt(const std::string&text);
extern int maria_real_query(MYSQL*mysql,std::string query);

namespace amigao{
  class SqlDBOperation:public DBOperationInterface{
  public:
    explicit SqlDBOperation(std::string sql_db_conf_path_){
      using namespace std;
      MAX_INSERT=1000;
      db_pool=new SqlDBPool(sql_db_conf_path_);
      MYSQL*mysql=(MYSQL*)db_pool->get_connection();
      std::string init_MAX_INSERT="show variables like '%max_allowed_packet%'";
      if(maria_real_query(mysql,init_MAX_INSERT)){
	db_pool->recycle_connection((void*)mysql);
	return;
      }
      MYSQL_RES*res=nullptr;
      if(!(res=mysql_store_result(mysql))){
	mysql_free_result(res);
	db_pool->recycle_connection((void*)mysql);
	return;
      }
      MYSQL_ROW row;
      row=mysql_fetch_row(res);
      MAX_INSERT=MAX_INSERT>atol(row[1])/520?MAX_INSERT:(atol(row[1])/520);
      mysql_free_result(res);
      db_pool->recycle_connection((void*)mysql);
    }
    virtual void add_dict_table(std::string dict_name,
				std::map<std::string,std::string>*word_no)override{
      using namespace std;
      if(!add_dict_table(dict_name))return;
      dict_name=crypt(dict_name);
      MYSQL*mysql=(MYSQL*)db_pool->get_connection();
      string insert="insert into "+(dict_name)+" (no,word) values ";
      string value="";
      for(auto iter=word_no->begin();iter!=word_no->end();++iter){
	if(insert.size()+value.size()+iter->second.size()+crypt(iter->first).size()>MAX_INSERT){
	  if(value.size()>6)
	    if(maria_real_query(mysql,insert+value.substr(0,value.size()-1))){
	      db_pool->recycle_connection((void*)mysql);
	      return;
	    }
	  value="";
	}
	value+="("+iter->second+",'"+crypt(iter->first)+"'),";
      }
      if(value.size()>6){
	insert+=(value.substr(0,value.size()-1));
	maria_real_query(mysql,insert);
      }
      db_pool->recycle_connection((void*)mysql);
    }
    virtual void add_dict_table(std::string dict_name,
				std::string dict_file_path)override{
      using namespace std;
      ifstream ifs(dict_file_path);
      if(!ifs.is_open())
	return;
      string tmp_str="";
      set<string>*word_set=new set<string>;
      while(std::getline(ifs,tmp_str)){
	for(size_t i=0;i<tmp_str.size();++i)
	  if(tmp_str[i]>='A'&&tmp_str[i]<='Z')
	    tmp_str[i]+=32;
	if(!tmp_str.empty())
	  word_set->insert(tmp_str);
      }
      ifs.close();
      add_dict_table(dict_name,word_set);
      delete word_set;
    }
    virtual void add_dict_table(std::string dict_name,
				std::set<std::string>*word_set)override{
      using namespace std;
      if(!add_dict_table(dict_name)||word_set->empty())return;
      dict_name=crypt(dict_name);
      MYSQL*mysql=(MYSQL*)db_pool->get_connection();
      string insert="insert into "+(dict_name)+" (word) values ";
      string item="";
      for(auto iter=word_set->begin();iter!=word_set->end();++iter){
	string tmp_str=crypt(*iter);
	if(tmp_str.size()+item.size()-1+insert.size()>MAX_INSERT){
	  if(item.size()>5)
	    if(maria_real_query(mysql,insert+item.substr(0,item.size()-1))){
	      db_pool->recycle_connection((void*)mysql) ;
	      return;
	    }
	  item="";
	}
	if(!tmp_str.empty())
	  item+=("('"+tmp_str+"'),");
      }
      if(item.size()>5)
	maria_real_query(mysql,insert+item.substr(0,item.size()-1));
      db_pool->recycle_connection((void*)mysql);      
    }
    virtual bool add_dict_table(std::string dict_name)override{
      using namespace std;
      if(dict_name.empty())return false;
      MYSQL*mysql=(MYSQL*)db_pool->get_connection();
      string clean_dict="drop table if exists "+crypt(dict_name);
      if(maria_real_query(mysql,clean_dict)){
	db_pool->recycle_connection((void*)mysql);
	return false;
      }
      string create_dict="create table "+crypt(dict_name)+" (no int unsigned primary key auto_increment,word   char(255) not null)";
      if(maria_real_query(mysql,create_dict)){
	db_pool->recycle_connection((void*)mysql);
	return false;
      }
      op_dict_manager(dict_name,"","","");
      db_pool->recycle_connection((void*)mysql);
      return true;
    }
    virtual void op_website_nice(std::string url,
				 std::string nice)override{
      using namespace std;
      MYSQL*mysql=(MYSQL*)db_pool->get_connection();
      if(!if_exist("url","website_nice","url",(url))){
	string insert="insert into amigao.website_nice (url,nice) values ('"+crypt(url)+"','"+nice+"')";
	maria_real_query(mysql,insert);
	db_pool->recycle_connection((void*)mysql);
	return;
      }
      string update="update amigao.website_nice set nice='"+nice+"' where url like '"+crypt(url)+"'";
      maria_real_query(mysql,update);
      db_pool->recycle_connection((void*)mysql);
    }
    virtual void op_visited_url(std::string url,
				std::string&title,
				std::string&contents,
				std::string&html)override{
      using namespace std;
      MYSQL*mysql=(MYSQL*)db_pool->get_connection();
      if(title.empty())title="#empty";
      if(contents.empty())contents="#empty";
      //      if(html.empty())
	html="#empty()";
      if(if_exist("url","visited_url","url",url)){
	string update="update amigao.visited_url set title='"+crypt(title)+"',contents='"+crypt(contents)+"',html='"+crypt(html)+"' where url like '"+crypt(url)+"'";
	maria_real_query(mysql,update);
	db_pool->recycle_connection((void*)mysql);
	return;
      }
      string insert="insert into amigao.visited_url (url,title,contents,html) values ('"+crypt(url)+"','"+crypt(title)+"','"+crypt(contents)+"','"+crypt(html)+"')";
      maria_real_query(mysql,insert);
      db_pool->recycle_connection((void*)mysql);
    }
    virtual unsigned int get_url_no(std::string url)override{
      if(url.empty())return 0;
      using namespace std;
      stringstream ss;
      unsigned int ret=0;
      MYSQL*mysql=(MYSQL*)db_pool->get_connection();
    again:
      if(if_exist("url","visited_url","url",url)){
	string query="select no from amigao.visited_url where url like '"+crypt(url)+"'";
	if(maria_real_query(mysql,query)){
	  db_pool->recycle_connection((void*)mysql);
	  return 0;
	}
	MYSQL_RES*res=nullptr;
	MYSQL_ROW row;
	if(!(res=mysql_store_result(mysql))){
	  db_pool->recycle_connection((void*)mysql);
	  mysql_free_result(res);
	  return 0;
	}
	if(row=mysql_fetch_row(res)){
	  mysql_free_result(res);
	  db_pool->recycle_connection((void*)mysql);
	  ss<<row[0];
	  ss>>ret;
	  ss.clear();
	  return ret;
	}
	mysql_free_result(res);
	db_pool->recycle_connection((void*)mysql);
	return 0;
      }
      string insert="insert into amigao.visited_url (url) values ('"+crypt(url)+"')";
      if(maria_real_query(mysql,insert)){
	db_pool->recycle_connection((void*)mysql);
	return 0;
      }
      goto again;
    }
    virtual short get_website_nice(std::string url)override{
      if(!if_exist("nice","amigao.website_nice","url",url))
	return 10;
      using namespace std;
      url=crypt(url);
      MYSQL*mysql=(MYSQL*)db_pool->get_connection();
      MYSQL_ROW row;
      MYSQL_RES*res=nullptr;
      string query="select nice from amigao.website_nice where url like '"+url+"'";
      if(maria_real_query(mysql,query)){
	db_pool->recycle_connection((void*)mysql);
	return 10;
      }
      if(!(res=mysql_store_result(mysql))){
	db_pool->recycle_connection((void*)mysql);
	mysql_free_result(res);
	return 10;
      }
      short ret=10;
      stringstream ss;
      while(row=mysql_fetch_row(res)){
	ss<<row[0];
	short tmp;
	ss>>tmp;
	ss.clear();
	if(tmp<ret)ret=tmp;
      }
      db_pool->recycle_connection((void*)mysql);
      mysql_free_result(res);
      return ret;
    }
    virtual void rm_dict_table(std::string dict_name)override{
      using namespace std;
      rm_dict_manager(dict_name);
      MYSQL*mysql=(MYSQL*)db_pool->get_connection();
      string rm="drop table if exists "+crypt(dict_name);
      maria_real_query(mysql,rm);
      db_pool->recycle_connection((void*)mysql);      
    }
    virtual bool if_exist(std::string select,
			  std::string from,
			  std::string column,
			  std::string value)override{
      using namespace std;
      MYSQL*mysql=(MYSQL*)db_pool->get_connection();
      string query="select "+crypt(select)+" from "+crypt(from)+" where "+crypt(column)+" like '"+crypt(value)+"'";
      if(maria_real_query(mysql,query)){
	db_pool->recycle_connection((void*)mysql);
	return false;
      }
      MYSQL_RES*res=nullptr;
      if(!(res = mysql_store_result(mysql))){
	db_pool->recycle_connection((void*)mysql);
	mysql_free_result(res);
	return false;
      }
      db_pool->recycle_connection((void*)mysql);
      if(!mysql_fetch_row(res)){
	mysql_free_result(res);      
	return false;
      }
      mysql_free_result(res);      
      return true;
    }
    virtual void op_dict_manager(std::string dict_name,
				 std::string status,
				 std::string dict_no,
				 std::string comments)override{
      using namespace std;
      if(status.empty())status="#default";
      stringstream ss;
      ss<<dict_no;
      int tmp_no;
      ss>>tmp_no;
      ss.clear();
      status=crypt(status);
      comments=crypt(comments);
      if(tmp_no>=10000||tmp_no<1000)dict_no="";
      MYSQL*mysql=(MYSQL*)db_pool->get_connection();
      if(!if_exist("dict_name","dictionary.dict_manager","dict_name",dict_name)){
	string insert;
	if(dict_no.empty())
	  insert="insert into dictionary.dict_manager (dict_name,status,comments) values ('"+crypt(dict_name)+"','"+status+"','"+comments+"')";
	else
	  insert="insert into dictionary.dict_manager (dict_name,status,dict_no,comments) values ('"+crypt(dict_name)+"','"+status+"','"+dict_no+"','"+comments+"')";
	maria_real_query(mysql,insert);
	db_pool->recycle_connection((void*)mysql);
	return;
      }
      dict_name=crypt(dict_name);
      string update="";
      if(dict_no.empty())
	update="update dictionary.dict_manager set dict_name='"+dict_name+"',status='"+status+"',comments='"+comments+"'";
      else
	update="update dictionary.dict_manager set dict_name='"+dict_name+"',status='"+status+"',comments='"+comments+"',dict_no="+dict_no;	   
      maria_real_query(mysql,update);
      db_pool->recycle_connection((void*)mysql);
    }
    virtual void op_strategy_manager(std::string strategy_name,
				     std::string status,
				     std::string comments)override{
      using namespace std;
      MYSQL*mysql=(MYSQL*)db_pool->get_connection();
      if(status.empty())status="#default";
      if(comments.empty())comments="default";
      status=crypt(status);
      comments=crypt(comments);
      if(!if_exist("strategy_name","amigao.strategy_manager","strategy_name",strategy_name)){
	string insert="insert into amigao.strategy_manager (strategy_name,status,comments) values ('"+crypt(strategy_name)+"','"+status+"','"+comments+"')";
	maria_real_query(mysql,insert);
	db_pool->recycle_connection((void*)mysql);
	return;
      }
      string update="update amigao.strategy_manager set ";
      if(status.empty()){
	if(comments.empty()){
	  db_pool->recycle_connection((void*)mysql);
	  return; 
	}
	update+=("comments='"+comments+"',");
      }
      else{
	update+=("status='"+status+"',");
	if(!comments.empty())
	  update+=("comments='"+comments+"',");
      }
      if(update.empty()){
	maria_real_query(mysql,update);
	db_pool->recycle_connection((void*)mysql);
	return;	
      }
      update=update.substr(0,update.size()-1);
      update+=(" where strategy_name='"+strategy_name+"'");
      maria_real_query(mysql,update);
      db_pool->recycle_connection((void*)mysql);
    }
    virtual void rm_strategy_manger(std::string strategy_name)override{
      if(!if_exist("strategy_name","amigao.strategy_manager","strategy_name",strategy_name))return;
      using namespace std;
      strategy_name=crypt(strategy_name);
      MYSQL*mysql=(MYSQL*)db_pool->get_connection();
      string rm="delete from amigao.strategy_manager where strategy_name='"+strategy_name+"'";
      maria_real_query(mysql,rm);
      db_pool->recycle_connection((void*)mysql);      
    }
    virtual void rm_dict_manager(std::string dict_name)override{
      using namespace std;
      if(!if_exist("dict_name","dictionary.dict_manager","dict_name",dict_name))
	return;
      dict_name=crypt(dict_name);
      MYSQL*mysql=(MYSQL*)db_pool->get_connection();
      string rm="delete from dictionary.dict_manager where dict_name='"+dict_name+"'";
      maria_real_query(mysql,rm);
      db_pool->recycle_connection((void*)mysql);      
    }
    ~SqlDBOperation(){
    }
  };
}

#endif
