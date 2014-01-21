#include "SafeJson.h"


//�Ƿ���Ҫ����ԭֵ
#define ISREWRITE(arg) ((arg) & FLAG_REWRITE)
//�Ƿ��ַ��������ּ��Զ�ת
#define ISSTREQNUM(arg) ((arg) & FLAG_STREQNUM)
//�Ƿ��ϸ����Ա
#define ISSTRICTMEMBER(arg)	((arg) & FLAG_STRICTMEMBER)
//״̬���ԣ����Ѿ��������Ҳ��Ǹ���ģʽ����ִ��FalseAction
#define CHECK_WRONG(FalseAction) if(m_state != JSON_RIGHT){ FalseAction ;}
//reset֮��return
#define RESET_RETURN(arg) reset();return (arg)
//�����쳣��Ϣ
#define SETEXCEPTION(ecode,einfo) m_exception.code=ecode;m_exception.info=einfo
//�Ƿ���������
#define ISNUMBERTYPE(ptrjson) ((ptrjson)->isInt() || (ptrjson)->isUInt() || (ptrjson)->isDouble())
//ƴ��3���ַ�����������3�����������ͣ�����Ϊint��string�ȼ�����
#define CREATESTREAM(streamname,str1,str2,str3) std::stringstream streamname;streamname << (str1) << (str2) << (str3)


#ifdef _UNITTEST_
	int g_inst_num = 0;	//���Զ����ͷ�
#endif


//����
SafeJson::SafeJson(Json::Value& json,int param)
{
	//���洫���ֵ
	m_pjson = &json;
	m_param = param;
	//��ʼ��һЩֵ
	m_pjsonLast = m_pjson;
	setState(JSON_RIGHT,"");
}

//����
SafeJson::~SafeJson()
{
#ifdef _UNITTEST_
	//std::cout << ">>Release SafeJson." << std::endl;
	g_inst_num ++;
#endif
}

//�����ڲ�����
void SafeJson::reset()
{
	//jsonָ��ָ���ʼ��λ�ò��ָ�״̬;
	m_pjsonLast = m_pjson;
	m_state=JSON_RIGHT; //ֻ��״̬����exception
}

//ȡjson�Ӷ���
SafeJson& SafeJson::operator[]( const char *key )
{
	CHECK_WRONG(return *this);
	if(NULL == key){return *this;}	//���˿�ָ�룬ɶҲ����
	
	if(m_pjsonLast->isObject())
	{
		if(ISSTRICTMEMBER(m_param) && !(m_pjsonLast->isMember(key)))
		{
			CREATESTREAM(ss, "[\"", key, "\"]");
			setState(JSON_WRONG_NOMEMBER,ss.str());
		}
		else
		{
			m_pjsonLast = &(*m_pjsonLast)[key];
		}
	}
	else
	{
		if(!ISREWRITE(m_param))
		{
			//���ñ�־λ������Ϳ���ignore�����Ķ�д�����ˣ�ֱ��reset��
			CREATESTREAM(ss, "[\"", key, "\"]");
			setState(JSON_WRONG_GETOBJECT,ss.str());
		}
		else
		{
			//���ǵ�ԭ����ֵ
			(*m_pjsonLast) = Json::Value(Json::objectValue);
			m_pjsonLast = &(*m_pjsonLast)[key];
		}
	}
	
	return *this;
}

//ȡjson�Ӷ���
SafeJson& SafeJson::operator[]( const std::string &key )
{
	return (*this)[ key.c_str() ];
}

//ȡjson�Ӷ���
SafeJson& SafeJson::operator[](unsigned int index)
{
	CHECK_WRONG(return *this);
	
	//���ȡ�±겻�����
	if(m_pjsonLast->isArray())
	{
		//����������ϸ�ĳ�Ա��飬����Ŀ���±곬���˵�ǰ��С�������ô�����Ϣ��
		//���������������assert���󣬵���ҵ���Ͽ�����Ҫ�����ϸ���Ĺ���
		if(ISSTRICTMEMBER(m_param) && (index >=  m_pjsonLast->size()))
		{
			//�����ַ���
			CREATESTREAM(ss, "[", index, "]");
			setState(JSON_WRONG_NOMEMBER,ss.str());
		}
		else
		{
			//ֱ�ӽ����Ӷ���
			m_pjsonLast = &(*m_pjsonLast)[index];
		}
	}
	else
	{
		if(!ISREWRITE(m_param))
		{
			//���ñ�־λ������Ϳ���ignore�����Ķ�д�����ˣ�ֱ��reset��
			CREATESTREAM(ss, "[", index, "]");
			setState(JSON_WRONG_GETARRAY,ss.str());
		}
		else
		{
			(*m_pjsonLast) = Json::Value(Json::arrayValue);
			m_pjsonLast = &(*m_pjsonLast)[index];
		}
	}
	
	return *this;

}

void SafeJson::setState(JSON_STATE state,const std::string& actionstr)
{
	m_state=(state); 
	if(JSON_RIGHT == state)
	{
		//���Եڶ�������
		SETEXCEPTION(JSON_RIGHT,"");
	}
	else
	{
		SETEXCEPTION(state,getCurValueTag()+"->"+actionstr);
	}

}

std::string SafeJson::getasXXTag(Json::ValueType jsontype)
{
	switch(jsontype)
	{
		case Json::intValue:
			return "[asInt]";
		case Json::uintValue:
			return "[asUInt]";
		case Json::realValue:
			return "[asDouble]";
		case Json::stringValue:
			return "[asString]";
		case Json::booleanValue:
			return "[asBoolean]";
		case Json::arrayValue:
			return "[asArray]";
		case Json::objectValue:
			return "[asObject]";
		case Json::nullValue:
			return "[asNull]";
		default:
			//UNREACHABLE
			return "[asUnknown]";
			break;
		
	}
}

//��ѯ��ǰ��״̬����ȷ�����쳣
const JsonException& SafeJson::getException()
{
	return m_exception;
}

void SafeJson::trimRightLn(std::string& str)
{
	//find_last_of �����һ�γ��ֵ�λ�ã����Ҫ�ҵ��ַ������ȴ���1�����ص����ҵ����ַ������Ҷ˵�λ��
	size_t  sz = str.find_last_of('\n');
    if(std::string::npos != sz)
    {
        str.erase(sz,std::string::npos);                
    }

}

std::string SafeJson::getCurValueTag()
{
	if(ISNUMBERTYPE(m_pjsonLast))
	{
		std::string tmpstr = m_pjsonLast->toStyledString();
		//ɾ��ĩβ���з�
		trimRightLn(tmpstr);
		return "[Number:" + tmpstr +"]";
	}
	else if(m_pjsonLast->isArray())
	{
		return "[Array]";
	}
	else if(m_pjsonLast->isObject())
	{
		return "[Object]";
	}
	else if(m_pjsonLast->isString())
	{
		return "[String]";
		//�����ַ�����Ҫ�Ļ����Լ��ϣ�m_pjsonLast->asString();�����ַ������ܺܳ���ռ�ڴ�
	}
	else if(m_pjsonLast->isBool())
	{
		CREATESTREAM(ss,"[Bool:", m_pjsonLast->asBool(), "]");
		return ss.str();
	}
	else
	{	
		//isNull()
		return "[Null]";
	}
		
}

SafeJson& SafeJson::operator=( const SafeJson& other )
{
	//UNREACHABLE
	return *this;
}

//��ֵ
SafeJson& SafeJson::operator=( const Json::Value& other )
{
	CHECK_WRONG(RESET_RETURN(*this));
	//�������Ա���ֵ�������jsoncpp��ֵ���ַ����ı䣬
	//�����ĵ�ַ��Զ�����ı�,���Ը�ֵû��ϵ
	//assert(m_pjson != m_pjsonLast);	//���ܰѸ��ڵ㸲�ǵ�(X)
	(*m_pjsonLast) = other;
	RESET_RETURN(*this);
}


//�Ƿ�����Ȼת��
bool SafeJson::test(Json::ValueType jsontype)
{
	CHECK_WRONG(RESET_RETURN(false));
	
	if(Json::nullValue == jsontype)
	{
		//Json::nullValue��ʾ������ת��ĳ�����ͣ�ֻ���·��������ֱ��true
		setState(JSON_RIGHT,"");
		RESET_RETURN(true);
	}
	
	if(canNatureConvert(*m_pjsonLast,jsontype))
	{
		setState(JSON_RIGHT,"");
		RESET_RETURN(true);
	}
	else
	{
		setState(JSON_WRONG_ASSOMETYPE, getasXXTag(jsontype));
		RESET_RETURN(false);
	}
}

//ת��string������asXX���߼��������ͬ��ע�����ƣ��Ͳ�д��
std::string SafeJson::asString()
{
	//���ǰ���Ѿ�����[]ȡ�Ӷ��������ô��������ͺ���֮
	//RESET_RETURN�����ò���Ȼ�󷵻�Ŀ��ֵ
	CHECK_WRONG(RESET_RETURN(""));
	
	//����ܹ���Ȼת��(asString())��string����.
	if(canNatureConvert(*m_pjsonLast,Json::stringValue))
	{
		//��ΪRESET_RETURN�����ò���������m_pjsonLast���ֵҪ�ݴ�����
		Json::Value	*tmp_pjson = m_pjsonLast;	
		//���ò���Ȼ�󷵻�Ŀ��ֵasString()
		RESET_RETURN(tmp_pjson->asString());
	}
	else if(ISSTREQNUM(m_param) && ISNUMBERTYPE(m_pjsonLast))
	{
		//���������Ȼת���������������ַ����ֵȼ۵ı�־���ҵ�ǰ��������ֵ���ͣ���ô�ͽ���ת��
		std::string tmpstr = m_pjsonLast->toStyledString();
		//ɾ��ĩβ���з�
		trimRightLn(tmpstr);
		//���ò���Ȼ�󷵻�Ŀ��ֵtmpstr
		RESET_RETURN(tmpstr);
	}
	else
	{
		//��������޷�ת�����ͷ���Ĭ��ֵ
		RESET_RETURN("");
	}

}

//ת��char*���ܷ���NULL!
const char* SafeJson::asCString()
{
	CHECK_WRONG(RESET_RETURN(NULL));
	
	//�������Ȼת��
	if(canNatureConvert(*m_pjsonLast,Json::stringValue))
	{
		Json::Value	*tmp_pjson = m_pjsonLast;
		RESET_RETURN(tmp_pjson->asCString());
	}
	else
	{
		//asCString�޷�ʵ����ֵ��ת��asString����		
		RESET_RETURN(NULL);
	}
}

//ת��bool
bool SafeJson::asBool()
{
	CHECK_WRONG(RESET_RETURN(false));
	
	//it is always safe
	if(canNatureConvert(*m_pjsonLast,Json::booleanValue))
	{
		Json::Value	*tmp_pjson = m_pjsonLast;
		RESET_RETURN(tmp_pjson->asBool());
	}
	else
	{
		//UNREACHABLE
		RESET_RETURN(false);
	}
	

}

//ת��double
double SafeJson::asDouble()
{
	CHECK_WRONG(RESET_RETURN(0.0));
	
	if(canNatureConvert(*m_pjsonLast,Json::realValue))
	{
		Json::Value	*tmp_pjson = m_pjsonLast;
		RESET_RETURN(tmp_pjson->asDouble());
	}
	else if(ISSTREQNUM(m_param) && m_pjsonLast->isString())
	{
		//�ַ���ת����ֵ
		std::string tmpstr = m_pjsonLast->asString();
		double tmpret = strtod(tmpstr.c_str(),NULL);
		RESET_RETURN(tmpret);
	}
	else
	{
		RESET_RETURN(0.0);
	}
}

//ת��int
int SafeJson::asInt()
{
	CHECK_WRONG(RESET_RETURN(0));
	
	if(canNatureConvert(*m_pjsonLast,Json::intValue))
	{
		Json::Value	*tmp_pjson = m_pjsonLast;
		RESET_RETURN(tmp_pjson->asInt());
	}
	else if(ISSTREQNUM(m_param) && m_pjsonLast->isString())
	{
		//�ַ���ת����ֵ
		std::string tmpstr = m_pjsonLast->asString();
		int tmpret = atoi(tmpstr.c_str());
		RESET_RETURN(tmpret);
	}
	else
	{
		RESET_RETURN(0);
	}
	

}

//ת��uint
unsigned int SafeJson::asUInt()
{
	CHECK_WRONG(RESET_RETURN(0u));
	
	if(canNatureConvert(*m_pjsonLast,Json::uintValue))
	{
		Json::Value	*tmp_pjson = m_pjsonLast;
		RESET_RETURN(tmp_pjson->asUInt());
	}
	else if(ISSTREQNUM(m_param) && m_pjsonLast->isString())
	{
		//�ַ���ת����ֵ
		std::string tmpstr = m_pjsonLast->asString();
		unsigned int tmpret = atoi(tmpstr.c_str());
		RESET_RETURN(tmpret);
	}
	else
	{
		RESET_RETURN(0u);
	}

}

//�Ƿ��ת
bool SafeJson::canNatureConvert(Json::Value& json,Json::ValueType jsontype)
{
	if (isRealSafe(json,jsontype))
	{
		return true;
	}
	switch (jsontype)
	{
		case Json::intValue:
		case Json::uintValue:
		case Json::realValue:
			return json.isInt()
				|| json.isUInt()
				|| json.isDouble();
		case Json::stringValue:
			//boolean���Ϳ���asString()�����ǲ���asCString()������ͳһ��boolean���͵�������תString
			return json.isString();
		case Json::booleanValue:
			//isRealSafe���Ѿ�������
			return true;	
		case Json::nullValue:
		case Json::arrayValue:
		case Json::objectValue:
			//objectValue�����ǲ���ת��ȥ��Ҳ������asObject�ӿ�
		default:			
			return false;			
	
	}
	//UNREACHABLE
	return false;
}


/*
��Ȼ��ת�����
*/
bool SafeJson::isRealSafe(Json::Value& json,Json::ValueType jsontype)
{
	return (json.isNull()) || (Json::booleanValue == jsontype);
}



