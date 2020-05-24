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

#include"values.h"
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
      if(url.size()>URL_SIZE||!db_op)return;
      db_op->op_visited_url(url,title,contents,html);
      long cur_url_no;
      if((cur_url_no=db_op->get_url_no(url))==0)
	return;
      string cur_str_url_no;
      stringstream ss;
      ss<<cur_url_no;
      ss>>cur_str_url_no;
      ss.clear();
      MYSQL*mysql=(MYSQL*)db_op->get_db();
      MYSQL_ROW row;
      MYSQL_RES*res=nullptr;
      const string update_amigaoreverse_weight="update reverse set weight= case no ";
      vector<string>update_amigaoreverse_weight_vec;
      {
	unordered_map<string,unsigned int>wordno_weight;
	unordered_map<string,string>amigaoreverse_no_weight;
	string tail="",when_then="",no_in="";
	const string select_amigaoreverse_no_weight="select no,weight from amigao.reverse where 0>1 ";
	vector<string>select_amigaoreverse_no_weight_vec;
	{
	  unordered_map<string,unsigned int>word_weight;
	  const string select_dictionaryword_no_word="select no,word from dictionary.word where 0>1 ";
	  vector<string>select_dictionaryword_no_word_vec;
	  {
	    vector<string>title_vec;
	    jieba->CutForSearch(title,title_vec,true);
	    for(auto const&e:title_vec){
	      string tmp_str="";
	      for(size_t i=0;i<e.size();++i)
		if(e[i]!=' '&&e[i]!='\r'&&e[i]!='\n'&&e[i]!='\t')
		  tmp_str+=e[i];
	      tmp_str=crypt(tmp_str);
	      if(tmp_str.empty()||tmp_str.size()>DB_DICTIONARY_WORD_SIZE)continue;
	      word_weight[tmp_str]+=10;
	      if(select_dictionaryword_no_word.size()+tail.size()+tmp_str.size()+11>db_op->get_MAX_INSERT()){
		select_dictionaryword_no_word_vec.push_back(tail);
		tail="";
	      }
	      tail+="or word='"+tmp_str+"' ";
	    }
	    if(tail.size()>11)select_dictionaryword_no_word_vec.push_back(tail);
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
	      if(tmp_str.empty()||tmp_str.size()>DB_DICTIONARY_WORD_SIZE)continue;
	      ++word_weight[tmp_str];
	      if(select_dictionaryword_no_word.size()+tail.size()+tmp_str.size()+11>db_op->get_MAX_INSERT()){
		select_dictionaryword_no_word_vec.push_back(tail);
		tail="";
	      }
	      tail+="or word='"+tmp_str+"' ";
	    }
	    if(tail.size()>11)select_dictionaryword_no_word_vec.push_back(tail);
	    tail="";
	  }
	  for(auto const&e:select_dictionaryword_no_word_vec){
	    if(maria_real_query(mysql,select_dictionaryword_no_word+e)||
	       !(res=mysql_store_result(mysql))){
	      cerr<<"---<reverse valuate strategy error>---\nmaria_real_query/mysql_store_result \n---</reverse valuate strategy error>---"<<endl;
	      exit(1);
	    }
	    while(row=mysql_fetch_row(res)){
	      string tmp_no=row[0],tmp_word=row[1];
	      wordno_weight[tmp_no]=word_weight[tmp_word];
	      if(select_amigaoreverse_no_weight.size()+tail.size()+tmp_no.size()+9>db_op->get_MAX_INSERT()){
		select_amigaoreverse_no_weight_vec.push_back(tail);
		tail="";
	      }
	      tail+="or no='"+tmp_no+"' ";
	    }
	    mysql_free_result(res);
	    res=nullptr;
	  }
	  if(tail.size()>9)select_amigaoreverse_no_weight_vec.push_back(tail);
	  tail="";
	}
	//wordno_weight: reverse(no)-> weight(unsigned int)
	//amigaoreverse_no_weight: reverse(no),weight(string,string)
	auto update_weight=[&](string&amigaoreverse_weight,const string &amigaoreverse_no)->void{
			     unsigned int cur_weight=wordno_weight[amigaoreverse_no];
			     if(cur_weight==0)return;
			     int cur_nums=1;//当前weight列中总节点数
			     string cur_weight_str="";//当前no行中待更新的weight值
			     ss<<cur_weight;
			     ss>>cur_weight_str;
			     ss.clear();
			     if(cur_weight_str.empty()||!(cur_weight_str[0]>'0'&&cur_weight_str[0]<='9'))
			       return;
			     int first_node_sub=0;//第一个节点下标
			     const string insert_node="|"+cur_weight_str+"_"+cur_str_url_no;
			     {
			       //获取当前no行中原来的总节点数
			       while(amigaoreverse_weight[first_node_sub]!='|')++first_node_sub;
			       string cur_nums_str=amigaoreverse_weight.substr(0,first_node_sub);
			       ss<<cur_nums_str;
			       ss>>cur_nums;
			       ss.clear();
			     }
			     //试图查找cur_str_url_no是否存在于weight列中
			     {
			       string find_url_no="_"+cur_str_url_no+"|";
			       const int pos=amigaoreverse_weight.find(find_url_no);
			       if(pos<0){
				 int i=amigaoreverse_weight.size()-1;
				 while(amigaoreverse_weight[i]!='_')--i;
				 if(amigaoreverse_weight.substr(i+1,amigaoreverse_weight.size())==cur_str_url_no){
				   if(cur_nums==1){
				     amigaoreverse_weight=("1"+insert_node);
				     return;
				   }//仅有一个节点且相同
				   //有多个节点，先删除最后那个
				   --cur_nums;
				   string cur_nums_str="";
				   ss<<cur_nums;
				   ss>>cur_nums_str;
				   ss.clear();
				   while(amigaoreverse_weight[i]!='|')--i;
				   amigaoreverse_weight=cur_nums_str+amigaoreverse_weight.substr(first_node_sub,i-first_node_sub);
				 }
			       }else{
				 //当前url_no已经在weight中且不在尾部
				 int i=first_node_sub;
				 while(amigaoreverse_weight[i]!='_')++i;
				 if(i==pos){
				   //当前节点是头节点
				   --cur_nums;
				   string cur_nums_str="";
				   ss<<cur_nums;
				   ss>>cur_nums_str;
				   ss.clear();
				   while(amigaoreverse_weight[i]!='|')++i;
				   amigaoreverse_weight=(cur_nums_str+amigaoreverse_weight.substr(i,amigaoreverse_weight.size()));
				 }else{
				   //当前节点是中间节点
				   --cur_nums;
				   string cur_nums_str="";
				   ss<<cur_nums;
				   ss>>cur_nums_str;
				   ss.clear();
				   i=pos;
				   while(amigaoreverse_weight[i]!='|')--i;
				   int j=pos;
				   while(amigaoreverse_weight[j]!='|')++j;
				   amigaoreverse_weight=(cur_nums_str+amigaoreverse_weight.substr(first_node_sub,i-first_node_sub)+amigaoreverse_weight.substr(j,amigaoreverse_weight.size()));
				 }
			       }
			     }//到这里weight中可保证不存在cur_url_str_no
			     //重新计算first_node_sub
			     first_node_sub=0;
			     while(amigaoreverse_weight[first_node_sub]!='|')++first_node_sub;
			     {
			       //先直接将节点加到合适位置，之后再处理节点数量问题
			       int prev=first_node_sub+1,p=prev+1,next=p+1;
			       while(amigaoreverse_weight[p]!='_')++p;
			       string node_weight=amigaoreverse_weight.substr(prev,p-prev);
			       while(next<amigaoreverse_weight.size()&&amigaoreverse_weight[next]!='|')++next;
			       unsigned int node_weight_ui;
			       ss<<node_weight;
			       ss>>node_weight_ui;
			       ss.clear();
			       while(node_weight_ui>cur_weight){
				 if(next==amigaoreverse_weight.size()){
				   amigaoreverse_weight=amigaoreverse_weight+insert_node;
				   goto after_insert_node;
				 }
				 prev=++next;
				 p=prev+1;
				 while(amigaoreverse_weight[p]!='_')++p;
				 node_weight=amigaoreverse_weight.substr(prev,p-prev);
				 ss<<node_weight;
				 ss>>node_weight_ui;
				 ss.clear();
				 while(next<amigaoreverse_weight.size()&&amigaoreverse_weight[next]!='|')++next;
			       }
			       amigaoreverse_weight=(amigaoreverse_weight.substr(0,prev-1)+insert_node+amigaoreverse_weight.substr(prev-1,amigaoreverse_weight.size()));
			     }
	after_insert_node:
			     //处理节点数量问题
			     if(cur_nums==MAX_URLS){
			       int i=amigaoreverse_weight.size()-1;
			       while(amigaoreverse_weight[i]!='|')--i;
			       amigaoreverse_weight=amigaoreverse_weight.substr(0,i);
			       return;
			     }
			     {//更新节点数量
			       ++cur_nums;
			       string cur_nums_str="";
			       ss<<cur_nums;
			       ss>>cur_nums_str;
			       ss.clear();
			       int i=0;
			       while(amigaoreverse_weight[i]!='|')++i;
			       amigaoreverse_weight=cur_nums_str+amigaoreverse_weight.substr(i,amigaoreverse_weight.size());
			     }
			   };
	//update_amigaoreverse_weight(vec)
	for(auto const&e:select_amigaoreverse_no_weight_vec){
	  if(maria_real_query(mysql,select_amigaoreverse_no_weight+e)||!(res=mysql_store_result(mysql))){
	    cerr<<"---<reverse valuate strategy error>---\nmaria_real_query/mysql_store_result \n---</reverse valuate strategy error>---"<<endl;
	    exit(1);
	  }
	  while(row=mysql_fetch_row(res)){
	    string tmp_weight=row[1],tmp_no=row[0];
	    update_weight(tmp_weight,tmp_no);
	    if(tmp_no.size()>0&&when_then.size()+37+no_in.size()+2*tmp_no.size()+tmp_weight.size()>db_op->get_MAX_INSERT()){
	      tail=when_then+" end where no in ("+no_in.substr(0,no_in.size()-1)+")";
	      update_amigaoreverse_weight_vec.push_back(tail);
	      tail=when_then=no_in="";
	    }
	    when_then+=("when '"+tmp_no+"' then '"+tmp_weight+"' ");
	    no_in+=("'"+tmp_no+"',");
	  }
	  mysql_free_result(res);
	  res=nullptr;
	}
	if(no_in.size()>3){
	  tail=when_then+" end where no in ("+no_in.substr(0,no_in.size()-1)+")";
	  update_amigaoreverse_weight_vec.push_back(tail);
	}
	tail="";
      }
      for(auto const&e:update_amigaoreverse_weight_vec)
	if(maria_real_query(mysql,update_amigaoreverse_weight+e)){
	  cerr<<"---<reverse valuate strategy error>---\nmaria_real_query/mysql_store_result \n---</reverse valuate strategy error>---"<<endl;
	  exit(1);	     
	}
      db_op->recycle_db((void*)mysql);
    }
    ~ReverseValuateStrategy(){
      delete jieba;
    }
  };
}

#endif
