#include <iostream>

#include <ArcNet.h>

enum class CustomMsgTypes : uint32_t
{
	SpawnEntity,
	RemoveEntity
};

int main()
{
	arc::net::Message<CustomMsgTypes> msg;
	msg.header.id = CustomMsgTypes::SpawnEntity;

	int a = 1;
	bool b = true;
	float c = 3.14f;

	struct
	{
		float x = 0;
		float y = 1;
		float z = 2;
	} d[5];

	msg << a << b << c;

	a = 99;
	b = false;
	c = 90.0f;

	msg >> c >> b >> a;

	return 0;
}