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

#ifndef AMIGAO_STRATEGY_PLAIN_SEARCH_RESULT_H_
#define AMIGAO_STRATEGY_PLAIN_SEARCH_RESULT_H_

#include<vector>

#include"interface/search_result.h"

namespace amigao{
  class PlainSearchResult:public SearchResultInterface{
    class PlainSearchResultNode{
      friend PlainSearchResult;
      std::string url="",title="",contents="",html="";
      std::string last_time="";
      PlainSearchResultNode(const std::string &url_,
			    const std::string &title_,
			    const std::string &contents_,
			    const std::string &html_,
			    const std::string &last_time_)
	:url(url_),title(title_),contents(contents_),html(html_),last_time(last_time_){}
    };
    std::vector<PlainSearchResultNode>search_result_vec;
  public:
    virtual std::string get_url(size_t ith=0){
      if(ith<size())
	return search_result_vec[ith].url;
      return "";
    }
    virtual std::string get_time(size_t ith=0){
      if(ith<size())
	return search_result_vec[ith].last_time;
      return "";
    }
    virtual std::string get_title(size_t ith=0){
      if(ith<size())
	return search_result_vec[ith].title;
      return "";      
    }
    virtual std::string get_contents(size_t ith=0){
      if(ith<size())
	return search_result_vec[ith].contents;
      return "";      
    }
    virtual std::string get_html(size_t ith=0){
      if(ith<size())
	return search_result_vec[ith].html;
      return "";
    }
    void add_node(std::string &url,
		  std::string &title,
		  std::string &contents,
		  std::string &html,
		  std::string &time_){
      search_result_vec.push_back(PlainSearchResultNode(url,title,contents,html,time_));
    }
    virtual size_t size(){
      return search_result_vec.size();
    }
    bool empty(){
      return search_result_vec.empty();
    }
    ~PlainSearchResult(){}
  };
}

#endif
