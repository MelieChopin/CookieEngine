#ifndef __SYSTEMHANDLER_INL__
#define __SYSTEMHANDLER_INL__


namespace Cookie
{
	namespace ECS
	{
		namespace System
		{
			inline void SystemPhysics(ComponentPhysics& ph, float factor)
			{
				ph.Update(factor);
			}
			inline void SystemDraw(ComponentTransform& trs, ComponentModel& model, const Core::Math::Mat4& proj, const Core::Math::Mat4& view, ID3D11Buffer** CBuffer)
			{
				trs.ComputeTRS();
				model.Draw(proj,view, trs.TRS, CBuffer);
			}
			inline void SystemScriptStart(const ComponentScript& script)
			{
				for (int i = 0; i < script.scripts.size(); ++i)
					script.scripts[i].Start();
			}
			inline void SystemScriptUpdate(ComponentScript& script)
			{
				for (int i = 0; i < script.scripts.size(); ++i)
					script.scripts[i].Update();
			}

		}
	}
}

#endif