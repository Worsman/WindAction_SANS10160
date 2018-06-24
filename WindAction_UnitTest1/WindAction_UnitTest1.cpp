#include "stdafx.h"
#include "CppUnitTest.h"
#include "../WindAction_Library/WindActionLib.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WindAction_UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			// TODO: Your test code here
      Assert::AreEqual(WindAction_library::WindAction::PeakWindSpeedPressure(1500, 28, 0.02, 12, 'A', 1.0), 929.47437481210955);

		}

	};
}