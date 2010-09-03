
#include "common_cppunit_headers.h"
#include "oolua.h"
#include "expose_shared_ptr.h"

class Shared_pointer : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(Shared_pointer);
	CPPUNIT_TEST(registerClass_compiles);
	CPPUNIT_TEST(callConstMethod_whichIsRegisteredInUnderlyingType_doesNotThrowAnException);
	CPPUNIT_TEST(push_byValue_countIncreasesByOne);
	CPPUNIT_TEST(gcMethod_pushSharedPointerThenPullAConstBaseClassAndCallGc_countIsTheSameAsBeforePush);
	CPPUNIT_TEST(push_SameInstanceTwice_countIsIncreasedByOne);
	CPPUNIT_TEST_SUITE_END();
	OOLUA::Script* m_lua;
public:
	Shared_pointer():m_lua(0){}
	void setUp()
	{
		m_lua = new OOLUA::Script;
	}
	void tearDown()
	{
		delete m_lua;
	}
	void registerClass_compiles()
	{
		m_lua->register_class<OOLUA::Stub1_shared_ptr>();
	}
	void callConstMethod_whichIsRegisteredInUnderlyingType_doesNotThrowAnException()
	{
		m_lua->register_class<OOLUA::Shared_ptr_stub>();
		OOLUA::Shared_ptr_stub sp = OOLUA::Shared_ptr_stub( new Stub_for_shared_ptr);
		m_lua->run_chunk("func = function(obj) "
						 "obj:method() "
						 "end");
		CPPUNIT_ASSERT_NO_THROW(m_lua->call("func",sp));
	}
	
	void push_byValue_countIncreasesByOne()
	{
		OOLUA::register_class_and_bases<OOLUA::Shared_ptr_stub>(*m_lua);
		OOLUA::Shared_ptr_stub sp ( new Stub_for_shared_ptr);
		long before = sp.use_count();
		OOLUA::push2lua(*m_lua,sp);
		CPPUNIT_ASSERT_EQUAL(before+1,sp.use_count()); 
		
	}
	
	//need a special delete function registered for shared pointers
	void gcMethod_pushSharedPointerThenPullAConstBaseClassAndCallGc_countIsTheSameAsBeforePush()
	{
		OOLUA::register_class_and_bases<OOLUA::Shared_ptr_stub>(*m_lua);
		OOLUA::Shared_ptr_stub sp ( new Stub_for_shared_ptr);
		long before =sp.use_count();
		OOLUA::push2lua(*m_lua,sp);
		//can not yet pull a shared pointer but we can a constant base class
		Stub_for_shared_ptr const * returned_pointer;
		OOLUA::pull2cpp(*m_lua,returned_pointer);
		m_lua->gc();
		CPPUNIT_ASSERT_EQUAL(before,sp.use_count());
	}

	void push_SameInstanceTwice_countIsIncreasedByOne()
	{
		OOLUA::register_class_and_bases<OOLUA::Shared_ptr_stub>(*m_lua);
		OOLUA::Shared_ptr_stub sp ( new Stub_for_shared_ptr);
		long before =sp.use_count();
		OOLUA::push2lua(*m_lua,sp);
		OOLUA::push2lua(*m_lua,sp);
		CPPUNIT_ASSERT_EQUAL(before+1,sp.use_count()); 
	}



};
CPPUNIT_TEST_SUITE_REGISTRATION(Shared_pointer);


