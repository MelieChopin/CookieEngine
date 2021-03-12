#ifndef __SCRIPT_HPP__
#define __SCRIPT_HPP__

#include "Time.hpp"
#include "Debug.hpp"

#include <iostream>

namespace Cookie
{

	namespace Resources
	{
		class Script
		{
		public:

			virtual const char* GetName() const = 0; //typeid(*this).name();
			virtual void Start() = 0;
			virtual void Update() = 0;
		};


		class ScriptHelloWorld : public Script
		{
		public:
			ScriptHelloWorld() {};
			~ScriptHelloWorld() {};

			const char* GetName() const override { return "Script Hello World"; }
			void Start()		  override { CDebug.Log("Hello World start"); }
			void Update()		  override { CDebug.Log("Hello World update"); }
		};

		class ScriptWarning : public Script
		{
		public:
			ScriptWarning() {};
			~ScriptWarning() { };

			float cooldownMax = 2;
			float cooldownCurrent = 2;

			const char* GetName() const override { return "Script Warning"; }
			void Start()		  override { CDebug.Log("I will pop Warning"); }
			void Update()		  override
			{
				cooldownCurrent -= ImGui::GetIO().DeltaTime;

				if (cooldownCurrent < 0)
				{
					CDebug.Warning("This Warning will come back in 2s");
					cooldownCurrent = cooldownMax;
				}
			}
		};
	}
}

#endif // !__SCRIPT_HPP__
