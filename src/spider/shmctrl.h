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

#ifndef AMIGAO_SPIDER_SHMCTRL_H_
#define AMIGAO_SPIDER_SHMCTRL_H_

#include"values.h"

#include<pthread.h>

/*
the queue size should more than 2
 */
struct shmctrl{
  pthread_mutex_t url_lock,html_lock;
  char url_queue[URL_NUMS][URL_SIZE];
  char html_queue[HTML_NUMS][HTML_SIZE];
  int url_head,url_tail,html_head,html_tail;
};
/*
queue empty: tail_sub == head_sub
queue full: (tail_sub+1)%NUMS == (head_sub)
queue used size: (tail_sub-head_sub+NUMS)%NUMS
queue unused size: (head_sub-tail_sub+NUMS)%NUMS

insert into queue: 1.judge whether has enough size (m)
                   2.if not,not insert
		   3.else,insert:
		     tail_sub = (tail_sub+m)%NUMS

delete from queue: 1.judge the used size
                   2.delete n
		   3.head_sub = (head_sub+n)%NUMS
 */

#endif
