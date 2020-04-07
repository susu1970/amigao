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

#ifndef AMIGAO_STRATEGY_REVERSE_VALUATE_STRATEGY_H_
#define AMIGAO_STRATEGY_REVERSE_VALUATE_STRATEGY_H_

#include<iostream>
#include<string>
#include<unordered_map>
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
    int MAX_URLS=800;
    cppjieba::Jieba*jieba;
  public:
    explicit ReverseValuateStrategy(std::string cppjieba_dict_conf_ini_,DBOperationInterface*db_op){
      using namespace std;
      IniParser iniParser(cppjieba_dict_conf_ini_);
      DICT_PATH=iniParser.get_value("DICT_PATH");
      HMM_PATH=iniParser.get_value("HMM_PATH");
      USER_DICT_PATH=iniParser.get_value("USER_DICT_PATH");
      IDF_PATH=iniParser.get_value("IDF_PATH");
      STOP_WORD_PATH=iniParser.get_value("STOP_WORD_PATH");
      jieba=new cppjieba::Jieba(DICT_PATH,
				HMM_PATH,
				USER_DICT_PATH,
				IDF_PATH,
				STOP_WORD_PATH);
      MYSQL*mysql=(MYSQL*)db_op->get_db();
      MYSQL_RES*res=nullptr;
      MYSQL_ROW row;
      string get_max_url_per_word="select status from amigao.strategy_manager where strategy_name='reverse'";
      if(maria_real_query(mysql,get_max_url_per_word)||!(res=mysql_store_result(mysql))){
	db_op->recycle_db((void*)mysql);
	mysql_free_result(res);
	return;
      }
      if(row=mysql_fetch_row(res)){
	stringstream ss;
	string tmp_str=row[0];
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
      db_op->recycle_db((void*)mysql);
      mysql_free_result(res);
    }
    virtual void handle(std::string &url,
			std::string &title,
			std::string &contents,
			std::string &html,
			DBOperationInterface*db_op)override{
      using namespace std;
      if(url.size()>2048||!db_op)return;
      db_op->op_visited_url(url,title,contents,html);
      long cur_url_no;
      if((cur_url_no=db_op->get_url_no(url))==0)
	return;
      string cur_str_url_no=to_string(cur_url_no);
      MYSQL*mysql=(MYSQL*)db_op->get_db();
      MYSQL_ROW row;
      MYSQL_RES*res=nullptr;
      const string update_wordno_weight="update reverse set weight= case no ";
      vector<string>update_wordno_weight_vec;
      {
	unordered_map<string,unsigned int>wordno_weight;
	string tail="",when_then="",no_in="";
	const string select_wordno_weight="select no,weight from amigao.reverse where 0>1 ";
	vector<string>select_wordno_weight_vec;
	{
	  unordered_map<string,unsigned int>word_weight;
	  vector<string>find_wordno_vec;
	  const string find_wordno="select no,word from dictionary.word where 0>1 ";
	  {
	    vector<string>title_vec;
	    jieba->CutForSearch(title,title_vec,true);
	    for(auto const&e:title_vec){
	      string tmp_str="";
	      for(size_t i=0;i<e.size();++i)
		if(e[i]!=' '&&e[i]!='\r'&&e[i]!='\n'&&e[i]!='\t')
		  tmp_str+=e[i];
	      tmp_str=crypt(tmp_str);
	      if(tmp_str.empty()||tmp_str.size()>768)continue;
	      word_weight[tmp_str]+=10;
	      if(find_wordno.size()+tail.size()+tmp_str.size()+11>db_op->get_MAX_INSERT()){
		find_wordno_vec.push_back(tail);
		tail="";
	      }
	      tail+="or word='"+tmp_str+"' ";
	    }
	    if(tail.size()>11)find_wordno_vec.push_back(tail);
	    tail="";
	  }
	  {
	    vector<string>contents_vec;
	    jieba->CutForSearch(contents,contents_vec,true);
	    for(auto const&e:contents_vec){
	      string tmp_str="";
	      for(size_t i=0;i<e.size();++i)
		if(e[i]!=' '&&e[i]!='\r'&&e[i]!='\n'&&e[i]!='\t')
		  tmp_str+=e[i];
	      tmp_str=crypt(tmp_str);
	      if(tmp_str.empty()||tmp_str.size()>768)continue;
	      ++word_weight[tmp_str];
	      if(find_wordno.size()+tail.size()+tmp_str.size()+11>db_op->get_MAX_INSERT()){
		find_wordno_vec.push_back(tail);
		tail="";
	      }
	      tail+="or word='"+tmp_str+"' ";
	    }
	    if(tail.size()>11)find_wordno_vec.push_back(tail);
	    tail="";
	  }
	  for(auto const&e:find_wordno_vec){
	    if(maria_real_query(mysql,find_wordno+e)||
	       !(res=mysql_store_result(mysql))){
	      cerr<<"---<reverse valuate strategy error>---\nmaria_real_query/mysql_store_result \n---</reverse valuate strategy error>---"<<endl;
	      exit(1);
	    }
	    while(row=mysql_fetch_row(res)){
	      string tmp_no=row[0],tmp_word=row[1];
	      wordno_weight[tmp_no]=word_weight[tmp_word];
	      if(select_wordno_weight.size()+tail.size()+tmp_no.size()+9>db_op->get_MAX_INSERT()){
		select_wordno_weight_vec.push_back(tail);
		tail="";
	      }
	      tail+="or no='"+tmp_no+"' ";
	    }
	    mysql_free_result(res);
	    res=nullptr;
	  }
	  if(tail.size()>9)select_wordno_weight_vec.push_back(tail);
	  tail="";
	}
	auto update_weight=[](string &url_weight_str,string &word_)->void{
			     //first find old url node check it whether changed,and remove it
			     unsigned int cur_weight=wordno_weight[word_];
			     if(cur_weight==0)
			       return;
			     long i=0;
			     string cur_str_weight="";
			     stringstream ss;
			     ss<<cur_weight;
			     ss>>cur_str_weight;
			     ss.clear();
			     string url_node="|"+cur_str_weight+"_"+cur_str_url_no;
			     long old_url_node=0;
			     while(i<url_weight_str.size()&&url_weight_str[i]!='|')++i;
			     if(i>=url_weight_str.size())return;
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
	for(auto const&e:select_wordno_weight_vec){
	  if(maria_real_query(mysql,select_wordno_weight+e)||!(res=mysql_store_result(mysql))){
	    cerr<<"---<reverse valuate strategy error>---\nmaria_real_query/mysql_store_result \n---</reverse valuate strategy error>---"<<endl;
	    exit(1);
	  }
	  while(row=mysql_fetch_row(res)){
	    string tmp_weight=row[1],tmp_no=row[0];
	    update_weight(tmp_weight,tmp_no);
	    if(when_then.size()+37+no_in.size()+2*tmp_no.size()+tmp_weight.size()>db_op->get_MAX_INSERT()&&tmp_no.size()>0){
	      string tmp_str=when_then+" end where no in ("+no_in.substr(0,no_in.size()-1)+")";
	      update_wordno_weight_vec.push_back(tmp_str);
	      when_then=no_in="";
	    }
	    when_then+=("when '"+tmp_no+"' then '"+tmp_weight+"' ");
	    no_in+=("'"+tmp_no+"',");
	  }
	  mysql_free_result(res);
	  res=nullptr;
	}
	if(no_in.size()>3){
	  string tmp_str=when_then+" end where no in ("+no_in.substr(0,no_in.size()-1)+")";
	  update_wordno_weight_vec.push_back(tmp_str);
	}
      }
      for(auto const&e:update_wordno_weight_vec){
	if(maria_real_query(mysql,update_wordno_weight+e)){
	  cerr<<"---<reverse valuate strategy error>---\nmaria_real_query/mysql_store_result \n---</reverse valuate strategy error>---"<<endl;
	  exit(1);	     
	}
      }
      db_op->recycle_db((void*)mysql);
    }
    ~ReverseValuateStrategy(){
      delete jieba;
    }
  };
}

#endif
