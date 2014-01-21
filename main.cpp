#include "SafeJson.h"
#include <stdio.h>


extern int g_inst_num;


#define Jassert(a,b) if((a) != (b)){printf("%s:%d >> Jassert fail!\n",__FILE__,__LINE__);}

#define JSONSTR "{\"a\":[1,\"2\",true],\"b\":123,\"c\":\"123\"}"
/*
{
   "a" : [ 1, "2", 3 ],
   "b" : 123,
   "c" : "123"
}
*/



//����once��
void testOnce(Json::Value& root)
{
//oncejson����
	g_inst_num = 0;
	Jassert(ONCEJSON(root)["a"]["b"].asString(),"");
	Jassert(ONCEJSON(root)["a"]["b"].asCString(),NULL);
	Jassert(ONCEJSON(root)["a"]["b"].asInt(),0);
	Jassert(ONCEJSON(root)["a"]["b"].asUInt(),0u);
	Jassert(ONCEJSON(root)["a"]["b"].asDouble(),0.0);
	Jassert(g_inst_num,5);

	Jassert(ONCEJSON(root)["b"].asString(),"");
	Jassert(ONCEJSON(root)["b"].asCString(),NULL);
	Jassert(ONCEJSON(root)["b"].asInt(),123);
	Jassert(ONCEJSON(root)["b"].asUInt(),123u);
	Jassert(ONCEJSON(root)["b"].asDouble(),123.0);
	Jassert(g_inst_num,10);
	
	Jassert(ONCEJSON(root)["c"].asString(),"123");
	Jassert(ONCEJSON(root)["c"].asCString(),root["c"].asCString());
	Jassert(ONCEJSON(root)["c"].asInt(),0);
	Jassert(ONCEJSON(root)["c"].asUInt(),0u);
	Jassert(ONCEJSON(root)["c"].asDouble(),0.0);
	Jassert(g_inst_num,15);
	
//AUTOONCE����
	//Ĭ��ֵ����
	Jassert(AUTOONCE(root)["a"]["b"].asString(),"");
	Jassert(AUTOONCE(root)["a"]["b"].asCString(),NULL);
	Jassert(AUTOONCE(root)["a"]["b"].asInt(),0);
	Jassert(AUTOONCE(root)["a"]["b"].asUInt(),0u);
	Jassert(AUTOONCE(root)["a"]["b"].asDouble(),0.0);
	Jassert(g_inst_num,20);
	
	//��ֵת�ַ���
	Jassert(AUTOONCE(root)["b"].asString(),"123");
	Jassert(AUTOONCE(root)["b"].asCString(),NULL);
	Jassert(AUTOONCE(root)["b"].asInt(),123);
	Jassert(AUTOONCE(root)["b"].asUInt(),123u);
	Jassert(AUTOONCE(root)["b"].asDouble(),123.0);
	Jassert(g_inst_num,25);
	
	//�ַ���ת��ֵ
	Jassert(AUTOONCE(root)["c"].asString(),"123");
	Jassert(AUTOONCE(root)["c"].asCString(),root["c"].asCString());
	Jassert(AUTOONCE(root)["c"].asInt(),123);
	Jassert(AUTOONCE(root)["c"].asUInt(),123u);
	Jassert(AUTOONCE(root)["c"].asDouble(),123.0);
	Jassert(g_inst_num,30);

	
}

void testDefault(Json::Value& root)
{
	g_inst_num = 0;
	
	{
	SafeJson sjson(root);
	//������±�Ĭ��ֵ
	Jassert(sjson["a"]["b"].asString(),"");
	Jassert(sjson["a"]["b"].asCString(),NULL);
	Jassert(sjson["a"]["b"].asInt(),0);
	Jassert(sjson["a"]["b"].asUInt(),0u);
	Jassert(sjson["a"]["b"].asDouble(),0.0);
	
	//number->���ʹ���Ĭ��ֵ
	Jassert(sjson["b"].asString(),"");
	Jassert(sjson["b"].asCString(),NULL);
	Jassert(sjson["b"].asInt(),123);
	Jassert(sjson["b"].asUInt(),123u);
	Jassert(sjson["b"].asDouble(),123.0);
	
	//string->���ʹ���Ĭ��ֵ
	Jassert(sjson["c"].asString(),"123");
	Jassert(sjson["c"].asCString(),root["c"].asCString());
	Jassert(sjson["c"].asInt(),0);
	Jassert(sjson["c"].asUInt(),0u);
	Jassert(sjson["c"].asDouble(),0.0);
	
	Jassert(g_inst_num,0);
	}
	Jassert(g_inst_num,1);
	
}

void testrelease(Json::Value& root)
{
	g_inst_num = 0;
	//new�������
	SafeJson *pjson = new SafeJson(root);
	Jassert((*pjson)["b"].asString(),"");
	Jassert((*pjson)["c"].asUInt(),0u);
	delete pjson;
	Jassert(g_inst_num,1);

	
}

//��Ϊ��ı���������Բ��������ô���
void testFLAG_REWRITE(Json::Value root)
{
	g_inst_num = 0;
	{
	//����ԭֵ
	SafeJson sjson(root,FLAG_REWRITE);
	sjson["b"][2] = "arr";
	Jassert(sjson["b"][2].asString(),"arr");	//����ԭֵ
	sjson["b"]["2"]["3"]["4"] = "obj";
	Jassert(sjson["b"]["2"]["3"]["4"].asString(),"obj");	//����ԭֵ
	Jassert(g_inst_num,0);
	}
	Jassert(g_inst_num,1);
}


void testFLAG_ONLYONCE(Json::Value& root)
{
	
	{
	g_inst_num = 0;
	SafeJson sjson=ONCEJSON(root);
	sjson["b"][2] = "arr";
	Jassert(g_inst_num,0);
	}
	Jassert(g_inst_num,1);
	
	{
	g_inst_num = 0;
	SafeJson sjson=ONCEJSON(root);
	sjson["b"].asString();
	Jassert(g_inst_num,0);
	}
	Jassert(g_inst_num,1);
	
	{
	g_inst_num = 0;
	SafeJson sjson=ONCEJSON(root);
	sjson["b"].asInt();
	Jassert(g_inst_num,0);
	}
	Jassert(g_inst_num,1);
	
	{
	g_inst_num = 0;
	SafeJson sjson=ONCEJSON(root);
	sjson["b"].asUInt();
	Jassert(g_inst_num,0);
	}
	Jassert(g_inst_num,1);
	
	{
	g_inst_num = 0;
	SafeJson sjson=ONCEJSON(root);
	sjson["b"].asDouble();
	Jassert(g_inst_num,0);
	}
	Jassert(g_inst_num,1);
	
	{
	g_inst_num = 0;
	SafeJson sjson=ONCEJSON(root);
	sjson["b"].asBool();
	Jassert(g_inst_num,0);
	}
	Jassert(g_inst_num,1);
	
	{
	g_inst_num = 0;
	SafeJson sjson=ONCEJSON(root);
	sjson["b"].test(Json::intValue);
	Jassert(g_inst_num,0);
	}
	Jassert(g_inst_num,1);
	
	{
	g_inst_num = 0;
	SafeJson sjson=ONCEJSON(root);
	sjson["b"].test(Json::uintValue);
	Jassert(g_inst_num,0);
	}
	Jassert(g_inst_num,1);
	
	{
	g_inst_num = 0;
	SafeJson sjson=ONCEJSON(root);
	sjson["b"].test(Json::realValue);
	Jassert(g_inst_num,0);
	}
	Jassert(g_inst_num,1);
	
	{
	g_inst_num = 0;
	SafeJson sjson=ONCEJSON(root);
	sjson["b"].test(Json::stringValue);
	Jassert(g_inst_num,0);
	}
	Jassert(g_inst_num,1);
	
	{
	g_inst_num = 0;
	SafeJson sjson=ONCEJSON(root);
	sjson["b"].test(Json::arrayValue);
	Jassert(g_inst_num,0);
	}
	Jassert(g_inst_num,1);
	
	{
	g_inst_num = 0;
	SafeJson sjson=ONCEJSON(root);
	sjson["b"].test(Json::objectValue);
	Jassert(g_inst_num,0);
	}
	Jassert(g_inst_num,1);
	
	{
	g_inst_num = 0;
	SafeJson sjson=ONCEJSON(root);
	sjson["b"].test(Json::nullValue);
	Jassert(g_inst_num,0);
	}
	Jassert(g_inst_num,1);
	
	{
	g_inst_num = 0;
	SafeJson sjson=ONCEJSON(root);
	sjson["b"].test(Json::objectValue);
	Jassert(g_inst_num,0);
	}
	Jassert(g_inst_num,1);
	
	{
	g_inst_num = 0;
	SafeJson sjson=ONCEJSON(root);
	sjson["b"].test(Json::booleanValue);
	Jassert(g_inst_num,0);
	}
	Jassert(g_inst_num,1);
	
}


void testFLAG_STREQNUM(Json::Value& root)
{
	g_inst_num = 0;
	{
	SafeJson sjson(root,FLAG_STREQNUM);
	//Ĭ��ֵ����
	Jassert(sjson["a"]["b"].asString(),"");
	Jassert(sjson["a"]["b"].asCString(),NULL);
	Jassert(sjson["a"]["b"].asInt(),0);
	Jassert(sjson["a"]["b"].asUInt(),0u);
	Jassert(sjson["a"]["b"].asDouble(),0.0);
	
	//��ֵת�ַ���
	Jassert(sjson["b"].asString(),"123");
	Jassert(sjson["b"].asCString(),NULL);
	Jassert(sjson["b"].asInt(),123);
	Jassert(sjson["b"].asUInt(),123u);
	Jassert(sjson["b"].asDouble(),123.0);
	
	//�ַ���ת��ֵ
	Jassert(sjson["c"].asString(),"123");
	Jassert(sjson["c"].asCString(),root["c"].asCString());
	Jassert(sjson["c"].asInt(),123);
	Jassert(sjson["c"].asUInt(),123u);
	Jassert(sjson["c"].asDouble(),123.0);
	
	Jassert(g_inst_num,0);
	}
	
	Jassert(g_inst_num,1);
}


void testFLAG_STRICTMEMBER(Json::Value& root)
{
	g_inst_num = 0;
	{
	//asXXʧ��
	SafeJson sjson(root,FLAG_STRICTMEMBER);
	Jassert(sjson["a"].test(Json::nullValue),true);
	Jassert(sjson["a"].test(/*�ȼ���Json::nullValue����ʾ�����ֵ��Ŀ�����ͣ�ֻ���·��*/),true);
	Jassert(sjson["a"].test(Json::arrayValue),false);
	Jassert(sjson["a"].test(Json::objectValue),false);
	Jassert(sjson["a"].test(Json::intValue),false);
	Jassert(sjson["a"].test(Json::uintValue),false);
	Jassert(sjson["a"].test(Json::realValue),false);
	Jassert(sjson["a"].test(Json::booleanValue),true);//boolean is always ok
	Jassert(sjson["a"].test(Json::stringValue),false);
	
	//�±�ʧ��
	Jassert(sjson["e"].test(Json::nullValue),false);
	Jassert(sjson["e"].test(Json::arrayValue),false);
	Jassert(sjson["e"].test(Json::objectValue),false);
	Jassert(sjson["e"].test(Json::intValue),false);
	Jassert(sjson["e"].test(Json::uintValue),false);
	Jassert(sjson["e"].test(Json::realValue),false);
	Jassert(sjson["e"].test(Json::booleanValue),false);
	Jassert(sjson["e"].test(Json::stringValue),false);
	
	Jassert(sjson["a"][3].test(Json::nullValue),false);
	Jassert(sjson["a"][3].test(Json::arrayValue),false);
	Jassert(sjson["a"][3].test(Json::objectValue),false);
	Jassert(sjson["a"][3].test(Json::intValue),false);
	Jassert(sjson["a"][3].test(Json::uintValue),false);
	Jassert(sjson["a"][3].test(Json::realValue),false);
	Jassert(sjson["a"][3].test(Json::booleanValue),false);
	Jassert(sjson["a"][3].test(Json::stringValue),false);
	
	//�ɹ�
	Jassert(sjson["a"][0u].test(Json::intValue),true);
	Jassert(sjson["a"][0u].test(Json::uintValue),true);
	Jassert(sjson["a"][0u].test(Json::realValue),true);
	Jassert(sjson["a"][1].test(Json::stringValue),true);
	Jassert(sjson["a"][2].test(Json::booleanValue),true);
	Jassert(sjson["a"][2].test(Json::stringValue),false);//��Ȼboolean��asString�ǿ��Եĵ�����asCString�ǲ����Եģ�����asStringҲ������������

	
	Jassert(g_inst_num,0);
	}
	
	Jassert(g_inst_num,1);
}

void testException(Json::Value& root)
{
	g_inst_num = 0;
	{

	SafeJson sjson(root);
	//asXX
	Jassert(sjson["a"].test(Json::stringValue),false);	
	Jassert(sjson.getException().code,JSON_WRONG_ASSOMETYPE);
	Jassert(sjson.getException().info,"[Array]->[asString]");
	
	Jassert(sjson["a"].test(Json::intValue),false);	
	Jassert(sjson.getException().code,JSON_WRONG_ASSOMETYPE);
	Jassert(sjson.getException().info,"[Array]->[asInt]");
	
		Jassert(sjson["a"].test(Json::uintValue),false);	
	Jassert(sjson.getException().code,JSON_WRONG_ASSOMETYPE);
	Jassert(sjson.getException().info,"[Array]->[asUInt]");
	
		Jassert(sjson["a"].test(Json::realValue),false);	
	Jassert(sjson.getException().code,JSON_WRONG_ASSOMETYPE);
	Jassert(sjson.getException().info,"[Array]->[asDouble]");
	
		Jassert(sjson["a"].test(Json::objectValue),false);	
	Jassert(sjson.getException().code,JSON_WRONG_ASSOMETYPE);
	Jassert(sjson.getException().info,"[Array]->[asObject]");
	
		Jassert(sjson["a"].test(Json::arrayValue),false);	
	Jassert(sjson.getException().code,JSON_WRONG_ASSOMETYPE);
	Jassert(sjson.getException().info,"[Array]->[asArray]");
	
		Jassert(sjson["a"].test(Json::nullValue),true);//null��ʾֻ���·��	
	Jassert(sjson.getException().code,JSON_RIGHT);
	Jassert(sjson.getException().info,"");
	
	//not strict
	Jassert(sjson["e"].test(Json::arrayValue),true);	//�ն������Ȼת
	Jassert(sjson["e"].test(Json::objectValue),true);	//�ն������Ȼת
	Jassert(sjson["e"].test(Json::stringValue),true);
	Jassert(sjson["e"].test(Json::booleanValue),true);
	Jassert(sjson["e"].test(Json::uintValue),true);
	//can repeat
	Jassert(sjson["f"]["g"].test(Json::uintValue),true);
	root["f"]["g"].asString();
	Jassert(sjson["f"]["g"].test(Json::uintValue),true);	//asXX֮����ظ��ж�
	
	
	//["key"]
		Jassert(sjson["b"]["bbbbb"].test(Json::stringValue),false);	
	Jassert(sjson.getException().code,JSON_WRONG_GETOBJECT);
	Jassert(sjson.getException().info,"[Number:123]->[\"bbbbb\"]");//number���Եõ�����ֵ
	
	//[index]
		Jassert(sjson[6].test(Json::stringValue),false);	
	Jassert(sjson.getException().code,JSON_WRONG_GETARRAY);
	Jassert(sjson.getException().info,"[Object]->[6]");
	
	
	//strict
	SafeJson sjson1(root,FLAG_STRICTMEMBER);
	//������û���ַ��±�
	Jassert(sjson1["h"].test(Json::arrayValue),false);	
	Jassert(sjson1.getException().code,JSON_WRONG_NOMEMBER);
	Jassert(sjson1.getException().info,"[Object]->[\"h\"]");
	Jassert(sjson1["h"].test(Json::objectValue),false);	
	Jassert(sjson1.getException().code,JSON_WRONG_NOMEMBER);
	Jassert(sjson1.getException().info,"[Object]->[\"h\"]");
	
	//���Ӷ������ʹ���
	Jassert(sjson1["a"][0u].test(Json::stringValue),false);
	Jassert(sjson1.getException().code,JSON_WRONG_ASSOMETYPE);
	Jassert(sjson1.getException().info,"[Number:1]->[asString]");
	
	//0�±꣨NULL��������
	//ͬʱ���Գɹ�ʱ��exception�Բ���
	char *pc=NULL;
		Jassert(sjson1["a"][pc][1].test(Json::stringValue),true);//
	Jassert(sjson1.getException().code,JSON_RIGHT);
	Jassert(sjson1.getException().info,"");
	
	//�����±����
	Jassert(sjson1["a"][3].test(Json::stringValue),false);
	Jassert(sjson1.getException().code,JSON_WRONG_NOMEMBER);
	Jassert(sjson1.getException().info,"[Array]->[3]");
	
	//�����ַ��±����
		Jassert(sjson1["a"]["3"].test(Json::stringValue),false);
	Jassert(sjson1.getException().code,JSON_WRONG_GETOBJECT);
	Jassert(sjson1.getException().info,"[Array]->[\"3\"]");
	

	
	Jassert(g_inst_num,0);
	}
	Jassert(g_inst_num,2);
}

//���Ҫ������
void testrootnode(Json::Value& root)
{
	SafeJson sjson(root);
	sjson = 123;
	//assert error
}


void testDouble(Json::Value root)
{
	root["b"] = 123.99;
	g_inst_num = 0;
		
	{
	SafeJson sjson(root);

	Jassert(sjson["b"].asString(),"");
	Jassert(sjson["b"].asCString(),(const char*)NULL);
	Jassert(sjson["b"].asInt(),123);
	Jassert(sjson["b"].asUInt(),123u);
	Jassert(sjson["b"].asDouble(),123.99);

	SafeJson sjson1(root,FLAG_STREQNUM);

	Jassert(sjson1["b"].asString(),"123.990");
	Jassert(sjson1["b"].asCString(),(const char*)NULL);
	Jassert(sjson1["b"].asInt(),123);
	Jassert(sjson1["b"].asUInt(),123u);
	Jassert(sjson1["b"].asDouble(),123.99);
	Jassert(g_inst_num,0);
	}
	Jassert(g_inst_num,2);


}


int main()
{
	Json::Reader reader;
	Json::Value root;
	bool parsingSuccessful = reader.parse( JSONSTR, root );
	if ( !parsingSuccessful )
	{
		printf("Failed to parse %s\n",reader.getFormatedErrorMessages().c_str());
		return 1;
	}

	
	printf("=>%s\n",root.toStyledString().c_str());
	
	printf("====>testOnce\n");
	testOnce(root);
	
	printf("====>testDefault\n");
	testDefault(root);
	
	printf("====>testrelease\n");
	testrelease(root);
	
	printf("====>testFLAG_REWRITE\n");
	testFLAG_REWRITE(root);
	
	printf("====>testFLAG_ONLYONCE\n");
	testFLAG_ONLYONCE(root);
	
	printf("====>testFLAG_STREQNUM\n");
	testFLAG_STREQNUM(root);
	
	printf("====>testFLAG_STRICTMEMBER\n");
	testFLAG_STRICTMEMBER(root);
	
	printf("====>testException\n");
	testException(root);
	
	printf("====>testDouble\n");
	testDouble(root);
	/*
	Json::Value jv;
	jv["a"]["b"]["c"] = 123;
	Json::Value *pjv=&jv["a"]["b"]["c"];	
	std::cout << pjv << std::endl;
	
	SafeJson sj(jv,FLAG_REWRITE);
	sj["a"]["b"]["c"]["d"] = 3;
	pjv=&jv["a"]["b"]["c"];	
	std::cout <<jv["a"]["b"]["c"]["d"].asInt() << std::endl;
	std::cout << pjv <<std::endl;
	*/
	return 0;
   
}



   
