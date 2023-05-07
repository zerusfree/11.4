#include "pch.h"
#include "CppUnitTest.h"
#include "../Source.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest114
{
	TEST_CLASS(UnitTest114)
	{
	public:
		
        TEST_METHOD(TestMethod1)
        {
            string filename = "routes.bin";
            fstream f(filename, std::ios::binary | std::ios::in | std::ios::out);

            Route routes[] = {
                {"A", "C", 3},
                {"C", "B", 2},
                {"B", "A", 1},
            };

            for (int i = 0; i < sizeof(routes) / sizeof(Route); i++) {
                f.write((char*)&routes[i], sizeof(Route));
            }

            f.close();

            Sort(filename);

            f.open(filename, std::ios::binary | std::ios::in);
            Route sortedRoutes[sizeof(routes) / sizeof(Route)];
            for (int i = 0; i < sizeof(routes) / sizeof(Route); i++) {
                sortedRoutes[i] = fRead(f, i);
            }
            f.close();

            for (int i = 0; i < sizeof(sortedRoutes) / sizeof(Route) - 1; i++) {
                Assert::IsTrue(sortedRoutes[i].numberOfRoute <= sortedRoutes[i + 1].numberOfRoute);
            }
        }
	};
}