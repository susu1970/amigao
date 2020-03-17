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

#ifndef AMIGAO_STRATEGY_REVERSE_SEARCH_STRATEGY_H_
#define AMIGAO_STRATEGY_REVERSE_SEARCH_STRATEGY_H_

#include"strategy/reverse_search_result.hpp"

namespace amigao{
  class ReverseSearchStrategy:public SearchResultInterface{
  public:
    ReverseSearchStrategy(){
      search_result=new ReverseSearchResult();
    }
    virtual void handle(std::string sentence,DBOperationInterface*db_op)override{
      using namespace std;
      /*
      MYSQL*mysql=(MYSQL*)db_pool->get_connection();
      MYSQL_RES*res;
      MYSQL_ROW row;
      cppjieba::Jieba jieba(DICT_PATH,
			    HMM_PATH,
			    USER_DICT_PATH,
			    IDF_PATH,
			    STOP_WORD_PATH);
      vector<string>word;
      jieba.CutForSearch(crypt(sentence),word,true);
      string find_search_word="select no,word from dictionary.word where 0>1 ";
      string find_stop_word="select word from dictionary.stop_word where 0>1 ";
      string tail="";
      map<string,int>weight_,weight;
      map<string,unsigned long>urlno_weight;//url no : url weight
      map<string,string>wordno_word;//no : word
      for(auto const&e:word)
	++weight_[e];
      vector<string>tail_vec;
      for(auto iter=weight_.begin();iter!=weight_.end();++iter){
	string tmp_str=crypt(iter->first);
	if(tmp_str.size()+tail.size()+11+find_stop_word.size()>MAX_INSERT){
	  tail_vec.push_back(tail);
	  tail="";
	}
	tail+=("or word='"+tmp_str+"' ");
      }
      if(tail.size()>11){
	tail_vec.push_back(tail);
      }
      for(auto tail_:tail_vec){
	if(maria_real_query(mysql,find_search_word+tail)||
	   !(res=mysql_store_result(mysql))){
	  db_pool->recycle_connection((void*)mysql);
	  return ret_queue;
	}
	while(row=mysql_fetch_row(res)){
	  string tmp_str=decrypt(row[1]);
	  weight[tmp_str]=weight_[tmp_str];
	  wordno_word[row[0]]=tmp_str;
	}
      }
      for(auto tail_:tail_vec){
	if(maria_real_query(mysql,find_stop_word+tail)||
	   !(res=mysql_store_result(mysql))){
	  db_pool->recycle_connection((void*)mysql);
	  return ret_queue;
	}
	while(row=mysql_fetch_row(res)){
	  string tmp_str=decrypt(row[0]);
	  weight.erase(tmp_str);
	}	
      }
      string str="select no,weight from reverse where 0>1 ";
      vector<string>str_vec1;
      tail="";
      for(auto iter=wordno_word.begin();iter!=wordno_word.end();++iter){
	if(weight.find(iter->second)==weight.end())continue;
	if(tail.size()+14+iter->first.size()>MAX_INSERT){
	  str_vec1.push_back(tail);
	  tail="";
	}
	tail+=("or no like '"+iter->first+"' ");
      }
      map<string,string>item;//word : weight
      for(auto str_:str_vec1){
	if(maria_real_query(mysql,str)||
	   !(res=maria_store_result(mysql))){
	  db_pool->recycle_connection((void*)mysql);
	  return ret_queue;
	}
	while(row=mysql_fetch_row(res)){
	  item[wordno_word[row[0]]]=row[1];
	}
      }
      for(auto it=item.begin();it!=item.end();++it){
	string uw=it->second;
	long pos=0;
	stringstream ss;
	while(pos<uw.size()&&uw[pos]!='|')++pos;
	string tmp_weight;
	long pos1=++pos;
	for(;pos<uw.size();++pos){
	  if(uw[pos]=='_'){
	    tmp_weight=uw.substr(pos1,pos-pos1);
	    pos1=pos+1;
	  }else if(uw[pos]=='|'){
	    ss<<tmp_weight;
	    unsigned long t1=0;
	    ss>>t1;
	    t1*=weight[it->first];
	    ss.clear();
	    urlno_weight[uw.substr(pos1,pos-pos1)]+=t1;
	    pos1=pos+1;
	  }
	}
	ss<<tmp_weight;
	unsigned long t1=0;
	ss>>t1;
	t1*=weight[it->first];
	ss.clear();
	urlno_weight[uw.substr(pos1,pos-pos1)]+=t1;
      }
      //result urls : address,title,contents,html
      string s1="select no,address,title,contents,html from amigao.visited_url where 0>1 ";
      tail="";
      vector<string>s1_vec;
      for(auto it=urlno_weight.begin();it!=urlno_weight.end();++it){
	if(tail.size()+it->first.size()+s1.size()+14>MAX_INSERT){
	  s1_vec.push_back(tail);
	  tail="";
	}
	tail+=("or no like '"+it->first+"' ");
      }
      for(auto s1_:s1_vec){
	if(maria_real_query(mysql,s1)||
	   !(res=maria_store_result(mysql))){
	  mysql_free_result(res);
	  db_pool->recycle_connection((void*)mysql);
	  return ret_queue;
	}
	while(row=mysql_fetch_row(res)){
	  string strs[4]={
			  {""},
			  {""},
			  {""},
			  {""}
	  };
	  if(row[1])
	    strs[0]=decrypt(row[1]);
	  if(row[2])
	    strs[1]=decrypt(row[2]);
	  if(row[3])
	    strs[2]=decrypt(row[3]);
	  if(row[4])
	    strs[3]=decrypt(row[4]);
	  ReverseSearchResult*node=new ReverseSearchResult((urlno_weight[row[0]]),strs[0],strs[1],strs[2],strs[3]);
	  ret_queue.push(node);
	}
      }
      mysql_free_result(res);
      db_pool->recycle_connection((void*)mysql);
      return ret_queue;
      */
    }
  };
}
#endif
