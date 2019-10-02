#pragma once

#include <string_view>

class SCFG
{
public:
	SCFG() = delete;
	SCFG(std::string_view path);
	SCFG(const SCFG& o) = delete;
	SCFG(SCFG&& o);

	~SCFG();

	void LoadConfig();
	void SaveProfile();

	template<class T>
	T GetValueByName(const std::string_view name);
	
	 template<class T>
	 void SetValue(const std::string_view key, const T& value);



private:
};
