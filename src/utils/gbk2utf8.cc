//referenced from:
// https://blog.csdn.net/weixin_38595946/article/details/103128955?depth_1-utm_source=distribute.pc_relevant.none-task&utm_source=distribute.pc_relevant.none-task
// Author: 758293230@qq.com
#include <stdlib.h>
#include <string.h>
#include <iconv.h> 
#include <string>

using namespace std;

int gbk2utf8(char* input, size_t charInPutLen, char* output, size_t &charOutPutLen){
  int ret = 0;
  iconv_t cd;
  cd = iconv_open("utf-8", "GBK");
  ret = iconv(cd, &input, &charInPutLen, &output, &charOutPutLen);
  iconv_close(cd);
  return ret;
}
//if not gbk,won't change source string 
int gbk2utf8(const string& input, string& output){
  int ret = 0;
  size_t charInPutLen = input.length();
  //you may notice that the output size will  double growth
  size_t charOutPutLen = 2*charInPutLen+1;
  char *pTemp = new char[charOutPutLen];
  memset(pTemp,0,charOutPutLen);
  iconv_t cd;
  char *pSource =(char *)input.c_str();
  char *pOut = pTemp;
  cd = iconv_open("utf-8", "GBK");
  ret = iconv(cd, &pSource, &charInPutLen, &pTemp, &charOutPutLen);
  iconv_close(cd);
  output= pOut;
  delete []pOut; //注意这里，不能使用delete []pTemp, iconv函数会改变指针pTemp的值
  return ret;
}

