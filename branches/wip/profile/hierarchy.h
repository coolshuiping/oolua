#ifndef PROFILE_HIERARCHY_H_
#	define PROFILE_HIERARCHY_H_

class ProfileBase
{
public:
	ProfileBase():_i(0){}
	virtual ~ProfileBase(){}
	void increment_a_base(ProfileBase* base)
	{
		++base->_i;
	}
private:
	int _i;
};

class ProfileAnotherBase
{
public:
	virtual ~ProfileAnotherBase(){}
};
class ProfileDerived : public ProfileBase
{
public:

};

class ProfileMultiBases : public ProfileBase, public ProfileAnotherBase
{
public:
};

#endif
