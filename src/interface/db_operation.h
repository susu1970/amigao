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

#ifndef AMIGAO_INTERFACE_DB_OPERATION_H_
#define AMIGAO_INTERFACE_DB_OPERATION_H_

#include<unordered_map>
#include<unordered_set>

#include"interface/db_pool.h"
#include"interface/search_strategy.h"
#include"interface/valuate_strategy.h"

namespace amigao{
  class DBOperationInterface{
  protected:
    DBPoolInterface*db_pool;
    long MAX_INSERT;
  public:
    long get_MAX_INSERT(){
      return MAX_INSERT;
    }
    void* get_db(){
      return (db_pool->get_connection());
    }
    void recycle_db(void* conn){
      db_pool->recycle_connection((void*)conn);
    }
    void search_sentence(std::string sentence,SearchStrategyInterface*search_strategy,void*search_result,short start_page=1,short results_per_page=10){
      search_strategy->handle(sentence,search_result,this,start_page,results_per_page);
    }
    void valuate_page(std::string url,
		      std::string &title,
		      std::string &contents,
		      std::string &html,
		      ValuateStrategyInterface*valuate_strategy){
      valuate_strategy->handle(url,title,contents,html,this);
    }
    virtual void add_dict_table(std::string dict_name,
				std::unordered_map<std::string,std::string>*word_no)=0;
    virtual void add_dict_table(std::string dict_name,
				std::string dict_file_path)=0;
    virtual void add_dict_table(std::string dict_name,
				std::unordered_set<std::string>*word_unordered_set)=0;
    virtual bool add_dict_table(std::string dict_name)=0;
    virtual void op_website_nice(std::string url,
				 std::string nice)=0;
    virtual void op_visited_url(std::string url,
				std::string&title,
				std::string&contents,
				std::string&html)=0;
    virtual unsigned int get_url_no(std::string url)=0;
    virtual short get_website_nice(std::string url)=0;
    virtual void rm_dict_table(std::string dict_name)=0;
    virtual bool if_exist(std::string select,
			  std::string from,
			  std::string column,
			  std::string value)=0;
    virtual void op_dict_manager(std::string dict_name,
				 std::string status,
				 std::string dict_no,
				 std::string comments)=0;
    virtual void rm_dict_manager(std::string dict_name)=0;
     virtual void op_strategy_manager(std::string strategy_name,
				     std::string status,
				     std::string comment)=0;
    virtual void rm_strategy_manger(std::string strategy_name)=0;
    virtual ~DBOperationInterface(){
      delete db_pool;
    }
  };
}

#endif
