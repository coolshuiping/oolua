
#	include "oolua.h"
#	include "cpp_class_ops.h"
#	include "lua_class_ops.h"

#	include "common_cppunit_headers.h"

const int int_not_set = 0;
const int int_set = 1;
const bool bool_not_set = false;
const bool bool_set = true;
const std::string string_not_set("not set");
const std::string string_set("hello world");
const char* c_string_not_set = "not set";
const char*  c_string_set= "hello world";



class ParamConstructor 
{
public:
	ParamConstructor(int i)
		:m_int(i)
		,m_bool(bool_not_set)
		,m_int_ptr(int_not_set)
		,m_string(string_not_set){}
	ParamConstructor(bool b)
		:m_int(int_not_set)
		,m_bool(b)
		,m_int_ptr(int_not_set)
		,m_string(string_not_set){}
	ParamConstructor(int *  i)
		:m_int(int_not_set)
		,m_bool(bool_not_set)
		,m_int_ptr(*i)
		,m_string(string_not_set){}
	ParamConstructor(char const *  str)
		:m_int(int_not_set)
		,m_bool(bool_not_set)
		,m_int_ptr(int_not_set)
		,m_string(str){}

	int m_int;
	bool m_bool;
	int m_int_ptr;
	std::string m_string;
};

OOLUA_CLASS_NO_BASES(ParamConstructor)
	OOLUA_TYPEDEFS No_default_constructor,Has_new_type_constructors OOLUA_END_TYPES
	OOLUA_CONSTRUCTORS_BEGIN
		OOLUA_CONSTRUCTOR_1(bool )
		OOLUA_CONSTRUCTOR_1(int )
		OOLUA_CONSTRUCTOR_1(char const*)
	OOLUA_CONSTRUCTORS_END
OOLUA_CLASS_END

EXPORT_OOLUA_NO_FUNCTIONS(ParamConstructor)

class Construct : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(Construct);
	CPPUNIT_TEST(new_luaCreatesInstanceThenReturnsIt_returnIsNoneNull);
	CPPUNIT_TEST(new_luaCreatesInstance_noException);
	CPPUNIT_TEST(register_noDefaultConstructor_compiles);
	CPPUNIT_TEST(new_CallingIntParamConstructor_runChunkReturnsTrue);
	CPPUNIT_TEST(new_CallingIntParamConstructorPassingInitailisedValue_InstanceHasIntSetToInitailisedValue);
	CPPUNIT_TEST(new_CallingBoolParamConstructor_runChunkReturnsTrue);
	CPPUNIT_TEST(new_CallingBoolParamConstructorPassingInitailisedValue_InstanceHasBoolSetToInitailisedValue);
	//To OOLUA there is no difference between a function that takes an int, int*, int& or any variation
	//the first registered constructor which matches will be called.
	//CPPUNIT_TEST(new_CallingIntPtrParamConstructor_runChunkReturnsTrue);
	//CPPUNIT_TEST(new_CallingIntPtrParamConstructorPassingInitailisedValue_InstanceHasBoolSetToInitailisedValue);
	CPPUNIT_TEST(new_CallingCharConstPtrParamConstructor_runChunkReturnsTrue);
	CPPUNIT_TEST(new_CallingCharConstPtrParamConstructorPassingInitailisedValue_InstanceHasStringSetToInitailisedValue);
	CPPUNIT_TEST_SUITE_END();

	OOLUA::Script * m_lua;
	std::string register_and_create_one_param_constructor()
	{
		m_lua->register_class<ParamConstructor>();
		m_lua->run_chunk("foo = function(i) return ParamConstructor:new(i) end");
		return "foo";
	}
	struct ParamConstructorWrapper 
	{
		OOLUA::cpp_acquire_ptr<ParamConstructor> instance;
		std::auto_ptr<ParamConstructor> auto_delete_instance;
		void set_ptr_to_auto_delete()
		{
			auto_delete_instance = std::auto_ptr<ParamConstructor>(instance.m_ptr);
		}
	};
	void pull_ParamWrapper(ParamConstructorWrapper& wrap)
	{
		OOLUA::pull2cpp(*m_lua,wrap.instance);
		wrap.set_ptr_to_auto_delete();
	}
public:
    Construct():m_lua(0){}
    LVD_NOCOPY(Construct)
	void setUp()
	{
		m_lua = new OOLUA::Script;
		m_lua->register_class<Class_ops>();
	}
	void tearDown()
	{
		delete m_lua;
	}

	void new_luaCreatesInstanceThenReturnsIt_returnIsNoneNull()
	{
		createAndReturnClassOps(m_lua);
		OOLUA::cpp_acquire_ptr<Class_ops> res;
		OOLUA::pull2cpp(*m_lua,res);
		CPPUNIT_ASSERT_EQUAL(true, res.m_ptr != 0);
		delete res.m_ptr;
	}
	void new_luaCreatesInstance_noException()
	{
		std::string foo("Class_ops:new()");
		CPPUNIT_ASSERT_NO_THROW( m_lua->run_chunk(foo) );
	}

	void createAndReturnClassOps(OOLUA::Script *lua)
	{
		std::string foo(\
			"createAndReturn = function() \n"
				"return Class_ops:new() \n"
			"end");
		lua->run_chunk(foo);
		CPPUNIT_ASSERT_NO_THROW( lua->call("createAndReturn") );
	}
	
	void register_noDefaultConstructor_compiles()
	{
		m_lua->register_class<ParamConstructor>();
	}
	void new_CallingIntParamConstructor_runChunkReturnsTrue()
	{
		m_lua->register_class<ParamConstructor>();
		bool result = m_lua->run_chunk("ParamConstructor:new(1)");
		CPPUNIT_ASSERT_EQUAL(true,result);
		
	}
	void new_CallingIntParamConstructorPassingInitailisedValue_InstanceHasIntSetToInitailisedValue()
	{
		m_lua->call(register_and_create_one_param_constructor(),int_set);
		ParamConstructorWrapper wrap;
		pull_ParamWrapper(wrap);
		CPPUNIT_ASSERT_EQUAL(int_set,wrap.instance.m_ptr->m_int);
	}
	void new_CallingBoolParamConstructor_runChunkReturnsTrue()
	{
		m_lua->register_class<ParamConstructor>();
		bool result = m_lua->run_chunk("ParamConstructor:new(true)");
		CPPUNIT_ASSERT_EQUAL(true,result);
		
	}
	
	void new_CallingBoolParamConstructorPassingInitailisedValue_InstanceHasBoolSetToInitailisedValue()
	{
		m_lua->call(register_and_create_one_param_constructor(),bool_set);
		ParamConstructorWrapper wrap;
		pull_ParamWrapper(wrap);
		CPPUNIT_ASSERT_EQUAL(bool_set,wrap.instance.m_ptr->m_bool);
	}
	
	void new_CallingIntPtrParamConstructor_runChunkReturnsTrue()
	{
		m_lua->register_class<ParamConstructor>();
		bool result = m_lua->run_chunk("ParamConstructor:new(1)");
		CPPUNIT_ASSERT_EQUAL(true,result);
		
	}
	void new_CallingIntPtrParamConstructorPassingInitailisedValue_InstanceHasBoolSetToInitailisedValue()
	{
		m_lua->call(register_and_create_one_param_constructor(),int_set);
		ParamConstructorWrapper wrap;
		pull_ParamWrapper(wrap);
		CPPUNIT_ASSERT_EQUAL(int_set,wrap.instance.m_ptr->m_int_ptr);
	}
	
	
	
	
	void new_CallingCharConstPtrParamConstructor_runChunkReturnsTrue()
	{
		m_lua->register_class<ParamConstructor>();
		bool result = m_lua->run_chunk("ParamConstructor:new(\"do not care\")");
		CPPUNIT_ASSERT_EQUAL(true,result);
	}
	void new_CallingCharConstPtrParamConstructorPassingInitailisedValue_InstanceHasStringSetToInitailisedValue()
	{
		m_lua->call(register_and_create_one_param_constructor(),c_string_set);
		ParamConstructorWrapper wrap;
		pull_ParamWrapper(wrap);
		CPPUNIT_ASSERT_EQUAL(string_set,wrap.instance.m_ptr->m_string);
	}
	
};

CPPUNIT_TEST_SUITE_REGISTRATION( Construct );

