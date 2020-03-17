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

#ifndef AMIGAO_STRATEGY_REVERSE_VALUATE_STRATEGY_H_
#define AMIGAO_STRATEGY_REVERSE_VALUATE_STRATEGY_H_

#include<string>
#include<map>
#include<vector>

#include"interface/valuate_strategy.h"
#include"utils/ini_parser.hpp"
#include"utils/cppjieba/Jieba.hpp"
#include"db/sql_db_operation.hpp"


namespace amigao{
  class ReverseValuateStrategy:public ValuateStrategyInterface{
    std::string DICT_PATH="";
    std::string HMM_PATH="";
    std::string USER_DICT_PATH=""; 
    std::string IDF_PATH="";
    std::string STOP_WORD_PATH="";
    void op_reverse(std::string &url,
		    std::map<std::string,unsigned int>*weight,
		    DBOperationInterface*db_op){
      using namespace std;
      long cur_url_no;
      if((cur_url_no=db_op->get_url_no(url))==0)return;
      MYSQL*mysql=(MYSQL*)db_op->db_pool->get_connection();
      MYSQL_RES*res=nullptr;
      MYSQL_ROW row;
      string get_max_url_per_word="select status from amigao.strategy_manager where strategy_name like 'reverse'";
      int MAX_URLS=1000;
      if(maria_real_query(mysql,get_max_url_per_word)||!(res=mysql_store_result(mysql))){
	  db_op->db_pool->recycle_connection((void*)mysql);
	  mysql_free_result(res);
	  return;
      }
      if(row=mysql_fetch_row(res)){
	stringstream ss;
	string tmp_str=row[0];
	tmp_str=decrypt(tmp_str);
	long pos=tmp_str.find("#max_url_per_word=");
	if(pos!=-1&&pos+18<tmp_str.size()){
	  tmp_str=tmp_str.substr(pos+18,tmp_str.size());
	  long pos1=tmp_str.find("#");
	  if(pos1!=-1){
	    ss<<tmp_str.substr(0,pos1);
	    ss>>MAX_URLS;
	    ss.clear();
	  }else{
	    ss<<tmp_str.substr(0,tmp_str.size());
	    ss>>MAX_URLS;
	    ss.clear();
	  }
	}
      }
      mysql_free_result(res);
      res=nullptr;
      stringstream ss;
      ss<<cur_url_no;
      string cur_str_url_no;
      ss>>cur_str_url_no;
      ss.clear();
      auto update_weight=[&](string &url_weight_str,string &word_)->void{
			   //first find old url node check it whether changed,and remove it
			   int cur_weight=(*weight)[word_];
			   if(cur_weight==0)
			     return;
			   long i=0;
			   string cur_str_weight;
			   ss<<cur_weight;
			   ss>>cur_str_weight;
			   ss.clear();
			   string url_node="|"+cur_str_weight+"_"+cur_str_url_no;
			   long old_url_node=0;
			   while(i<url_weight_str.size()&&url_weight_str[i]!='|')++i;
			   if(i==url_weight_str.size())return;
			   long first_line=i;
			   string cur_nums_str=url_weight_str.substr(0,i);
			   unsigned cur_nums;
			   ss<<cur_nums_str;
			   ss>>cur_nums;
			   ss.clear();
			   if((old_url_node=url_weight_str.find("_"+cur_str_url_no))!=-1){
			     long i=old_url_node,j=i;
			     for(;i>=0&&url_weight_str[i]!='|';--i);
			     for(;j<url_weight_str.size()&&
				   url_weight_str[j]!='|';++j);
			     if(i<url_weight_str.size()&&j>=i&&
				url_weight_str.substr(i,j-i).compare(url_node)==0){
			       return;
			     }
			     if(cur_nums==1){
			       url_weight_str="1"+url_node;
			       return;
			     }
			     if(j==url_weight_str.size()){
			       url_weight_str=cur_nums_str+url_weight_str.substr(first_line,i-first_line)+url_node;
			       return;
			     }
			     --cur_nums;
			     ss<<cur_nums;
			     ss>>cur_nums_str;
			     ss.clear();
			     url_weight_str=cur_nums_str+url_weight_str.substr(first_line,i-first_line);			       
			   }
			   for(long j;i<url_weight_str.size();++i){
			     if(url_weight_str[i]=='|')j=i+1;
			     else if(url_weight_str[i]=='_'){
			       if(j>=url_weight_str.size()||i<=j||j==0)return;
			       long tmp_weight;
			       ss<<url_weight_str.substr(j,i-j);
			       ss>>tmp_weight;
			       ss.clear();
			       if(cur_weight>=tmp_weight){
				 bool full=false;
				 if(cur_nums<MAX_URLS){
				   ++cur_nums;
				 }else
				   full=true;
				 ss<<cur_nums;
				 ss>>cur_nums_str;
				 ss.clear();
				 string head;
				 int i1=i;
				 if(j-1==first_line)
				   head=cur_nums_str+url_node;
				 else
				   head=cur_nums_str+url_weight_str.substr(first_line,j-1-first_line)+url_node;
				 if(!full){
				   url_weight_str=head+url_weight_str.substr(j-1,url_weight_str.size());
				 }
				 else{
				   long l_tail=url_weight_str.size()-1;
				   while(l_tail>=0&&url_weight_str[l_tail]!='|')--l_tail;
				   if(l_tail>=0&&url_weight_str[l_tail]=='|'&&l_tail>=j-1){
				     if(l_tail==j-1)
				       url_weight_str=head;
				     else
				       url_weight_str=head+url_weight_str.substr(j-1,l_tail-j+1);
				   }
				   else return;
				 }
				 break;
			       }
			     }
			   }
			   if(i==url_weight_str.size()&&cur_nums<MAX_URLS){
			     ++cur_nums;
			     ss<<cur_nums;
			     ss>>cur_nums_str;
			     ss.clear();
			     url_weight_str=cur_nums_str+url_weight_str.substr(first_line,url_weight_str.size())+url_node;
			   }
			 };
      string select="select word,weight from amigao.reverse where 0>1 ";
      string update="update amigao.reverse set weight=case word ";
      string insert="insert into amigao.reverse (word,weight) values ";
      string when_then="",word_in="",tail="";
      vector<string>*select_vec=new vector<string>;
      vector<string>*update_vec=new vector<string>;
      vector<string>*insert_vec=new vector<string>;
      for(auto iter=weight->begin();iter!=weight->end();++iter){
	string tmp_str=(iter->first);
	if(tmp_str.size()+tail.size()+select.size()+16>db_op->MAX_INSERT){
	  select_vec->push_back(tail);
	  tail="";
	}
	tail+=("or word like '"+tmp_str+"' ");
      }
      if(tail.size()>16)
	select_vec->push_back(tail);
      tail="";
      string lock="lock table amigao.reverse write",unlock="unlock tables";
      if(maria_real_query(mysql,lock)){//lock reverse
	db_op->db_pool->recycle_connection((void*)mysql);
	delete select_vec;delete update_vec;delete insert_vec;
	return;
      }
      for(auto const&select_:(*select_vec)){
	if(maria_real_query(mysql,select+select_)||
	   !(res=mysql_store_result(mysql))){
	  delete select_vec;delete update_vec;delete insert_vec;
	  mysql_free_result(res);
	  maria_real_query(mysql,unlock);
	  db_op->db_pool->recycle_connection((void*)mysql);
	  return;
	}
	while(row=mysql_fetch_row(res)){
	  string word=row[0];
	  if(word.empty()||weight->find(word)==weight->end())continue;
	  string url_weight=row[1];	  
	  update_weight(url_weight,word);
	  if(update.size()+when_then.size()+word_in.size()+word.size()*2+url_weight.size()+18>db_op->MAX_INSERT&&word_in.size()>0){
	    string tmp_str=when_then+" end where word in ("+word_in.substr(0,word_in.size()-1)+")";
	    update_vec->push_back(tmp_str);
	    when_then="";word_in="";
	  }
	  when_then+=("when '"+word+"' then '"+url_weight+"' ");
	  word_in+=("'"+word+"',");
	  weight->erase(word);
	}
	mysql_free_result(res);res=nullptr;
      }
      for(auto iter=weight->begin();iter!=weight->end();++iter){
	ss<<(iter->second);
	int tmp_weight_;
	ss>>tmp_weight_;
	ss.clear();
	if(tmp_weight_==0)continue;
	string tmp_weight=to_string(tmp_weight_);
	if(tail.size()+insert.size()+7+(iter->first).size()+(tmp_weight).size()>db_op->MAX_INSERT){
	  if(!tail.empty())
	    insert_vec->push_back(tail.substr(0,tail.size()-1));
	  tail="";
	}
	tail+=("('"+iter->first+"','1|"+tmp_weight+"_"+cur_str_url_no+"'),");
      }
      if(tail.size()>8){
	insert_vec->push_back(tail.substr(0,tail.size()-1));
      }
      for(auto const&insert_:(*insert_vec)){
	if(maria_real_query(mysql,insert+insert_)){
	  maria_real_query(mysql,unlock);
	  delete select_vec;delete update_vec;delete insert_vec;
	  db_op->db_pool->recycle_connection((void*)mysql);
	  return;
	}
      }
      if(maria_real_query(mysql,unlock)){
	  delete select_vec;delete update_vec;delete insert_vec;
	  db_op->db_pool->recycle_connection((void*)mysql);
	  return;
      }
      for(auto const &update_:(*update_vec))
	if(maria_real_query(mysql,update+update_)){
	  delete select_vec;delete update_vec;delete insert_vec;
	  db_op->db_pool->recycle_connection((void*)mysql);
	  return;
	}
      delete select_vec;delete update_vec;delete insert_vec;
      db_op->db_pool->recycle_connection((void*)mysql);
    }
  public:
    explicit ReverseValuateStrategy(std::string cppjieba_dict_conf_ini_){
      using namespace std;
      IniParser iniParser(cppjieba_dict_conf_ini_);
      DICT_PATH=iniParser.get_value("DICT_PATH");
      HMM_PATH=iniParser.get_value("HMM_PATH");
      USER_DICT_PATH=iniParser.get_value("USER_DICT_PATH");
      IDF_PATH=iniParser.get_value("IDF_PATH");
      STOP_WORD_PATH=iniParser.get_value("STOP_WORD_PATH");
    }
    virtual void handle(std::string url,
			std::string &title,
			std::string &contents,
			std::string &html,
			DBOperationInterface*db_op)override{
      using namespace std;
      if(url.size()>2048||!db_op)return;
      cppjieba::Jieba jieba(DICT_PATH,
			    HMM_PATH,
			    USER_DICT_PATH,
			    IDF_PATH,
			    STOP_WORD_PATH);
      vector<string>* title_vec=new vector<string>;
      vector<string>* contents_vec=new vector<string>;
      jieba.CutForSearch(title,*title_vec,true);
      jieba.CutForSearch(contents,*contents_vec,true);
      map<string,unsigned int>*weight=new map<string,unsigned int>;
      for(auto&e:*title_vec)
	if(!e.empty()&&e.size()<=255)
	  (*weight)[crypt(e)]+=10;
      for(auto&e:*contents_vec)
	if(!e.empty()&&e.size()<=255)
	  ++(*weight)[crypt(e)];
      delete title_vec;delete contents_vec;
      db_op->op_visited_url(url,title,contents,html);
      op_reverse(url,weight,db_op);
      delete weight;
      return;
    }
    ~ReverseValuateStrategy(){
    }
  };
}

#endif
