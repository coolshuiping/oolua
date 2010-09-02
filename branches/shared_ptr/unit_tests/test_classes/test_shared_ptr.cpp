
#include "common_cppunit_headers.h"
#include "oolua.h"
#include "expose_shared_ptr.h"

class Shared_pointer : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(Shared_pointer);
	CPPUNIT_TEST(registerClass_compiles);
	CPPUNIT_TEST(callMethod_whichIsRegisteredInUnderlyingType_doesNotThrowAnException);
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
	void callMethod_whichIsRegisteredInUnderlyingType_doesNotThrowAnException()
	{
		m_lua->register_class<OOLUA::Shared_ptr_stub>();
		CPPUNIT_ASSERT_NO_THROW(
								m_lua->run_chunk("func = function(obj) "
												 "obj:method() "
												"end")
								);
	}


};
CPPUNIT_TEST_SUITE_REGISTRATION(Shared_pointer);


