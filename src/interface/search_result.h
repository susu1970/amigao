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

#ifndef AMIGAO_INTERFACE_SEARCH_RESULT_H_
#define AMIGAO_INTERFACE_SEARCH_RESULT_H_

#include<string>

namespace amigao{
  class SearchResultInterface{

  public:
    virtual std::string get_url()=0;
    virtual std::string get_title()=0;
    virtual std::string get_contents()=0;
    virtual std::string get_html()=0;
    virtual std::string pop_url()=0;
    virtual std::string pop_html()=0;
    virtual std::string pop_contents()=0;
    virtual ~SearchResultInterface(){}
  };
}
#endif
