1.
main.cpp�ǲ��Թ����õģ�
safejson.cpp����Ҫ�ļ�
safejson.h��ͷ�ļ�

2.

json�ļ���������jsoncpp��ͷ�ļ������õ�����0.5�İ汾������汾��ͬ����ʹ�ö�Ӧ��ͷ�ļ���
ʹ�õ�ʱ��Ҫlibjson.a�ǵ�Ҫ���ӽ�ȥ


3.
Ϊ����main.cpp������safejson.h�п��˲��Ժ�_UNITTEST_��
����㲻��Ҫ������ע��������Ҳû��Ӱ��


4.
ʹ�÷���
#include "SafeJson.h"
SafeJson sjson(request);
If(sjson[��a��][��b��][��c��].test(Json::stringValue))
{
 Std::string user = request[��a��][��b��][��c��].asString();
}
else
{
std::cout << sjson.getException().code << std::endl;
std::cout << sjson.getException().info << std::endl;
}
�������ܱ�֤���ر����ԡ����ҹ�����ȫ�ܹ��������Ҫ��

