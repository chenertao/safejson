/*********************************************************
	-- ������	18948
	jsoncpp��������ֵ���Ͳ���Ԥ�ڣ����³���ҵ�
	Ϊ�˰�ȫ�Ķ�д�ֶ�ֵ��ʵ��safejson��

**********************************************************/
#ifndef SAFE_JSON_INCLUDE_H
#define SAFE_JSON_INCLUDE_H

#define _UNITTEST_
#include "json/json.h"
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <cassert>

#define BITMSK(bit)				(int)(1 << (bit))

#define FLAG_REWRITE BITMSK(1)		//[]����ʱ�����Ͳ�ͬ�½��ն����滻ԭ����
//#define FLAG_ONLYONCE BITMSK(2)	//���������
#define FLAG_STREQNUM BITMSK(3)		//string��number֮��ʵ���Զ���ת,�����־��������ԭ��test()Ϊfalse�ı��true��Ҳ����test()��ʱ��number��string��Զ����Ϊ���ܻ�ת�ġ���Ȼ������ʹ���ϳ���
#define FLAG_STRICTMEMBER BITMSK(4)		//�ϸ���["key"]�Ƿ����,[index]�Ƿ񳬹���С����Ȼ�����߲���������jsoncpp��assert

typedef struct JSONEXCEPTION
{
	int code;
	std::string info;	
}JsonException,*pJsonException;

enum JSON_STATE
{
	JSON_RIGHT,				
	JSON_WRONG_GETOBJECT,	//cannot ["key"]
	JSON_WRONG_GETARRAY,	//cannot [3]
	JSON_WRONG_ASSOMETYPE,	//cannot asXX()
	JSON_WRONG_NOMEMBER,	//object's key not exist,or array's index exceed size
};

class SafeJson
{
private:
	//������Ĭ�Ϲ���
	SafeJson(){}
public:
	//����
	~SafeJson();
	/*
		���캯�������봫��jsonֵ��
		param��ѡ:
			FLAG_REWRITE��ʾ�򿪸���ģʽ�������Ѿ����ڵ��ֶΣ���obj["key"]=1��������ȡ�±�(��obj["key"]["subkey"]=5)���ؽ�����
				����ԭ�ֶ�����(����ֵ1�����obj["key"]["subkey"]=5)�������Ӵ˲������򲻻Ḳ�ǣ���ֵ������Ч��Ĭ�ϲ��򿪸ò�����
			FLAG_STREQNUM
				�ַ�������ֵ֮���Զ���ת����"123"��123�ܻ������asXX����������Զ�ת��
			FLAG_STRICTMEMBER
				�ϸ���["key"]�Ƿ����,[index]�Ƿ񳬹���С����Ȼ�����߲���������jsoncpp��assert
	*/
	SafeJson(Json::Value& json,int param=0);
	
	/*
		ȡ��json�����صĻ���SafeJson����.�����ڲ�ָ���Ѿ�ָ������json��
		��FLAG_REWRITEģʽ����json��boject���ͣ����ô���״̬��
		FLAG_REWRITEģʽ��json��boject���ͣ����ؽ��Ӷ���		
	*/
	SafeJson& operator[]( const char *key );
	
	//ͬoperator[]( const char *key );
	SafeJson& operator[]( const std::string &key );
	
	//ȡ��json����json��Array���ͣ��߼�ͬoperator[]( const char *key );
	SafeJson& operator[](unsigned int index);
	
	//��ֵ
	SafeJson& operator=( const Json::Value& other );
	
	//ͬjsoncpp�Ľӿ�
	std::string asString();	
	const char* asCString();//���ܷ���NULL
	bool asBool();
	double asDouble();
	int asInt();
	unsigned int asUInt();	

public:
	//�������ṩһЩ����Ĺ��ܺ���������Ҫ��ʱ��ʹ��
	
	/*
	*  �Ƿ�����Ȼת��Ŀ�����ͣ�
	*  ����false������ĳ��·���ж�ĳ����objectȡ��["key"]
	*  ���߶Է�arrayȡ��[5]��
	*  ����asXX()ʱjsontype�޷�ת��	
	*  jsontype Ϊֵ��Ŀ������
	*  ȡJson::nullValue��Ĭ��ֵ��ʱ����ʾֻ���[xx]·���Ŀɴ��ԣ��������ת����ĳ������
	*/	
	bool test(Json::ValueType jsontype = Json::nullValue);
	
	/*
	* ��ѯ��ǰ��״̬����ȷ�����쳣
	* ����ֵ��
	* code:ȡ��JSON_STATE
	* info:�Ǵ�����Ϣ����ʽΪ"[ԭ����]->[�������Ĳ���]"
	* ԭ����Ϊ:1.[Object]  2.[Array]  3.[String] 4.[Boolean:true/false]  5.[Number:123/1.23]  6.[Null]
	* �������Ĳ�����1.["key"]  2.[12]  3.[asString]  4.[asInt]  ...(�����getasXXTag())  
	*/
	const JsonException& getException();
	
private:

	//��ֵ������˽�д������õ����÷�
	SafeJson& operator=( const SafeJson& other );

	/**********************************************************
	*���ܣ��ܷ��Զ�ת����Ŀ�����͡�
	*��Ϊjsoncpp�Դ���isConvertibleTo������:
	*int���͵�isConvertibleTo(string)����true
	*��int����toString()ȴ��ʧ�ܵ�,
	*���Բ�ʹ��isConvertibleTo�ж��Ƿ��ת�����Լ�ʵ��һ����
	json�����ͣ�
	namespace Json {
	   enum ValueType
	   {
		  nullValue = 0, ///< 'null' value
		  intValue,      ///< signed integer value
		  uintValue,     ///< unsigned integer value
		  realValue,     ///< double value
		  stringValue,   ///< UTF-8 string value
		  booleanValue,  ///< bool value
		  arrayValue,    ///< array value (ordered list)
		  objectValue    ///< object value (collection of name/value pairs).
	   };
	}
	Json::Value& json ʵʩת����Ŀ��json����
	Json::ValueType jsontype ת����Ŀ������
	***********************************************************/
	bool canNatureConvert(Json::Value& json,Json::ValueType jsontype);
	
	//��Ȼ��ȫ��ת�����
	bool isRealSafe(Json::Value& json,Json::ValueType jsontype);
	
	//�����ڲ�״̬
	void reset();
	
	
	//���漸����������Ϊ��֧���쳣��Ϣ�ļ�¼�ͻ�ȡ
	/*
	*��ȡ��ǰֵ�����ݸ�Ҫ��
	*	[Array]		��ӦisArray()
	*	[Object]	��ӦisObject()
	*	[String]	��ӦisString()
	*	[Number:12345]	��ӦisInt() || isUInt() || isDouble()
	*	[Boolean:false]	��ӦisBool()
	*	[Null]
	*/
	std::string getCurValueTag();
	
	//���һ��string�����һ��\n����֮�������
	void trimRightLn(std::string& str);
	
	/*
	*��ö������ת���ַ���asXX�����ڱ�ʾִ��ĳ������
		enum ValueType
	   {
		  nullValue = 0,  [asNull]
		  intValue,       [asInt]
		  uintValue,      [asUInt]
		  realValue,      [asDouble]
		  stringValue,    [asString]
		  booleanValue,   [asBool]
		  arrayValue,     [asArray]
		  objectValue     [asObject]
	   };
	*/
	std::string getasXXTag(Json::ValueType jsontype);
	
	//����״̬��exception��Ϣ
	void setState(JSON_STATE state,const std::string& actionstr);
	
private:
	//ָ���ʼ��ʱ�Ķ�������resetʱʹ��
	Json::Value	*m_pjson;
	
	//ָ��ǰjson�������json���Ӷ������в����������ָ��Ķ�����Ҫ
	Json::Value *m_pjsonLast;
	
	//�ڲ�״̬��ָʾ�Ƿ����
	JSON_STATE m_state;
	
	//���ܲ�����ͬ���캯���е�param
	int m_param;
	
	//������Ϣ
	JsonException m_exception;
};

//����������������ֻ��һ�д���
#define ONCEJSON(json) (SafeJson(json))

//�Զ�ʵ����ֵ���ַ���ת����json
#define AUTOONCE(json) (SafeJson(json,FLAG_STREQNUM))

//�ϸ����Ա
#define STRICTONCE(json) (SafeJson(json,FLAG_STRICTMEMBER))

//�ؽ�json
#define REWTONCE(json) (SafeJson(json,FLAG_REWRITE))


#endif

