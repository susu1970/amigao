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

#ifndef AMIGAO_STRATEGY_PLAIN_SEARCH_STRATEGY_H_
#define AMIGAO_STRATEGY_PLAIN_SEARCH_STRATEGY_H_

#include<sstream>
#include<unordered_map>

#include"values.h"
#include"interface/search_strategy.h"
#include"utils/cppjieba/Jieba.hpp"
#include"strategy/plain_search_result.hpp"
#include"db/sql_db_operation.hpp"

extern std::string crypt(const std::string&);
extern std::string decrypt(const std::string&);

namespace amigao{
  class PlainSearchStrategy:public SearchStrategyInterface{
    cppjieba::Jieba*jieba;
  public:
    explicit PlainSearchStrategy(cppjieba::Jieba*jieba_):jieba(jieba_){}
    virtual void handle(std::string &sentence,void*search_result_,DBOperationInterface*db_op,short &start_page,short &results_per_page,bool use_stop_word=false){
      using namespace std;
      if(sentence.empty()||!db_op){
	start_page=results_per_page=-1;
	return;	
      }
      sentence=sentence.substr(0,MAX_SEARCH_SENTENCE_SIZE);
      if(start_page<1)start_page=1;
      if(results_per_page<1||results_per_page>MAX_RESULTS_PER_PAGE)results_per_page=DEFAULT_RESULTS_PER_PAGE;
      string tail="";
      MYSQL*mysql=(MYSQL*)db_op->get_db();
      MYSQL_RES*res=nullptr;
      MYSQL_ROW row;
      unordered_map<string,unsigned long>urlno_weight;
      {
	unordered_map<string,short>wordno_times;
	unordered_map<string,string>wordno_weight;
	{
	  const string find_wordno_weight="select no,weight from amigao.reverse where 0>1 ";
	  const string find_word="select word,no from dictionary.word where 0>1 ";
	  const string find_stop_word="select word from dictionary.stop_word where 0>1 ";
	  vector<string>wordno_weight_vec;
	  unordered_map<string,short>tmp_times;
	  vector<string>word_vec,find_vec;
	  jieba->CutForSearch(sentence,word_vec,true);
	  for(auto const&e:word_vec){
	    if(e.empty()||e.size()>WORD_SIZE)continue;
	    string tmp_str=crypt(e);
	    if(tail.size()+find_stop_word.size()+tmp_str.size()+11>db_op->get_MAX_INSERT()){
	      find_vec.push_back(tail);
	      tail="";
	    }
	    tail+="or word='"+tmp_str+"' ";
	    ++tmp_times[tmp_str];
	  }
	  if(tail.size()>11)find_vec.push_back(tail);
	  tail="";
	  if(use_stop_word){
	    for(auto const&e:find_vec){
	      if(maria_real_query(mysql,find_stop_word+e)||
		 !(res=mysql_store_result(mysql))){
		cerr<<"---<plain_search_strategy find_stop_word error>---\nmaria_real_query/mysql_store_result\n---</plain_search_strategy find_stop_word error>---"<<endl;
		exit(1);
	      }
	      while(row=mysql_fetch_row(res))
		tmp_times.erase(row[0]);
	      mysql_free_result(res);
	      res=nullptr;
	    }
	  }
	  for(auto const&e:find_vec){
	    if(maria_real_query(mysql,find_word+e)||
	       !(res=mysql_store_result(mysql))){
	      cerr<<"---<plain_search_strategy error>---\nfind_word error\nmaria_real_query/mysql_store_result\n---</plain_search_strategy error>---"<<endl;
	      exit(1);
	    }
	    while(row=mysql_fetch_row(res)){
	      string tmp_word=row[0],tmp_no=row[1];
	      wordno_times[tmp_no]=tmp_times[tmp_word];
	      if(tail.size()+tmp_no.size()+9+find_wordno_weight.size()>db_op->get_MAX_INSERT()){
		wordno_weight_vec.push_back(tail);
		tail="";
	      }
	      tail+="or no='"+tmp_no+"' ";
	    }
	    mysql_free_result(res);
	    res=nullptr;
	  }
	  if(tail.size()>9)wordno_weight_vec.push_back(tail);
	  tail="";
	  for(auto const&e:wordno_weight_vec){
	    if(maria_real_query(mysql,find_wordno_weight+e)||
	       !(res=mysql_store_result(mysql))){
	      cerr<<"---<plain_search_strategy find_wordno_weight error/>---"<<endl;
	      exit(1);
	    }
	    while(row=mysql_fetch_row(res)){
	      wordno_weight[row[0]]=row[1];
	    }
	    mysql_free_result(res);
	    res=nullptr;
	  }
	}//wordno_times,wordno_weight
	auto parse_weight=[](unordered_map<string,unsigned long>&urlno_weight,
			     const string&weight_str,short times){
			    stringstream ss;
			    size_t i=0,j;
			    while(weight_str[i]!='|')++i;
			    while(i<weight_str.size()){
			      if((j=++i)>=weight_str.size())return;
			      while(i<weight_str.size()&&weight_str[i]!='_')
				++i;
			      string tmp_weight=weight_str.substr(j,i-j);
			      ss<<tmp_weight;
			      unsigned int tmp_int;
			      ss>>tmp_int;
			      ss.clear();
			      tmp_int*=times;
			      if((j=++i)==weight_str.size())return;
			      while(i<weight_str.size()&&weight_str[i]!='|')
				++i;
			      string tmp_urlno=weight_str.substr(j,i-j);
			      urlno_weight[tmp_urlno]+=tmp_int;
			    }
			  };
	//wordno_times,wordno_weight
	for(auto iter=wordno_weight.begin();iter!=wordno_weight.end();++iter)
	  parse_weight(urlno_weight,iter->second,wordno_times[iter->first]);
      }
      //urlno_vec,urlno_weight;
      if(urlno_weight.empty()){
	start_page=results_per_page=-1;
	return;
      }
      int end=urlno_weight.size()-1;
      int start_sub=(start_page-1)*results_per_page;
      if(start_sub>end){
	start_page=results_per_page=-1;
	return;	
      }
      int end_sub=start_sub+results_per_page-1;
      if(end_sub>end)end_sub=end;
      int total_pages=(end+1)/results_per_page;
      if((end+1)%results_per_page)++total_pages;
      results_per_page=total_pages;
      string urlno_arr[end+1];
      {
	size_t i=0;
	for(auto iter=urlno_weight.begin();iter!=urlno_weight.end();++iter,++i)
	  urlno_arr[i]=iter->first;
      }
      {
	//create heap.
	int p=end/2;
	while(p>=0){
	  int next=p,q=p;
	  while(q<=end){
	    int left=q*2+1,right=q*2+2;
	    if(left<=end&&(urlno_weight[urlno_arr[left]])>(urlno_weight[urlno_arr[next]]))next=left;
	    if(right<=end&&(urlno_weight[urlno_arr[right]])>(urlno_weight[urlno_arr[next]]))next=right;
	    if(q==next)break;
	    string tmp_str=urlno_arr[q];
	    urlno_arr[q]=urlno_arr[next];
	    urlno_arr[next]=tmp_str;
	    q=next;
	  }
	  --p;
	}
	//heap sort (only partial elements need to be sorted)
	auto heap_sort1=[&](int tail){
			  if(tail==0)return;
			  string tmp_str=urlno_arr[tail];
			  urlno_arr[tail]=urlno_arr[0];
			  urlno_arr[0]=tmp_str;
			  int p=0,next=0;
			  while(p<tail){
			    int left=p*2+1,right=p*2+2;
			    if(left<tail&&urlno_weight[urlno_arr[left]]>urlno_weight[urlno_arr[next]])next=left;
			    if(right<tail&&urlno_weight[urlno_arr[right]]>urlno_weight[urlno_arr[next]])next=right;
			    if(next==p)break;
			    tmp_str=urlno_arr[p];
			    urlno_arr[p]=urlno_arr[next];
			    urlno_arr[next]=tmp_str;
			    p=next;
			  }
			};
	for(int i=0;i<=end_sub;++i)
	  heap_sort1(end-i);
      }
      const string get_url="select url,title,contents,last_update_time from amigao.visited_url where 0>1 ";
      string field_no="";
      vector<string>get_url_vec;
      for(int i=end-start_sub,j=0;j<=end_sub;++j,--i){
	if(urlno_arr[i]=="0")continue;
	cout<<"urlno_arr[i]: "<<urlno_arr[i]<<endl;
	if(tail.size()+2*urlno_arr[i].size()+31>db_op->get_MAX_INSERT()){
	  string tmp_str=tail+"order by field(no"+field_no+") ";
	  get_url_vec.push_back(tmp_str);
	  tail="";field_no="";
	}
	tail+="or no='"+urlno_arr[i]+"' ";
	field_no+=",'"+urlno_arr[i]+"'";
      }
      if(tail.size()>9){
	string tmp_str=tail+"order by field(no"+field_no+") ";
	get_url_vec.push_back(tmp_str);
      }
      tail="";field_no="";
      cout<<"ok"<<endl;
      PlainSearchResult*search_result=(PlainSearchResult*)search_result_;
      for(auto const&e:get_url_vec){
	if(maria_real_query(mysql,get_url+e)||!(res=mysql_store_result(mysql))){
	  cerr<<"---<plain_search_strategy error>---\nmaria_real_query/mysql_store_result\n---</plain_search_strategy error>---"<<endl;
	  exit(1);
	}
	while(row=mysql_fetch_row(res)){
	  string url=decrypt(row[0]);
	  string title=decrypt(row[1]).substr(0,MAX_CONTENTS_SIZE);
	  string contents=decrypt(row[2]).substr(0,MAX_CONTENTS_SIZE);
	  string html="";
	  string time=row[3];
	  search_result->add_node(url,title,contents,html,time);
	}
	mysql_free_result(res);
	res=nullptr;	
      }
      db_op->recycle_db((void*)mysql);
    }
  };
}

#endif
