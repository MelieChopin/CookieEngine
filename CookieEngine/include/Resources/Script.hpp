#ifndef __SCRIPT_HPP__
#define __SCRIPT_HPP__

#define SOL_ALL_SAFETIES_ON 1
#include "Sol/sol.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

/*
#ifdef _WIN32
#pragma comment (lib, "liblua54.a")
#endif*/

namespace Cookie
{

	namespace Resources
	{
		class ScriptObject;


		class Script
		{
		public:
			std::string filename;
			std::filesystem::file_time_type lastUpdateTime;

			sol::state state;
			sol::function construct;
			sol::function start;
			sol::function update;

			Script(const char* _filename) : filename(_filename)
			{
				lastUpdateTime = std::filesystem::last_write_time(filename);

				state.open_libraries(sol::lib::base);
				state.script_file(filename);
				construct = state["Construct"];
				update = state["Start"];
				update = state["Update"];
			}

			~Script() {}

			inline ScriptObject CreateObject(std::string entityId);
		};

		
		class ScriptObject
		{
		public:
			ScriptObject(Script& aScript, std::string entityId) : script(&aScript)
			{
				table = script->state[entityId].get_or_create<sol::table>();
				script->construct(table);
			}

			void Start() const { script->start(table); }
			void Update() const { script->update(table); }


			Script* script;
			sol::table table;
		};
		
		
		inline ScriptObject Script::CreateObject(std::string entityId)
		{
			return ScriptObject(*this, entityId);
		}


	}
}

#endif // !__SCRIPT_HPP__
